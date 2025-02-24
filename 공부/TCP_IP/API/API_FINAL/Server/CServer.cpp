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

    // 서버 소켓 생성
    m_ServSock = socket(AF_INET, SOCK_STREAM, 0);
    if (m_ServSock == INVALID_SOCKET)
    {
        wprintf_s(L"socket() error:%d\n", WSAGetLastError());
        return false;
    }

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

    // 리슨
    if (listen(m_ServSock, 5) == SOCKET_ERROR)
    {
        wprintf_s(L"listen() error:%d\n", WSAGetLastError());
        return false;
    }

    cout << "서버 실행 성공" << endl;
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
    if (iResult == SOCKET_ERROR) {
        wprintf_s(L"select() error:%d\n", WSAGetLastError());
        return false;
    }

    if (iResult > 0)
    {
        for (int i = 0; i < m_tReadSet.fd_count; i++)
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

void CServer::Send_Unicast(const SESSION* pSession, const MSG_BASE* pMSG, const int iSize)
{
    int iResult = send(pSession->clntSock, (char*)pMSG, iSize, 0);
    if (iResult == SOCKET_ERROR)
    {
        wprintf_s(L"send() error:%d\n", WSAGetLastError());
    }
}

void CServer::Send_Broadcast(const SESSION* pSession, const MSG_BASE* pMSG, const int iSize)
{
    for (int i = 0; i < m_iSessionCnt; i++)
    {
        if (pSession == &m_arrSession[i]) continue;
        int iResult = send(m_arrSession[i].clntSock, (char*)pMSG, iSize, 0);
        if (iResult == SOCKET_ERROR)
        {
            wprintf_s(L"send() error:%d\n", WSAGetLastError());
        }
    }
}

void CServer::Read_Proc(const SESSION* pSession)
{
    char szMSG[16];

    int iResult = recv(pSession->clntSock, szMSG, sizeof(szMSG), 0);
    int iErrCode = WSAGetLastError();

    if (iResult == 0 || (iResult == SOCKET_ERROR && iErrCode != WSAEWOULDBLOCK))
    {
        wprintf_s(L"recv() error: %d\n", iErrCode);
        closesocket(pSession->clntSock);

        for (int i = 0; i < m_iSessionCnt; i++)
        {
            if (m_arrSession[i].clntSock == pSession->clntSock)
            {
                m_arrSession[i] = m_arrSession[m_iSessionCnt - 1];
                m_iSessionCnt--;
                break;
            }
        }

        FD_CLR(pSession->clntSock, &m_tReadSet);
        return;
    }

    int iType{ 0 };
    if (iResult > 0)
    {
        memcpy(&iType, szMSG, sizeof(int));
    }

    switch (iType)
    {
    case MOVE_RIGHT_PLAYER:
    {
        MSG_MOVE_RIGHT_PLAYER& recvMSG = (MSG_MOVE_RIGHT_PLAYER&)szMSG;

        MSG_MOVE_RIGHT_PLAYER msgMoveRight;
        msgMoveRight.id = recvMSG.id;

        Send_Broadcast(NULL, (MSG_BASE*)&msgMoveRight, sizeof(MSG_MOVE_RIGHT_PLAYER));
        break;
    }
    case MOVE_LEFT_PLAYER:
    {
        MSG_MOVE_LEFT_PLAYER& recvMSG = (MSG_MOVE_LEFT_PLAYER&)szMSG;

        MSG_MOVE_LEFT_PLAYER msgMoveLeft;
        msgMoveLeft.id = recvMSG.id;

        Send_Broadcast(NULL, (MSG_BASE*)&msgMoveLeft, sizeof(MSG_MOVE_LEFT_PLAYER));
        break;
    }
    case STOP_RIGHT_PLAYER:
    {
        MSG_STOP_RIGHT_PLAYER& recvMSG = (MSG_STOP_RIGHT_PLAYER&)szMSG;

        MSG_STOP_RIGHT_PLAYER msgStopRight;
        msgStopRight.id = recvMSG.id;

        Send_Broadcast(NULL, (MSG_BASE*)&msgStopRight, sizeof(MSG_STOP_RIGHT_PLAYER));
        break;
    }
    case STOP_LEFT_PLAYER:
    {
        MSG_STOP_LEFT_PLAYER& recvMSG = (MSG_STOP_LEFT_PLAYER&)szMSG;

        MSG_STOP_LEFT_PLAYER msgStopLeft;
        msgStopLeft.id = recvMSG.id;

        Send_Broadcast(NULL, (MSG_BASE*)&msgStopLeft, sizeof(MSG_STOP_LEFT_PLAYER));
        break;
    }
    case DELETE_PLAYER:
    {
        MSG_DELETE_PLAYER& recvMSG = (MSG_DELETE_PLAYER&)szMSG;
        cout << "Delete Player ID:" << recvMSG.id << endl;

        MSG_DELETE_PLAYER msgDeletePlayer;
        msgDeletePlayer.id = recvMSG.id;

        Send_Broadcast(&m_arrSession[msgDeletePlayer.id], (MSG_BASE*)&msgDeletePlayer, sizeof(MSG_DELETE_PLAYER));
        break;
    }
    }
}
