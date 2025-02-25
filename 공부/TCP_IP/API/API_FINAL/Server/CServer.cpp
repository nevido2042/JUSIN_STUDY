#include <locale>
#include <iostream>
#include "CServer.h"

CServer::CServer()
    :m_ServSock(INVALID_SOCKET),
    //m_iSessionCnt(0),
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
    Recieve_Message();
    // 메시지 전송 처리
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

        int iRandX = (WINCX / 10) + rand() % (WINCX / 2);
        int iRandY = (WINCY / 10) + rand() % (WINCY / 2);

        SESSION* pNewSession = new SESSION(clntAdr, clntSock, m_iID, iRandX, iRandY);
        m_vecSession.push_back(pNewSession);
        //링버퍼 초기화
        m_vecSession.back()->recvQ = CRingBuffer(5000);
        m_vecSession.back()->sendQ = CRingBuffer(5000);

        // 클라이언트에게 ID 전송
        MSG_ALLOC_ID msgAllocID = { ALLOC_ID, m_iID };
        Send_Unicast(m_vecSession.back(), (MSG_BASE*)&msgAllocID, sizeof(msgAllocID));

        //기존 유저에게는 신입만 생성하도록
        MSG_CREATE_PLAYER tMSG = { CREATE_PLAYER, m_iID, m_vecSession.back()->x, m_vecSession.back()->y };
        Send_Broadcast(m_vecSession.back(), (MSG_BASE*)&tMSG, sizeof(tMSG));

        //신입에게는 자신 포함한 모두를 생성하라하고
        for (size_t i = 0; i < m_vecSession.size(); i++)
        {
            tMSG.id = m_vecSession[i]->id;
            tMSG.x = m_vecSession[i]->x;
            tMSG.y = m_vecSession[i]->y;
            Send_Unicast(m_vecSession.back(), (MSG_BASE*)&tMSG, sizeof(tMSG));
        }

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

void CServer::Send_Broadcast(SESSION* _pSession, const MSG_BASE* pMSG, const int iSize)
{
    int iResult{ 0 };

    for (SESSION* pSession : m_vecSession)
    {
        if (pSession == _pSession)
            continue;

        iResult = pSession->sendQ.Enqueue((char*)pMSG, iSize);
        if (iResult < iSize)
        {
            wprintf_s(L"sendQ.Enqueue() error\n");
            exit(1);
        }
    }

}

void CServer::Read_Proc(SESSION* _pSession)
{
    //recvQ에 다이렉트로 꽂는다.
    if (_pSession->recvQ.DirectEnqueueSize() > 0)
    {
        int retRecv = recv(_pSession->clntSock, _pSession->recvQ.GetRearBufferPtr() + 1, _pSession->recvQ.DirectEnqueueSize(), 0);

        if (retRecv == 0)
        {
            Delete_Session(_pSession);
            return;
        }
        if (retRecv == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAEWOULDBLOCK)
                return;

            Delete_Session(_pSession);
            wprintf_s(L"recv() Error:%d(Line:%d)\n", WSAGetLastError(), __LINE__);
            return;
        }

        _pSession->recvQ.MoveRear(retRecv);
    }
    else//버퍼를 통해 리시브큐로 꽂는다.
    {
        char Buffer[BUF_SIZE];
        int iResult = recv(_pSession->clntSock, Buffer, sizeof(Buffer), 0);
        int iErrCode = WSAGetLastError();

        if (iResult == 0 || (iResult == SOCKET_ERROR && iErrCode != WSAEWOULDBLOCK))
        {
            if (iErrCode)
            {
                wprintf_s(L"recv() error: %d\n", iErrCode);
            }

            // 소켓 정리
            FD_CLR(_pSession->clntSock, &m_tReadSet);
            closesocket(_pSession->clntSock);
        }

        _pSession->recvQ.Enqueue(Buffer, iResult);
    }

    while (true)
    {
        if (_pSession->recvQ.GetUseSize() < MSG_SIZE)
            break;//중단

        char Msg[MSG_SIZE];

        int iResult{ 0 };
        iResult = _pSession->recvQ.Dequeue(Msg, MSG_SIZE);
        if (iResult != MSG_SIZE)
        {
            //결함
            exit(1);
        }

        int iType{ 0 };
        if (iResult > 0)
        {
            memcpy(&iType, Msg, sizeof(int));
            Decode_Message(iType, Msg);
        }
    }
}

void CServer::Decode_Message(int iType, char* pMsg)
{
    switch (iType)
    {
    case MOVE_RIGHT_PLAYER:
    {
        MSG_MOVE_RIGHT_PLAYER& recvMSG = (MSG_MOVE_RIGHT_PLAYER&)*pMsg;
        wprintf_s(L"%d: MOVE_RIGHT_PLAYER\n", recvMSG.id);

        MSG_MOVE_RIGHT_PLAYER msgMoveRight;
        msgMoveRight.id = recvMSG.id;

        Send_Broadcast(NULL, (MSG_BASE*)&msgMoveRight, sizeof(MSG_MOVE_RIGHT_PLAYER));
        break;
    }
    case MOVE_LEFT_PLAYER:
    {
        MSG_MOVE_LEFT_PLAYER& recvMSG = (MSG_MOVE_LEFT_PLAYER&)*pMsg;
        wprintf_s(L"%d: MOVE_LEFT_PLAYER\n", recvMSG.id);

        MSG_MOVE_LEFT_PLAYER msgMoveLeft;
        msgMoveLeft.id = recvMSG.id;

        Send_Broadcast(NULL, (MSG_BASE*)&msgMoveLeft, sizeof(MSG_MOVE_LEFT_PLAYER));
        break;
    }
    case STOP_RIGHT_PLAYER:
    {
        MSG_STOP_RIGHT_PLAYER& recvMSG = (MSG_STOP_RIGHT_PLAYER&)*pMsg;
        wprintf_s(L"%d: STOP_RIGHT_PLAYER\n", recvMSG.id);

        MSG_STOP_RIGHT_PLAYER msgStopRight;
        msgStopRight.id = recvMSG.id;

        Send_Broadcast(NULL, (MSG_BASE*)&msgStopRight, sizeof(MSG_STOP_RIGHT_PLAYER));
        break;
    }
    case STOP_LEFT_PLAYER:
    {
        MSG_STOP_LEFT_PLAYER& recvMSG = (MSG_STOP_LEFT_PLAYER&)*pMsg;
        wprintf_s(L"%d: STOP_LEFT_PLAYER\n", recvMSG.id);

        MSG_STOP_LEFT_PLAYER msgStopLeft;
        msgStopLeft.id = recvMSG.id;

        Send_Broadcast(NULL, (MSG_BASE*)&msgStopLeft, sizeof(MSG_STOP_LEFT_PLAYER));
        break;
    }
    case DELETE_PLAYER:
    {
        MSG_DELETE_PLAYER& recvMSG = (MSG_DELETE_PLAYER&)*pMsg;
        wprintf_s(L"%d: DELETE_PLAYER\n", recvMSG.id);

        MSG_DELETE_PLAYER msgDeletePlayer;
        msgDeletePlayer.id = recvMSG.id;
        
        //세션중 아이디가 같은 녀석의 세션을 제외하고 보내려했는데 그냥 보내볼까

        Send_Broadcast(NULL/*&m_vecSession[msgDeletePlayer.id]*/, (MSG_BASE*)&msgDeletePlayer, sizeof(MSG_DELETE_PLAYER));
        break;
    }
    }
}

void CServer::Recieve_Message()
{
    fd_set tCpySet;
    FD_ZERO(&m_tReadSet);
    FD_SET(m_ServSock, &m_tReadSet);

    // 타임아웃 설정 (0초, 0마이크로초)
    TIMEVAL tTimeOut = { 0, 0 };

    // 클라이언트 소켓을 읽기 집합에 추가

    for (SESSION* pSession : m_vecSession)
    {
        FD_SET(pSession->clntSock, &m_tReadSet);
    }

    // 소켓 상태를 확인하기 위한 복사본
    tCpySet = m_tReadSet;

    // select() 호출
    int iResult = select(0, &tCpySet, NULL, NULL, &tTimeOut);
    if (iResult == SOCKET_ERROR)
    {
        wprintf_s(L"select() error:%d\n", WSAGetLastError());
        return;  // 오류가 발생하면 종료
    }

    // select()가 성공적으로 완료된 경우
    if (iResult > 0)
    {
        // 선택된 소켓들을 순차적으로 처리
        for (u_int i = 0; i < tCpySet.fd_count; i++)
        {
            SOCKET currentSock = tCpySet.fd_array[i];

            // 서버 소켓(새로운 클라이언트 연결) 처리
            if (currentSock == m_ServSock)
            {
                AcceptProc(); // 클라이언트 연결 처리
            }
            else
            {
                for (auto it = m_vecSession.begin(); it != m_vecSession.end(); )
                {
                    if ((*it)->clntSock == currentSock)
                    {
                        // 데이터 수신 전 소켓 상태 확인
                        char buffer[1];
                        int result = recv(currentSock, buffer, sizeof(buffer), MSG_PEEK);
                        if (result <= 0) // 연결 종료되었거나 오류 발생
                        {
                            MSG_DELETE_PLAYER tMSG;
                            tMSG.type = DELETE_PLAYER;
                            tMSG.id = (*it)->id;
                            Send_Broadcast(NULL, (MSG_BASE*)&tMSG, sizeof(MSG_BASE));

                            // 클라이언트 소켓 종료 처리
                            closesocket((*it)->clntSock);

                            // 세션 제거
                            it = m_vecSession.erase(it); // erase는 삭제된 다음 요소의 iterator를 반환함
                        }
                        else
                        {
                            Read_Proc(*it);
                            ++it;
                        }
                    }
                    else
                    {
                        ++it;
                    }
                }
            }
        }
    }
}

void CServer::Send_Message()
{
    fd_set writeSet;
    FD_ZERO(&writeSet);
    fd_set cpySet;

    //플레이어 세션들의 sendQ 사용여부를 확인하고 세트에 넣는다.
    for (size_t i = 0; i < m_vecSession.size(); i++)
    {
        if (m_vecSession[i]->sendQ.GetUseSize() > 0)
            FD_SET(m_vecSession[i]->clntSock, &writeSet);
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
                for (size_t j = 0; j < m_vecSession.size(); j++)
                {
                    if (m_vecSession[j]->clntSock != writeSet.fd_array[j])
                        continue;

                    int iPeek{};
                    iPeek = m_vecSession[j]->sendQ.Peek(Buffer, m_vecSession[j]->sendQ.GetUseSize());

                    int iSend{};
                    iSend = send(m_vecSession[j]->clntSock, Buffer, iPeek, 0);
                    if (iSend == SOCKET_ERROR)
                        wprintf_s(L"send() error:%d\n", WSAGetLastError());

                    m_vecSession[j]->sendQ.MoveFront(iSend);
                }
            }
        }
    }
}

void CServer::Delete_Session(SESSION* _pSession)
{
    //클로즈 소켓
    closesocket(_pSession->clntSock);

    //브로드 캐스트 캐릭터 삭제
    MSG_DELETE_PLAYER tMsg;
    tMsg.id = _pSession->id;
    tMsg.type = DELETE_PLAYER;
    wprintf_s(L"id:%d Disconnect\n", _pSession->id);
    Send_Broadcast(_pSession, (MSG_BASE*)&tMsg, sizeof(MSG_BASE));

    //세션 정리
    for (auto iter = m_vecSession.begin(); iter != m_vecSession.end();)
    {
        if ((*iter)->id == _pSession->id)
        {
            iter = m_vecSession.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}
