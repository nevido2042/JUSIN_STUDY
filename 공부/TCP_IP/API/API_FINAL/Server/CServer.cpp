#include <locale>
#include <iostream>
#include "CServer.h"

CServer::CServer()
    :m_ServSock(INVALID_SOCKET),
    m_iSessionCnt(0),
    m_iID(0)
{
    ZeroMemory(&m_tReadSet, sizeof(fd_set));
}

CServer::~CServer()
{
}

bool CServer::Initialize()
{
    setlocale(LC_ALL, "KOREAN");

    // 윈속 초기화
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        wprintf_s(L"WSAStartup() error:%d\n", WSAGetLastError());
        return false;
    }
    wprintf_s(L"Success WSAStartup()\n");

    // 서버 소켓 생성
    m_ServSock = socket(AF_INET, SOCK_STREAM, 0);
    if (m_ServSock == INVALID_SOCKET)
    {
        wprintf_s(L"socket() error:%d\n", WSAGetLastError());
        return false;
    }
    wprintf_s(L"Success socket()\n");

    // 넌블로킹 소켓 설정
    u_long mode = 1;
    ioctlsocket(m_ServSock, FIONBIO, &mode);

    // 서버 주소 설정
    SOCKADDR_IN servAdr;
    ZeroMemory(&servAdr, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAdr.sin_port = htons(_wtoi(PORT));

    // 바인드
    if (bind(m_ServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
    {
        wprintf_s(L"bind() error:%d\n", WSAGetLastError());
        return false;
    }
    wprintf_s(L"Success bind()\n");

    // 리슨
    if (listen(m_ServSock, 5) == SOCKET_ERROR)
    {
        wprintf_s(L"listen() error:%d\n", WSAGetLastError());
        return false;
    }
    wprintf_s(L"Success listen()\n");

    wprintf_s(L"서버 실행 성공\n");

    return true;
}

bool CServer::Update()
{
    return Network();
}

void CServer::Release()
{
}

bool CServer::Network()
{
    fd_set tCpySet;
    FD_ZERO(&m_tReadSet);
    FD_SET(m_ServSock, &m_tReadSet);
    TIMEVAL tTimeOut = { 0, 0 };

    for (int i = 0; i < m_iSessionCnt; i++)
    {
        FD_SET(m_arrSession[i].clntSock, &m_tReadSet);
    }

    tCpySet = m_tReadSet;

    int iResult = select(0, &tCpySet, NULL, NULL, &tTimeOut);
    if (iResult == SOCKET_ERROR) 
    {
        wprintf_s(L"select() error:%d\n", WSAGetLastError());
        return false;
    }

    if (iResult > 0)
    {
        for (u_int i = 0; i < m_tReadSet.fd_count; i++)
        {
            if (FD_ISSET(m_tReadSet.fd_array[i], &tCpySet))
            {
                if (m_tReadSet.fd_array[i] == m_ServSock)
                {
                    AcceptProc();// 클라이언트 연결 처리
                }
                else
                {
                    for (int j = 0; j < m_iSessionCnt; j++)
                    {
                        if (m_tReadSet.fd_array[i] == m_arrSession[j].clntSock)
                        {
                            Read_Proc(&m_arrSession[j]); // 데이터 수신 처리
                        }
                    }
                }
            }
        }
    }

    Send_Message();
    return true;
}

void CServer::AcceptProc()
{
    while (true)
    {
        SOCKADDR_IN clntAdr;
        int iAdrSize = sizeof(clntAdr);
        SOCKET clntSock = accept(m_ServSock, (SOCKADDR*)&clntAdr, &iAdrSize);

        if (clntSock == INVALID_SOCKET)
        {
            int iErrCode = WSAGetLastError();
            if (iErrCode == WSAEWOULDBLOCK)
                break;

            wprintf_s(L"accept() error: %d\n", iErrCode);
            return;
        }

        cout << "ID:" << m_iID << " 클라이언트 접속" << endl;

        m_arrSession[m_iSessionCnt] = { clntAdr, clntSock, m_iID, rand() % 800, rand() % 600 };
        //링버퍼 초기화
        m_arrSession[m_iSessionCnt].recvQ = CRingBuffer(5000);
        m_arrSession[m_iSessionCnt].sendQ = CRingBuffer(5000);

        // 클라이언트에게 ID 전송
        MSG_ALLOC_ID msgAllocID = { ALLOC_ID, m_iID };
        Send_Unicast(&m_arrSession[m_iSessionCnt], (MSG_BASE*)&msgAllocID, sizeof(msgAllocID));

        // 기존 클라이언트 정보 전송
        MSG_CREATE_PLAYER tMSG = { CREATE_PLAYER, m_iID };
        for (int i = 0; i < m_iSessionCnt + 1; i++)
        {
            tMSG.id = m_arrSession[i].id;
            tMSG.x = m_arrSession[i].x;
            tMSG.y = m_arrSession[i].y;
            Send_Unicast(&m_arrSession[m_iSessionCnt], (MSG_BASE*)&tMSG, sizeof(tMSG));
        }

        Send_Broadcast(NULL, (MSG_BASE*)&tMSG, sizeof(tMSG));

        m_iSessionCnt++;
        m_iID++;
    }
}

void CServer::Send_Unicast(SESSION* pSession, const MSG_BASE* pMSG, const int iSize)
{
    int iResult{ 0 };
    iResult = pSession->sendQ.Enqueue((char*)pMSG, iSize);
    if (iResult < iSize)
    {
        wprintf_s(L"sendQ.Enqueue() error\n");
        exit(1);
    }
}

void CServer::Send_Broadcast(SESSION* pSession, const MSG_BASE* pMSG, const int iSize)
{
    int iResult{ 0 };
    for (int i = 0; i < m_iSessionCnt; i++)
    {
        if (pSession == &m_arrSession[i])
            continue;

        iResult = m_arrSession[i].sendQ.Enqueue((char*)pMSG, iSize);
        if (iResult < iSize)
        {
            wprintf_s(L"sendQ.Enqueue() error\n");
            exit(1);
        }
    }

}

void CServer::Read_Proc(SESSION* pSession)
{
    char Buffer[BUF_SIZE];

    int iResult = recv(pSession->clntSock, Buffer, sizeof(Buffer), 0);
    int iErrCode = WSAGetLastError();

    if (iResult == 0 || (iResult == SOCKET_ERROR && iErrCode != WSAEWOULDBLOCK))
    {
        if (iErrCode)
        {
            wprintf_s(L"recv() error: %d\n", iErrCode);
        }

        // 세션 목록에서 제거
        for (int i = 0; i < m_iSessionCnt - 1; i++)
        {
            if (m_arrSession[i].clntSock == pSession->clntSock)
            {
                m_arrSession[i] = m_arrSession[m_iSessionCnt - 1];
                m_iSessionCnt--;
                break;
            }
        }

        // 소켓 정리
        FD_CLR(pSession->clntSock, &m_tReadSet);
        closesocket(pSession->clntSock);
    }

    pSession->recvQ.Enqueue(Buffer, iResult);

    while (true)
    {
        if (pSession->recvQ.GetUseSize() < MSG_SIZE)
            break;//중단

        char Msg[MSG_SIZE];

        int iResult{ 0 };
        iResult = pSession->recvQ.Dequeue(Msg, MSG_SIZE);
        if (iResult != MSG_SIZE)
        {
            //결함
            exit(1);
        }

        int iType{ 0 };
        if (iResult > 0)
        {
            memcpy(&iType, Buffer, sizeof(int));
        }

        Decode_Message(iType, Buffer);
    }
}

void CServer::Decode_Message(int iType, char* pBuffer)
{
    switch (iType)
    {
    case MOVE_RIGHT_PLAYER:
    {
        MSG_MOVE_RIGHT_PLAYER& recvMSG = (MSG_MOVE_RIGHT_PLAYER&)*pBuffer;
        wprintf_s(L"%d: MOVE_RIGHT_PLAYER\n", recvMSG.id);

        MSG_MOVE_RIGHT_PLAYER msgMoveRight;
        msgMoveRight.id = recvMSG.id;

        Send_Broadcast(NULL, (MSG_BASE*)&msgMoveRight, sizeof(MSG_MOVE_RIGHT_PLAYER));
        break;
    }
    case MOVE_LEFT_PLAYER:
    {
        MSG_MOVE_LEFT_PLAYER& recvMSG = (MSG_MOVE_LEFT_PLAYER&)*pBuffer;
        wprintf_s(L"%d: MOVE_LEFT_PLAYER\n", recvMSG.id);

        MSG_MOVE_LEFT_PLAYER msgMoveLeft;
        msgMoveLeft.id = recvMSG.id;

        Send_Broadcast(NULL, (MSG_BASE*)&msgMoveLeft, sizeof(MSG_MOVE_LEFT_PLAYER));
        break;
    }
    case STOP_RIGHT_PLAYER:
    {
        MSG_STOP_RIGHT_PLAYER& recvMSG = (MSG_STOP_RIGHT_PLAYER&)*pBuffer;
        wprintf_s(L"%d: STOP_RIGHT_PLAYER\n", recvMSG.id);

        MSG_STOP_RIGHT_PLAYER msgStopRight;
        msgStopRight.id = recvMSG.id;

        Send_Broadcast(NULL, (MSG_BASE*)&msgStopRight, sizeof(MSG_STOP_RIGHT_PLAYER));
        break;
    }
    case STOP_LEFT_PLAYER:
    {
        MSG_STOP_LEFT_PLAYER& recvMSG = (MSG_STOP_LEFT_PLAYER&)*pBuffer;
        wprintf_s(L"%d: STOP_LEFT_PLAYER\n", recvMSG.id);

        MSG_STOP_LEFT_PLAYER msgStopLeft;
        msgStopLeft.id = recvMSG.id;

        Send_Broadcast(NULL, (MSG_BASE*)&msgStopLeft, sizeof(MSG_STOP_LEFT_PLAYER));
        break;
    }
    case DELETE_PLAYER:
    {
        MSG_DELETE_PLAYER& recvMSG = (MSG_DELETE_PLAYER&)*pBuffer;
        wprintf_s(L"%d: DELETE_PLAYER\n", recvMSG.id);

        MSG_DELETE_PLAYER msgDeletePlayer;
        msgDeletePlayer.id = recvMSG.id;

        Send_Broadcast(&m_arrSession[msgDeletePlayer.id], (MSG_BASE*)&msgDeletePlayer, sizeof(MSG_DELETE_PLAYER));
        break;
    }
    }
}

void CServer::Send_Message()
{
    fd_set writeSet;
    FD_ZERO(&writeSet);
    fd_set cpySet;

    //플레이어 세션들의 sendQ 사용여부를 확인하고 세트에 넣는다.
    for (int i = 0; i < m_iSessionCnt; i++)
    {
        if (m_arrSession[i].sendQ.GetUseSize() > 0)
            FD_SET(m_arrSession[i].clntSock, &writeSet);
    }

    TIMEVAL tTimeOut = { 0, 0 };

    cpySet = writeSet;

    int iResult = select(0, NULL, &cpySet, NULL, &tTimeOut);
    if (iResult > 0)
    {
        char Buffer[BUF_SIZE];
        for (u_int i = 0; i < writeSet.fd_count; i++)
        {
            if (FD_ISSET(writeSet.fd_array[i], &cpySet))
            {
                for (int j = 0; j < m_iSessionCnt; j++)
                {
                    if (m_arrSession[j].clntSock != writeSet.fd_array[j])
                        continue;

                    int iPeek{};
                    iPeek = m_arrSession[j].sendQ.Peek(Buffer, m_arrSession[j].sendQ.GetUseSize());

                    int iSend{};
                    iSend = send(m_arrSession[j].clntSock, Buffer, iPeek, 0);
                    if (iSend == SOCKET_ERROR)
                        wprintf_s(L"send() error:%d\n", WSAGetLastError());

                    m_arrSession[j].sendQ.MoveFront(iSend);
                }
            }
        }
    }
}
