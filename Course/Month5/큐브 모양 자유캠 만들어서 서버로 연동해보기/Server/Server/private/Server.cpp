#include "PacketHandler.h"
#include "Server.h"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>


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
    m_iPort = Load_Config_File(TEXT("config.txt"));

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
    servAdr.sin_port = htons(m_iPort);

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
		closesocket(m_vecSession[i]->Get_Socket());
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

        int iRandX{ rand() % 3 };
        int iRandZ{ rand() % 3 };

        _float3 Position{ static_cast<_float>(iRandX) , 0.f, static_cast<_float>(iRandZ) };

        CSession* pNewSession = new CSession(clntAdr, clntSock, m_iID, Position);
        //링버퍼 초기화
        /*pNewSession->m_RecvQ = CRingBuffer(5000);
        pNewSession->m_SendQ = CRingBuffer(5000);*/

        //신입 자기 캐릭 생성
        CPacketHandler::mp_SC_CreateMyCharacter(&m_Packet,
            pNewSession->Get_SessionInfo().iID,
            pNewSession->Get_SessionInfo().Position
        );
        Send_Unicast(pNewSession, (_byte*)m_Packet.Get_BufferPtr(), m_Packet.Get_DataSize());

        //신입 뿌리기
        CPacketHandler::mp_SC_CreateOtherCharacter(&m_Packet,
            pNewSession->Get_SessionInfo().iID,
            pNewSession->Get_SessionInfo().Position
        );
        Send_Broadcast(pNewSession, (_byte*)m_Packet.Get_BufferPtr(), m_Packet.Get_DataSize());

        //신입에게 기존 유저 뿌리기
        for (CSession* pSession : m_vecSession)
        {
            //신입 뿌리기
            CPacketHandler::mp_SC_CreateOtherCharacter(&m_Packet,
                pSession->Get_SessionInfo().iID,
                pSession->Get_SessionInfo().Position
            );
            Send_Unicast(pNewSession, (_byte*)m_Packet.Get_BufferPtr(), m_Packet.Get_DataSize());
        }

        m_vecSession.push_back(pNewSession);
        m_iID++;
    }
}

void CServer::Send_Unicast(CSession* pSession, const _byte* pMSG, const int iSize)
{
    int iResult{ 0 };
    iResult = pSession->Get_SendQ().Enqueue((_byte*)pMSG, iSize);
    if (iResult < iSize)
    {
        wprintf_s(L"sendQ.Enqueue() error\n");
        exit(1);
    }

    m_Packet.Clear();
}

void CServer::Send_Broadcast(CSession* _pSession, const _byte* pMSG, const int iSize)
{
    int iResult{ 0 };

    for (CSession* pSession : m_vecSession)
    {
        if (pSession == _pSession)
            continue;

        iResult = pSession->Get_SendQ().Enqueue((_byte*)pMSG, iSize);
        if (iResult < iSize)
        {
            wprintf_s(L"sendQ.Enqueue() error\n");
            exit(1);
        }
    }

    m_Packet.Clear();
}

void CServer::Read_Proc(CSession* _pSession)
{
    //recvQ에 다이렉트로 꽂는다.
    if (_pSession->Get_RecvQ().DirectEnqueueSize() > 0)
    {
        int retRecv = recv(_pSession->Get_Socket(), (char*)_pSession->Get_RecvQ().Get_Rear() + 1, _pSession->Get_RecvQ().DirectEnqueueSize(), 0);

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

        _pSession->Get_RecvQ().Move_Rear(retRecv);
    }
    else//버퍼를 통해 리시브큐로 꽂는다.
    {
        _byte Buffer[BUF_SIZE];
        int iResult = recv(_pSession->Get_Socket(), (char*)Buffer, sizeof(Buffer), 0);
        int iErrCode = WSAGetLastError();

        if (iResult == 0 || (iResult == SOCKET_ERROR && iErrCode != WSAEWOULDBLOCK))
        {
            if (iErrCode)
            {
                wprintf_s(L"recv() error: %d\n", iErrCode);
            }

            // 소켓 정리
            FD_CLR(_pSession->Get_Socket(), &m_tReadSet);
            closesocket(_pSession->Get_Socket());
        }

        _pSession->Get_RecvQ().Enqueue(Buffer, iResult);
    }

    while (1)
    {
        //메시지 분석
        tagPACKET_HEADER tHeader;

        //헤더도 못 뽑는 정도 밖에 안들어왔다면
        if (sizeof(tHeader) > _pSession->Get_RecvQ().Get_UseSize())
        {
            break;
        }

        int retPeek = _pSession->Get_RecvQ().Peek((_byte*)&tHeader, sizeof(tHeader));
        if (retPeek != sizeof(tHeader))
        {
            wprintf_s(L"Peek() Error:%d\n", retPeek);
            exit(1);
        }
        if (tHeader.byCode != PACKET_CODE)
        {
            wprintf_s(L"BYTEbyCode Error:%d\n", tHeader.byCode);
            exit(1);
        }
        if (tHeader.bySize + sizeof(tHeader) > (size_t)_pSession->Get_RecvQ().Get_UseSize())
        {
            break;
        }

        _pSession->Get_RecvQ().Move_Front(sizeof(tHeader));
        Decode_Message(tHeader, _pSession);
    }
}

void CServer::Decode_Message(const tagPACKET_HEADER& _Header, CSession* _pSession)
{
    //_pSession->Recive((_byte*)m_Packet.GetBufferPtr(), _Header.bySize);
    int iResult = _pSession->Receive_Data(m_Packet, _Header.bySize);//_pSession->m_RecvQ.Dequeue((char*)m_Packet.GetBufferPtr(), _Header.bySize);
    if (iResult != _Header.bySize)
    {
        wprintf_s(L"Dequeue() Error:%d\n", iResult);
        exit(1);
    }
    m_Packet.Move_WritePos(iResult);

    switch (_Header.byType)
    {
    case PACKET_CS_DELETE_MY_CHARACTER:
    {
        /*CPacketHandler::SC_DeleteCharacter(&m_Packet, _pSession->Get_SessionInfo().iID);

        wprintf_s(L"ID: %d, 캐릭터 삭제\n", _pSession->Get_SessionInfo().iID);

        Send_Broadcast(_pSession, (_byte*)m_Packet.GetBufferPtr(), m_Packet.GetDataSize());

        break;*/
    }
    case PACKET_CS_MOVE_START:
    {
        //wprintf_s(L"PACKET_CS_MOVE_START\n");

        _float3 MoveStartPos;
        _float3 MoveDir;
        CPacketHandler::net_Move_Start(&m_Packet, MoveStartPos, MoveDir);

        wprintf_s(L"Move_Dir:(%.2f, %.2f, %.2f)\n", MoveDir.x, MoveDir.y, MoveDir.z);

        CPacketHandler::mp_SC_Move_Start(&m_Packet, MoveStartPos, MoveDir, _pSession->Get_SessionInfo().iID);
        Send_Broadcast(_pSession, (_byte*)m_Packet.Get_BufferPtr(), m_Packet.Get_DataSize());

        break;
    }
    case PACKET_CS_MOVE_STOP:
    {

        //wprintf_s(L"PACKET_CS_MOVE_START\n");

        _float3 MoveStopPos;
        CPacketHandler::net_Move_Stop(&m_Packet, MoveStopPos);

        _pSession->Get_SessionInfo().Position = MoveStopPos;

        wprintf_s(L"MoveStopPos:(%.2f, %.2f, %.2f)\n", MoveStopPos.x, MoveStopPos.y, MoveStopPos.z);

        CPacketHandler::mp_SC_Move_Stop(&m_Packet, MoveStopPos, _pSession->Get_SessionInfo().iID);
        Send_Broadcast(_pSession, (_byte*)m_Packet.Get_BufferPtr(), m_Packet.Get_DataSize());

        break;
    }
    case PACKET_CS_POSITION:
    {
        _float3 Pos;

        CPacketHandler::net_Position(&m_Packet, Pos);

        _pSession->Get_SessionInfo().Position = Pos;
        wprintf_s(L"Pos:(%.2f, %.2f, %.2f)\n", Pos.x, Pos.y, Pos.z);

		CPacketHandler::mp_SC_Position(&m_Packet, Pos, _pSession->Get_SessionInfo().iID);

        Send_Broadcast(_pSession, (_byte*)m_Packet.Get_BufferPtr(), m_Packet.Get_DataSize());

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

    for (CSession* pSession : m_vecSession)
    {
        FD_SET(pSession->Get_Socket(), &m_tReadSet);
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
                for (auto iter = m_vecSession.begin(); iter != m_vecSession.end(); )
                {
                    if ((*iter)->Get_Socket() == currentSock)
                    {
                        // 데이터 수신 전 소켓 상태 확인
                        _byte buffer[1];
                        int result = recv(currentSock, (char*)buffer, sizeof(buffer), MSG_PEEK);
                        if (result <= 0) // 연결 종료되었거나 오류 발생
                        {
                            wprintf_s(L"클라이언트 종료 ID:%d, WSAGetLastError:%d\n", (*iter)->Get_SessionInfo().iID, WSAGetLastError());
                            CPacketHandler::mp_SC_DeleteCharacter(&m_Packet, (*iter)->Get_SessionInfo().iID);
                            Send_Broadcast(*iter, m_Packet.Get_BufferPtr(), m_Packet.Get_DataSize());

                            // 클라이언트 소켓 종료 처리
                            closesocket((*iter)->Get_Socket());
                            Safe_Delete(*iter);
                            // 세션 제거
                            iter = m_vecSession.erase(iter); // erase는 삭제된 다음 요소의 iterator를 반환함
                        }
                        else
                        {
                            Read_Proc(*iter);
                            ++iter;
                        }
                    }
                    else
                    {
                        ++iter;
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
        if (m_vecSession[i]->Get_SendQ().Get_UseSize() > 0)
            FD_SET(m_vecSession[i]->Get_Socket(), &writeSet);
    }

    TIMEVAL tTimeOut = { 0, 0 };

    cpySet = writeSet;

    int iResult = select(0, NULL, &cpySet, NULL, &tTimeOut);
    if (iResult > 0)
    {
        _byte Buffer[BUF_SIZE];

        for (u_int i = 0; i < writeSet.fd_count; i++)
        {
            SOCKET writeSock = writeSet.fd_array[i]; // 현재 write 가능 소켓

            // 벡터에서 해당 소켓을 가진 세션을 찾기
            for (size_t j = 0; j < m_vecSession.size(); j++)
            {
                if (m_vecSession[j]->Get_Socket() == writeSock)
                {
                    // 현재 세션의 큐에서 보낼 데이터 가져오기
                    int iPeek = m_vecSession[j]->Get_SendQ().Peek(Buffer, m_vecSession[j]->Get_SendQ().Get_UseSize());
                    if (iPeek <= 0) // 보낼 데이터가 없음
                        continue;

                    // 데이터 전송
                    int iSend = send(writeSock, (char*)Buffer, iPeek, 0);
                    if (iSend == SOCKET_ERROR)
                    {
                        wprintf_s(L"ID:%d, send() error:%d\n", m_vecSession[j]->Get_SessionInfo().iID, WSAGetLastError());
                        continue;
                    }

                    // 전송된 만큼 큐의 데이터를 이동
                    m_vecSession[j]->Get_SendQ().Move_Front(iSend);
                    // wprintf_s(L"To IndexSock: %d, Send Byte: %d\n", i, iSend);

                    break; // 해당 writeSock에 대한 처리가 끝났으므로 내부 루프 종료
                }
            }
        }
    }

}

void CServer::Delete_Session(CSession* _pSession)
{
    //클로즈 소켓
    closesocket(_pSession->Get_Socket());

    ////브로드 캐스트 캐릭터 삭제
    //MSG_DELETE_PLAYER tMsg;
    //tMsg.id = _pSession->iID;
    //tMsg.type = DELETE_PLAYER;
    //wprintf_s(L"id:%d Disconnect\n", _pSession->iID);
    //Send_Broadcast(_pSession, (_byte*)&tMsg, sizeof(MSG_BASE));

    //세션 정리
    for (auto iter = m_vecSession.begin(); iter != m_vecSession.end();)
    {
        if ((*iter)->Get_SessionInfo().iID == _pSession->Get_SessionInfo().iID)
        {
            iter = m_vecSession.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

int CServer::Load_Config_File(const wstring& filename)
{
    wifstream inFile(filename);
    if (!inFile.is_open())
    {
        cout << "Can't Open ServerConfigFile" << endl;
        return 2042;
    }

    // 파일의 첫 줄에서 포트 번호 읽기
    wstring PortLine;
    getline(inFile, PortLine);
    wstringstream wss(PortLine);
    int iPort;
    wss >> iPort;
    return iPort;
}
