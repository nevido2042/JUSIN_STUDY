#include <locale>
#include <iostream>
#include "CServer.h"
#include "PacketHandler.h"

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
	for (size_t i = 0; i < m_vecSession.size(); i++)
	{
		closesocket(m_vecSession[i]->clntSock);
		delete m_vecSession[i];
	}
	m_vecSession.clear();
	closesocket(m_ServSock);
	WSACleanup();
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
        //링버퍼 초기화
        pNewSession->recvQ = CRingBuffer(5000);
        pNewSession->sendQ = CRingBuffer(5000);

        CPacketHandler::mp_SC_CreateMyCharacter(&m_Packet,
            pNewSession->iID,
            pNewSession->iX,
            pNewSession->iY
        );
        Send_Unicast(pNewSession, (_byte*)m_Packet.GetBufferPtr(), m_Packet.GetDataSize());

        CPacketHandler::mp_SC_CreateOtherCharacter(&m_Packet,
            pNewSession->iID,
            pNewSession->iX,
            pNewSession->iY
        );
        Send_Broadcast(pNewSession, (_byte*)m_Packet.GetBufferPtr(), m_Packet.GetDataSize());


        //////헤더 작성
        //tagPACKET_HEADER tPacketHeader;

        ////기존 유저에게 신입 뿌리기
        //tagPACKET_SC_CREATE_OTHER_CHARACTER tSC_Create_Other_Character
        //{
        //    pNewSession->iID,
        //    pNewSession->iX,
        //    pNewSession->iY
        //};

        ////헤더 작성
        //tPacketHeader.BYTEbyCode = PACKET_CODE;
        //tPacketHeader.BYTEbySize = sizeof(tSC_Create_Other_Character);
        //tPacketHeader.BYTEbyType = PACKET_SC_CREATE_OTHER_CHARACTER;

        //Send_Broadcast(pNewSession, (_byte*)&tPacketHeader, sizeof(tPacketHeader));
        //Send_Broadcast(pNewSession, (_byte*)&tSC_Create_Other_Character, sizeof(tSC_Create_Other_Character));

        ////신입에게는 자신 포함한 모두를 생성하라하고
        //for (size_t i = 0; i < m_vecSession.size(); i++)
        //{
        //    //기존유저 정보 중 하나
        //    tagPACKET_SC_CREATE_OTHER_CHARACTER tSC_Create_Other_Character
        //    {
        //        m_vecSession[i]->iID,
        //        m_vecSession[i]->iX,
        //        m_vecSession[i]->iY
        //    };

        //    //헤더 작성
        //    tagPACKET_HEADER tPacketHeader;
        //    tPacketHeader.BYTEbyCode = PACKET_CODE;
        //    tPacketHeader.BYTEbySize = sizeof(tSC_Create_Other_Character);
        //    tPacketHeader.BYTEbyType = PACKET_SC_CREATE_OTHER_CHARACTER;

        //    Send_Unicast(pNewSession, (_byte*)&tPacketHeader, sizeof(tPacketHeader));
        //    Send_Unicast(pNewSession, (_byte*)&tSC_Create_Other_Character, sizeof(tSC_Create_Other_Character));
        //}

        //// 클라이언트에게 ID 전송
        //MSG_ALLOC_ID msgAllocID = { ALLOC_ID, m_iID };
        //Send_Unicast(m_vecSession.back(), (MSG_BASE*)&msgAllocID, sizeof(msgAllocID));

        ////기존 유저에게는 신입만 생성하도록
        //MSG_CREATE_PLAYER tMSG = { CREATE_PLAYER, m_iID, m_vecSession.back()->iX, m_vecSession.back()->iY };
        //Send_Broadcast(m_vecSession.back(), (MSG_BASE*)&tMSG, sizeof(tMSG));

        ////신입에게는 자신 포함한 모두를 생성하라하고
        //for (size_t i = 0; i < m_vecSession.size(); i++)
        //{
        //    tMSG.id = m_vecSession[i]->iID;
        //    tMSG.x = m_vecSession[i]->iX;
        //    tMSG.y = m_vecSession[i]->iY;
        //    Send_Unicast(m_vecSession.back(), (MSG_BASE*)&tMSG, sizeof(tMSG));
        //}
        m_vecSession.push_back(pNewSession);
        m_iID++;
    }
}

void CServer::Send_Unicast(SESSION* pSession, const _byte* pMSG, const int iSize)
{
    int iResult{ 0 };
    iResult = pSession->sendQ.Enqueue((_byte*)pMSG, iSize);
    if (iResult < iSize)
    {
        wprintf_s(L"sendQ.Enqueue() error\n");
        exit(1);
    }

    m_Packet.Clear();
}

void CServer::Send_Broadcast(SESSION* _pSession, const _byte* pMSG, const int iSize)
{
    int iResult{ 0 };

    for (SESSION* pSession : m_vecSession)
    {
        if (pSession == _pSession)
            continue;

        iResult = pSession->sendQ.Enqueue((_byte*)pMSG, iSize);
        if (iResult < iSize)
        {
            wprintf_s(L"sendQ.Enqueue() error\n");
            exit(1);
        }
    }

    m_Packet.Clear();
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
        _byte Buffer[BUF_SIZE];
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

    while (1)
    {
        //메시지 분석
        tagPACKET_HEADER tHeader;

        //헤더도 못 뽑는 정도 밖에 안들어왔다면
        if (sizeof(tHeader) > _pSession->recvQ.GetUseSize())
        {
            break;
        }

        int retPeek = _pSession->recvQ.Peek((_byte*)&tHeader, sizeof(tHeader));
        if (retPeek != sizeof(tHeader))
        {
            wprintf_s(L"Peek() Error:%d\n", retPeek);
            exit(1);
        }
        if (tHeader.BYTEbyCode != PACKET_CODE)
        {
            wprintf_s(L"BYTEbyCode Error:%d\n", tHeader.BYTEbyCode);
            exit(1);
        }
        if (tHeader.BYTEbySize + sizeof(tHeader) > _pSession->recvQ.GetUseSize())
        {
            break;
        }

        _pSession->recvQ.MoveFront(sizeof(tHeader));
        Decode_Message(tHeader, _pSession);
    }
}

void CServer::Decode_Message(const tagPACKET_HEADER& _Header, SESSION* _pSession)
{
    switch (_Header.BYTEbyType)
    {
    case PACKET_CS_DELETE_MY_CHARACTER:
    {
        CPacketHandler::mp_SC_DeleteCharacter(&m_Packet, _pSession->iID);
        wprintf_s(L"ID: %d, 캐릭터 삭제\n", _pSession->iID);

        Send_Broadcast(_pSession, (_byte*)m_Packet.GetBufferPtr(), m_Packet.GetDataSize() - sizeof(tagPACKET_HEADER));

        break;
    }
    }

    //switch (iType)
    //{
    //case MOVE_RIGHT_PLAYER:
    //{
    //    MSG_MOVE_RIGHT_PLAYER& recvMSG = (MSG_MOVE_RIGHT_PLAYER&)*pMsg;
    //    wprintf_s(L"%d: MOVE_RIGHT_PLAYER\n", recvMSG.id);

    //    MSG_MOVE_RIGHT_PLAYER msgMoveRight;
    //    msgMoveRight.id = recvMSG.id;

    //    Send_Broadcast(NULL, (_byte*)&msgMoveRight, sizeof(MSG_MOVE_RIGHT_PLAYER));
    //    break;
    //}
    //case MOVE_LEFT_PLAYER:
    //{
    //    MSG_MOVE_LEFT_PLAYER& recvMSG = (MSG_MOVE_LEFT_PLAYER&)*pMsg;
    //    wprintf_s(L"%d: MOVE_LEFT_PLAYER\n", recvMSG.id);

    //    MSG_MOVE_LEFT_PLAYER msgMoveLeft;
    //    msgMoveLeft.id = recvMSG.id;

    //    Send_Broadcast(NULL, (_byte*)&msgMoveLeft, sizeof(MSG_MOVE_LEFT_PLAYER));
    //    break;
    //}
    //case STOP_RIGHT_PLAYER:
    //{
    //    MSG_STOP_RIGHT_PLAYER& recvMSG = (MSG_STOP_RIGHT_PLAYER&)*pMsg;
    //    wprintf_s(L"%d: STOP_RIGHT_PLAYER\n", recvMSG.id);

    //    MSG_STOP_RIGHT_PLAYER msgStopRight;
    //    msgStopRight.id = recvMSG.id;

    //    Send_Broadcast(NULL, (_byte*)&msgStopRight, sizeof(MSG_STOP_RIGHT_PLAYER));
    //    break;
    //}
    //case STOP_LEFT_PLAYER:
    //{
    //    MSG_STOP_LEFT_PLAYER& recvMSG = (MSG_STOP_LEFT_PLAYER&)*pMsg;
    //    wprintf_s(L"%d: STOP_LEFT_PLAYER\n", recvMSG.id);

    //    MSG_STOP_LEFT_PLAYER msgStopLeft;
    //    msgStopLeft.id = recvMSG.id;

    //    Send_Broadcast(NULL, (_byte*)&msgStopLeft, sizeof(MSG_STOP_LEFT_PLAYER));
    //    break;
    //}
    //case DELETE_PLAYER:
    //{
    //    //MSG_DELETE_PLAYER& recvMSG = (MSG_DELETE_PLAYER&)*pMsg;
    //    //wprintf_s(L"%d: DELETE_PLAYER\n", recvMSG.id);

    //    //MSG_DELETE_PLAYER msgDeletePlayer;
    //    //msgDeletePlayer.id = recvMSG.id;
    //    //
    //    ////세션중 아이디가 같은 녀석의 세션을 제외하고 보내려했는데 그냥 보내볼까

    //    //Send_Broadcast(NULL/*&m_vecSession[msgDeletePlayer.id]*/, (_byte*)&msgDeletePlayer, sizeof(MSG_DELETE_PLAYER));
    //    break;
    //}
    //}
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
                        _byte buffer[1];
                        int result = recv(currentSock, buffer, sizeof(buffer), MSG_PEEK);
                        if (result <= 0) // 연결 종료되었거나 오류 발생
                        {
                            /*MSG_DELETE_PLAYER tMSG;
                            tMSG.type = DELETE_PLAYER;
                            tMSG.id = (*it)->iID;
                            Send_Broadcast(NULL, (_byte*)&tMSG, sizeof(MSG_BASE));*/

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
        _byte Buffer[BUF_SIZE];
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
                        wprintf_s(L"ID:%d, send() error:%d\n", m_vecSession[j]->iID, WSAGetLastError());

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

    ////브로드 캐스트 캐릭터 삭제
    //MSG_DELETE_PLAYER tMsg;
    //tMsg.id = _pSession->iID;
    //tMsg.type = DELETE_PLAYER;
    //wprintf_s(L"id:%d Disconnect\n", _pSession->iID);
    //Send_Broadcast(_pSession, (_byte*)&tMsg, sizeof(MSG_BASE));

    //세션 정리
    for (auto iter = m_vecSession.begin(); iter != m_vecSession.end();)
    {
        if ((*iter)->iID == _pSession->iID)
        {
            iter = m_vecSession.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

