#include "Server.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#include "PacketType.h"


CServer::CServer()
{
    ZeroMemory(&m_tReadSet, sizeof(fd_set));
}

CServer::~CServer()
{
}

_bool CServer::Initialize()
{
    Define_Packets();

    m_iPort = Load_Config_File(TEXT("../bin/config.txt"));

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
    if (::bind(m_ServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
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

_bool CServer::Update()
{
    // ESC와 S가 동시에 눌렸을 때만 종료
    if ((GetAsyncKeyState(VK_ESCAPE) & 0x8000) &&
        (GetAsyncKeyState('S') & 0x8000))
    {
        return false;
    }

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

_bool CServer::Network()
{
    Recieve_Message();
    Send_Message();
    Delete_Dead_Session();

    return true;
}


void CServer::AcceptProc()
{
    while (true)
    {
        SOCKADDR_IN clntAdr;
        _int iAdrSize = sizeof(clntAdr);
        SOCKET clntSock = accept(m_ServSock, (SOCKADDR*)&clntAdr, &iAdrSize);

        if (clntSock == INVALID_SOCKET)
        {
            _int iErrCode = WSAGetLastError();
            if (iErrCode == WSAEWOULDBLOCK)
                break;

            wprintf_s(L"accept() error: %d\n", iErrCode);
            return;
        }

        cout << "ID:" << m_iID << " 클라이언트 접속" << endl;

        CSession* pNewSession = new CSession(clntAdr, clntSock, m_iID, _float3{ 0.f, 0.f, 0.f });

        //아이디 부여
		PACKET_DESC Desc{};
        Desc.iID = m_iID;
        Send_Packet_Unicast(pNewSession, ENUM_CLASS(PacketType::SC_GIVE_ID), &Desc);

        m_vecSession.push_back(pNewSession);
        m_iID++;
    }
}

void CServer::Read_Proc(CSession* _pSession)
{
    //recvQ에 다이렉트로 꽂는다.
    if (_pSession->Get_RecvQ().DirectEnqueueSize() > 0)
    {
        _int retRecv = recv(_pSession->Get_Socket(), (_char*)_pSession->Get_RecvQ().Get_Rear() + 1, _pSession->Get_RecvQ().DirectEnqueueSize(), 0);

        if (retRecv == 0)
        {
            Set_Session_Dead(_pSession);
            return;
        }
        if (retRecv == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAEWOULDBLOCK)
                return;

            Set_Session_Dead(_pSession);
            wprintf_s(L"recv() Error:%d(Line:%d)\n", WSAGetLastError(), __LINE__);
            return;
        }

        _pSession->Get_RecvQ().Move_Rear(retRecv);
    }
    else//버퍼를 통해 리시브큐로 꽂는다.
    {
        _byte Buffer[BUF_SIZE];
        _int iResult = recv(_pSession->Get_Socket(), (_char*)Buffer, sizeof(Buffer), 0);
        _int iErrCode = WSAGetLastError();

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

    while (true)
    {
        //메시지 분석
        PACKET_HEADER tHeader;

        //헤더도 못 뽑는 정도 밖에 안들어왔다면
        if (sizeof(tHeader) > _pSession->Get_RecvQ().Get_UseSize())
        {
            break;
        }

        _int retPeek = _pSession->Get_RecvQ().Peek((_byte*)&tHeader, sizeof(tHeader));
        if (retPeek != sizeof(tHeader))
        {
            wprintf_s(L"Peek() Error:%d\n", retPeek);
            exit(1);
        }
        if (tHeader.byCode != PACKET_CODE)
        {
            wprintf_s(L"BYTEbyCode Error:%d\n", tHeader.byCode);
            //Delete_Session(_pSession);
        }
        if (tHeader.bySize + sizeof(tHeader) > (size_t)_pSession->Get_RecvQ().Get_UseSize())
        {
            break;
        }

        _pSession->Get_RecvQ().Move_Front(sizeof(tHeader));
        Decode_Packet(tHeader, _pSession);
    }
}

void CServer::Decode_Packet(const PACKET_HEADER& _Header, CSession* _pSession)
{
    _int iResult = _pSession->Receive_Data(m_Packet, _Header.bySize);
    if (iResult != _Header.bySize)
    {
        wprintf_s(L"Dequeue() Error:%d\n", iResult);
        exit(1);
    }
    m_Packet.Move_WritePos(iResult);

	//printf_s("Packet Type: %d\n", _Header.byType);

    auto it = m_PacketTypes.find(_Header.byType);
    if (it != m_PacketTypes.end())
    {
        it->second(nullptr/*&Desc*/); // 실제 패킷 처리 함수 호출
    }
    else
    {
        wprintf_s(L"Unknown packet type: %d\n", _Header.byType);
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
    _int iResult = select(0, &tCpySet, NULL, NULL, &tTimeOut);
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
				Check_Session_State(currentSock);
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
                    _int iPeek = m_vecSession[j]->Get_SendQ().Peek(Buffer, m_vecSession[j]->Get_SendQ().Get_UseSize());
                    if (iPeek <= 0) // 보낼 데이터가 없음
                        continue;

                    // 데이터 전송
                    _int iSend = send(writeSock, (_char*)Buffer, iPeek, 0);
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

void CServer::Delete_Dead_Session()
{
    //세션 정리
    for (auto iter = m_vecSession.begin(); iter != m_vecSession.end();)
    {
        if ((*iter)->Get_IsDead())
        {
            //클로즈 소켓
            closesocket((*iter)->Get_Socket());
            iter = m_vecSession.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void CServer::Check_Session_State(SOCKET sock)
{
    for (auto iter = m_vecSession.begin(); iter != m_vecSession.end(); )
    {
        if ((*iter)->Get_Socket() == sock)
        {
            // 데이터 수신 전 소켓 상태 확인
            _byte buffer[1];
            _int result = recv(sock, (_char*)buffer, sizeof(buffer), MSG_PEEK);
            if (result <= 0) // 연결 종료되었거나 오류 발생
            {
                wprintf_s(L"클라이언트 종료 ID:%d, WSAGetLastError:%d\n", (*iter)->Get_SessionInfo().iID, WSAGetLastError());
                //CPacketHandler::mp_SC_DeleteCharacter(&m_Packet, (*iter)->Get_SessionInfo().iID);
                //Send_Broadcast(*iter, m_Packet.Get_BufferPtr(), m_Packet.Get_DataSize());

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

void CServer::Set_Session_Dead(CSession* _pSession)
{
    _pSession->Set_IsDead();
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

HRESULT CServer::Send_Packet_Unicast(CSession* pSession, _uint iPacketType, void* pArg)
{
    auto it = m_PacketTypes.find(iPacketType);
    if (it == m_PacketTypes.end())
        return E_FAIL;

    it->second(pArg);

	Flush_SendBuffer(pSession);

    m_Packet.Clear();

    return S_OK;
}

HRESULT CServer::Send_Packet_Broadcast(CSession* _pSession, _uint iPacketType, void* pArg)
{
    auto it = m_PacketTypes.find(iPacketType);
    if (it == m_PacketTypes.end())
        return E_FAIL;

    for (CSession* pSession : m_vecSession)
        {
            if (pSession == _pSession)
                continue;

            it->second(pArg);
            Flush_SendBuffer(pSession);
            m_Packet.Clear();
        }

    return S_OK;
}

HRESULT CServer::Ready_Packet(_uint iPacketType, function<void(void*)> pFunction)
{
    if (!pFunction)
        return E_INVALIDARG;

    // 중복 등록 방지 (선택적)
    if (m_PacketTypes.find(iPacketType) != m_PacketTypes.end())
        return E_FAIL;

    m_PacketTypes[iPacketType] = std::move(pFunction);
    return S_OK;
}

HRESULT CServer::Clear_Packet()
{
    m_Packet.Clear();
    return S_OK;
}

HRESULT CServer::Input_Data(_byte * pByte, _int iSize)
{
    m_Packet.Enqueue(pByte, iSize);
    return S_OK;
}

HRESULT CServer::Output_Data(_byte* pByte, _int iSize)
{
    m_Packet.Dequeue(pByte, iSize);
    return S_OK;
}

HRESULT CServer::Update_Header()
{
    m_Packet.Update_HeaderSize(m_Packet.Get_DataSize() - sizeof(PACKET_HEADER));
    return S_OK;
}

HRESULT CServer::Define_Packets()
{
    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::SC_GIVE_ID), [this](void* pArg)
        {
            Clear_Packet();

            PACKET_DESC* pDesc = static_cast<PACKET_DESC*>(pArg);

            PACKET_HEADER tHeader{};
            tHeader.byCode = PACKET_CODE;
            tHeader.byType = ENUM_CLASS(PacketType::SC_GIVE_ID);

            Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
            Input_Data(reinterpret_cast<_byte*>(pDesc), sizeof(PACKET_DESC));

            Update_Header();

        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::CS_JOIN_MATCH), [this](void* pArg)
        {
            JOIN_MATCH_DESC Desc{};
            Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(JOIN_MATCH_DESC));
            Clear_Packet();

            CSession* pSession = Find_Session(Desc.iID);
            pSession->Get_SessionInfo().isJoinMatch = true;
            pSession->Get_SessionInfo().eTank = Desc.eTank;
			pSession->Get_SessionInfo().vBodyColor = Desc.vBodyColor;
			pSession->Get_SessionInfo().vTurretColor = Desc.vTurretColor;
			pSession->Get_SessionInfo().vGunColor = Desc.vGunColor;

            cout << "ID: " << Desc.iID << " TankType: " << ENUM_CLASS(pSession->Get_SessionInfo().eTank) << endl;

            //대기중인 플레이어 갯수 확인 후 있는 애들 게임 스타트 시작시키기

            _uint iPlayerCount_JoinMatch = { 0 };
             for (CSession* pSession : m_vecSession)
             {
                 cout << "iPlayerCount_JoinMatch:" << iPlayerCount_JoinMatch << endl;
                 if(true == pSession->Get_SessionInfo().isJoinMatch)
                     ++iPlayerCount_JoinMatch;
             }

             cout << "Total iPlayerCount_JoinMatch: " << iPlayerCount_JoinMatch << endl;


            if (iPlayerCount_JoinMatch == 2) //두명이 ready 상태면 시작시키기
            {
                for (CSession* _pSession : m_vecSession)
                {
                    //일단 랜덤 배치
                    _pSession->Get_SessionInfo().vPosition = _float3{ 300.f + rand() % 10, 100.f + +rand() % 10, 292.f + rand() % 10 };
                }

                cout << "Start_Game" << endl;
                //게임 씬 전환 해라
                Send_Packet_Broadcast(nullptr, ENUM_CLASS(PacketType::SC_START_GAME), pArg);
            }
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::SC_START_GAME), [this](void* pArg)
        {
            Clear_Packet();

            PACKET_HEADER tHeader{};
            tHeader.byCode = PACKET_CODE;
            tHeader.byType = ENUM_CLASS(PacketType::SC_START_GAME);

            Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));

            Update_Header();
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::CS_LOAD_COMPLETE), [this](void* pArg)
        {
            //로딩이 완료되면
            PACKET_DESC Packet_Desc{};
            Output_Data(reinterpret_cast<_byte*>(&Packet_Desc), sizeof(PACKET_DESC));
            Clear_Packet();

            CSession* pSession = Find_Session(Packet_Desc.iID);
            
            POSITION_DESC My_Pos_Desc = {};
            My_Pos_Desc.iID = Packet_Desc.iID;
            My_Pos_Desc.vPos = pSession->Get_SessionInfo().vPosition;

            //1. 내 캐릭터와
            Send_Packet_Unicast(pSession, ENUM_CLASS(PacketType::SC_CREATE_MY_CHARACTER), &My_Pos_Desc);

            cout << "Send_Packet_Unicast(SC_CREATE_MY_CHARACTER)" << endl;
            cout << "ID: " << My_Pos_Desc.iID << endl;
            cout << "Pos: " << My_Pos_Desc.vPos.x << endl;

            //2. 상대 캐릭터 만들어라
            for (CSession* pOther : m_vecSession)
            {
                if (pOther == pSession)
                    continue;

                CREATE_OTHER_TANK_DESC Other_Desc = {};
                Other_Desc.iID = pOther->Get_SessionInfo().iID;
                Other_Desc.vPos = pOther->Get_SessionInfo().vPosition;
                Other_Desc.eTank = pOther->Get_SessionInfo().eTank;
				Other_Desc.vBodyColor = pOther->Get_SessionInfo().vBodyColor;
				Other_Desc.vTurretColor = pOther->Get_SessionInfo().vTurretColor;
				Other_Desc.vGunColor = pOther->Get_SessionInfo().vGunColor;

                cout << "Send_Packet_Unicast(SC_CREATE_OTHER_CHARACTER)" << endl;
                cout << "ID: " << Other_Desc.iID << endl;
                cout << "Pos: " << Other_Desc.vPos.x << endl;

                Send_Packet_Unicast(pSession, ENUM_CLASS(PacketType::SC_CREATE_OTHER_CHARACTER), &Other_Desc);
            }

            //다 만들고 미니맵 만들어라
            Send_Packet_Unicast(pSession, ENUM_CLASS(PacketType::SC_CREATE_MINIMAP));

        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::SC_CREATE_MINIMAP), [this](void* pArg)
        {
            Clear_Packet();

            PACKET_HEADER tHeader{};
            tHeader.byCode = PACKET_CODE;
            tHeader.byType = ENUM_CLASS(PacketType::SC_CREATE_MINIMAP);

            Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));

            Update_Header();
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::SC_CREATE_MY_CHARACTER), [this](void* pArg)
        {
            Clear_Packet();

            PACKET_HEADER tHeader{};
            tHeader.byCode = PACKET_CODE;
            tHeader.byType = ENUM_CLASS(PacketType::SC_CREATE_MY_CHARACTER);

            Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));

            Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(POSITION_DESC));

            Update_Header();
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::SC_CREATE_OTHER_CHARACTER), [this](void* pArg)
        {
            Clear_Packet();
            PACKET_HEADER tHeader{};
            tHeader.byCode = PACKET_CODE;
            tHeader.byType = ENUM_CLASS(PacketType::SC_CREATE_OTHER_CHARACTER);
            Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
            Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(CREATE_OTHER_TANK_DESC));
            Update_Header();
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::CS_PRESS_W), [this](void* pArg)
        {

            BOOL_DESC Desc{};
            Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(BOOL_DESC));
            Clear_Packet();

            CSession* pSession = Find_Session(Desc.iID);

            //나를 제외한 모든 사람들에게 알려야함
            Send_Packet_Broadcast(pSession, ENUM_CLASS(PacketType::SC_PRESS_W), &Desc);
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::SC_PRESS_W), [this](void* pArg)
        {
            Clear_Packet();

            PACKET_HEADER tHeader{};
            tHeader.byCode = PACKET_CODE;
            tHeader.byType = ENUM_CLASS(PacketType::SC_PRESS_W);


            Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
            Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(BOOL_DESC));
            Update_Header();
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::CS_PRESS_S), [this](void* pArg)
        {

            BOOL_DESC Desc{};
            Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(BOOL_DESC));
            Clear_Packet();

            CSession* pSession = Find_Session(Desc.iID);

            //나를 제외한 모든 사람들에게 알려야함
            Send_Packet_Broadcast(pSession, ENUM_CLASS(PacketType::SC_PRESS_S), &Desc);
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::SC_PRESS_S), [this](void* pArg)
        {
            Clear_Packet();

            PACKET_HEADER tHeader{};
            tHeader.byCode = PACKET_CODE;
            tHeader.byType = ENUM_CLASS(PacketType::SC_PRESS_S);

            Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
            Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(BOOL_DESC));
            Update_Header();
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::CS_PRESS_A), [this](void* pArg)
        {

            BOOL_DESC Desc{};
            Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(BOOL_DESC));
            Clear_Packet();

            CSession* pSession = Find_Session(Desc.iID);

            //나를 제외한 모든 사람들에게 알려야함
            Send_Packet_Broadcast(pSession, ENUM_CLASS(PacketType::SC_PRESS_A), &Desc);
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::SC_PRESS_A), [this](void* pArg)
        {
            Clear_Packet();

            PACKET_HEADER tHeader{};
            tHeader.byCode = PACKET_CODE;
            tHeader.byType = ENUM_CLASS(PacketType::SC_PRESS_A);

            Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
            Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(BOOL_DESC));
            Update_Header();
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::CS_PRESS_D), [this](void* pArg)
        {

            BOOL_DESC Desc{};
            Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(BOOL_DESC));
            Clear_Packet();

            CSession* pSession = Find_Session(Desc.iID);

            //나를 제외한 모든 사람들에게 알려야함
            Send_Packet_Broadcast(pSession, ENUM_CLASS(PacketType::SC_PRESS_D), &Desc);
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::SC_PRESS_D), [this](void* pArg)
        {
            Clear_Packet();

            PACKET_HEADER tHeader{};
            tHeader.byCode = PACKET_CODE;
            tHeader.byType = ENUM_CLASS(PacketType::SC_PRESS_D);

            Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
            Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(BOOL_DESC));
            Update_Header();
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::CS_LEFT), [this](void* pArg)
        {

            BOOL_DESC Desc{};
            Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(BOOL_DESC));
            Clear_Packet();

            CSession* pSession = Find_Session(Desc.iID);

            //나를 제외한 모든 사람들에게 알려야함
            Send_Packet_Broadcast(pSession, ENUM_CLASS(PacketType::SC_LEFT), &Desc);
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::SC_LEFT), [this](void* pArg)
        {
            Clear_Packet();

            PACKET_HEADER tHeader{};
            tHeader.byCode = PACKET_CODE;
            tHeader.byType = ENUM_CLASS(PacketType::SC_LEFT);

            Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
            Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(BOOL_DESC));
            Update_Header();
        })))
        return E_FAIL;
    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::CS_RIGHT), [this](void* pArg)
        {

            BOOL_DESC Desc{};
            Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(BOOL_DESC));
            Clear_Packet();

            CSession* pSession = Find_Session(Desc.iID);

            //나를 제외한 모든 사람들에게 알려야함
            Send_Packet_Broadcast(pSession, ENUM_CLASS(PacketType::SC_RIGHT), &Desc);
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::SC_RIGHT), [this](void* pArg)
        {
            Clear_Packet();

            PACKET_HEADER tHeader{};
            tHeader.byCode = PACKET_CODE;
            tHeader.byType = ENUM_CLASS(PacketType::SC_RIGHT);

            Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
            Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(BOOL_DESC));
            Update_Header();
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::CS_UP), [this](void* pArg)
        {

            BOOL_DESC Desc{};
            Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(BOOL_DESC));
            Clear_Packet();

            CSession* pSession = Find_Session(Desc.iID);

            //나를 제외한 모든 사람들에게 알려야함
            Send_Packet_Broadcast(pSession, ENUM_CLASS(PacketType::SC_UP), &Desc);
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::SC_UP), [this](void* pArg)
        {
            Clear_Packet();

            PACKET_HEADER tHeader{};
            tHeader.byCode = PACKET_CODE;
            tHeader.byType = ENUM_CLASS(PacketType::SC_UP);

            Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
            Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(BOOL_DESC));
            Update_Header();
        })))
        return E_FAIL;
    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::CS_DOWN), [this](void* pArg)
        {

            BOOL_DESC Desc{};
            Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(BOOL_DESC));
            Clear_Packet();

            CSession* pSession = Find_Session(Desc.iID);

            //나를 제외한 모든 사람들에게 알려야함
            Send_Packet_Broadcast(pSession, ENUM_CLASS(PacketType::SC_DOWN), &Desc);
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::SC_DOWN), [this](void* pArg)
        {
            Clear_Packet();

            PACKET_HEADER tHeader{};
            tHeader.byCode = PACKET_CODE;
            tHeader.byType = ENUM_CLASS(PacketType::SC_DOWN);

            Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
            Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(BOOL_DESC));
            Update_Header();
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::CS_FIRE), [this](void* pArg)
        {

            PACKET_DESC Desc{};
            Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(PACKET_DESC));
            Clear_Packet();

            CSession* pSession = Find_Session(Desc.iID);

            //나를 제외한 모든 사람들에게 알려야함
            Send_Packet_Broadcast(pSession, ENUM_CLASS(PacketType::SC_FIRE), &Desc);
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::SC_FIRE), [this](void* pArg)
        {
            Clear_Packet();

            PACKET_HEADER tHeader{};
            tHeader.byCode = PACKET_CODE;
            tHeader.byType = ENUM_CLASS(PacketType::SC_FIRE);

            Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
            Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(PACKET_DESC));
            Update_Header();
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::CS_DIG), [this](void* pArg)
        {

            POSITION_DESC Desc{};
            Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(POSITION_DESC));
            Clear_Packet();

            CSession* pSession = Find_Session(Desc.iID);

            //나를 제외한 모든 사람들에게 알려야함
            Send_Packet_Broadcast(pSession, ENUM_CLASS(PacketType::SC_DIG), &Desc);
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::SC_DIG), [this](void* pArg)
        {
            Clear_Packet();

            PACKET_HEADER tHeader{};
            tHeader.byCode = PACKET_CODE;
            tHeader.byType = ENUM_CLASS(PacketType::SC_DIG);

            Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
            Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(POSITION_DESC));
            Update_Header();
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::CS_MODULE_STATE), [this](void* pArg)
        {
            MODULE_STATE_DESC Desc{};
            Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(MODULE_STATE_DESC));
            Clear_Packet();

            CSession* pSession = Find_Session(Desc.iID);

            printf_s("(CS_MODULE_STATE) ID: %d\n", Desc.iID);

            //나를 제외한 모든 사람들에게 알려야함
            Send_Packet_Broadcast(pSession, ENUM_CLASS(PacketType::SC_MODULE_STATE), &Desc);
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::SC_MODULE_STATE), [this](void* pArg)
        {
            Clear_Packet();

            PACKET_HEADER tHeader{};
            tHeader.byCode = PACKET_CODE;
            tHeader.byType = ENUM_CLASS(PacketType::SC_MODULE_STATE);

            Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
            Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(MODULE_STATE_DESC));
            Update_Header();
        })))
        return E_FAIL;


    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::CS_TANK_MATRIX), [this](void* pArg)
        {
            cout << "CS_TANK_MATRIX" << endl;

            TANK_MATRIX_DESC Desc{};
            Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(TANK_MATRIX_DESC));
            Clear_Packet();

            CSession* pSession = Find_Session(Desc.iID);

            //나를 제외한 모든 사람들에게 알려야함
            Send_Packet_Broadcast(pSession, ENUM_CLASS(PacketType::SC_TANK_MATRIX), &Desc);
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::SC_TANK_MATRIX), [this](void* pArg)
        {
            Clear_Packet();

            PACKET_HEADER tHeader{};
            tHeader.byCode = PACKET_CODE;
            tHeader.byType = ENUM_CLASS(PacketType::SC_TANK_MATRIX);

            cout << "SC_TANK_MATRIX" << endl;

            Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
            Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(TANK_MATRIX_DESC));
            Update_Header();
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::CS_MATRIX_BODY), [this](void* pArg)
        {
            MATRIX_DESC Desc{};
            Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(MATRIX_DESC));
            Clear_Packet();

            CSession* pSession = Find_Session(Desc.iID);

            //나를 제외한 모든 사람들에게 알려야함
            Send_Packet_Broadcast(pSession, ENUM_CLASS(PacketType::SC_MATRIX_BODY), &Desc);
        })))
        return E_FAIL;

    

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::SC_MATRIX_BODY), [this](void* pArg)
        {
            Clear_Packet();

            PACKET_HEADER tHeader{};
            tHeader.byCode = PACKET_CODE;
            tHeader.byType = ENUM_CLASS(PacketType::SC_MATRIX_BODY);

            Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
            Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(MATRIX_DESC));
            Update_Header();
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::CS_MATRIX_TURRET), [this](void* pArg)
        {
            MATRIX_DESC Desc{};
            Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(MATRIX_DESC));
            Clear_Packet();

            CSession* pSession = Find_Session(Desc.iID);

            //나를 제외한 모든 사람들에게 알려야함
            Send_Packet_Broadcast(pSession, ENUM_CLASS(PacketType::SC_MATRIX_TURRET), &Desc);
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::SC_MATRIX_TURRET), [this](void* pArg)
        {
            Clear_Packet();

            PACKET_HEADER tHeader{};
            tHeader.byCode = PACKET_CODE;
            tHeader.byType = ENUM_CLASS(PacketType::SC_MATRIX_TURRET);

            Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
            Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(MATRIX_DESC));
            Update_Header();
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::CS_MATRIX_GUN), [this](void* pArg)
        {
            MATRIX_DESC Desc{};
            Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(MATRIX_DESC));
            Clear_Packet();

            CSession* pSession = Find_Session(Desc.iID);

            //나를 제외한 모든 사람들에게 알려야함
            Send_Packet_Broadcast(pSession, ENUM_CLASS(PacketType::SC_MATRIX_GUN), &Desc);
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::SC_MATRIX_GUN), [this](void* pArg)
        {
            Clear_Packet();

            PACKET_HEADER tHeader{};
            tHeader.byCode = PACKET_CODE;
            tHeader.byType = ENUM_CLASS(PacketType::SC_MATRIX_GUN);

            Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
            Input_Data(reinterpret_cast<_byte*>(pArg), sizeof(MATRIX_DESC));
            Update_Header();
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::SC_POSITION), [this](void* pArg)
    {
        Clear_Packet();

        PACKET_HEADER tHeader{};
        tHeader.byCode = PACKET_CODE;
        tHeader.byType = ENUM_CLASS(PacketType::SC_POSITION);

        Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));

        POSITION_DESC* Position_Desc = static_cast<POSITION_DESC*>(pArg);

        Input_Data(reinterpret_cast<_byte*>(Position_Desc), sizeof(POSITION_DESC));
        Update_Header();
    })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::CS_POSITION), [this](void* pArg)
        {
            POSITION_DESC Desc{};
			Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(POSITION_DESC));

            //cout << Desc.vPos.x << ' ' << Desc.vPos.y << ' ' << Desc.vPos.z << endl;

            Clear_Packet();

            CSession* pSession = Find_Session(Desc.iID);
            pSession->Get_SessionInfo().vPosition = Desc.vPos;

            Send_Packet_Unicast(pSession, ENUM_CLASS(PacketType::SC_POSITION), &Desc);

        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::CS_PING), [this](void* pArg)
        {
            PACKET_DESC Desc{};
            Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(PACKET_DESC));
            CSession* pSession = Find_Session(Desc.iID);
            Send_Packet_Unicast(pSession, ENUM_CLASS(PacketType::SC_PING), &Desc);
            //cout << "CS_PING: ID_" << Desc.iID << endl;
            Clear_Packet();
        })))
        return E_FAIL;

    if (FAILED(Ready_Packet(ENUM_CLASS(PacketType::SC_PING), [this](void* pArg)
        {
            Clear_Packet();

            PACKET_HEADER tHeader{};
            tHeader.byCode = PACKET_CODE;
            tHeader.byType = ENUM_CLASS(PacketType::SC_PING);

            Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));

            Update_Header();
        })))
        return E_FAIL;

    return S_OK;
}

CSession* CServer::Find_Session(_int iID)
{
    for (CSession* pSession : m_vecSession)
    {
		if (pSession->Get_SessionInfo().iID == iID)
			return pSession;
    }

    return nullptr;
}

HRESULT CServer::Flush_SendBuffer(CSession* pSession)
{
    _int iResult{ 0 };
    iResult = pSession->Get_SendQ().Enqueue((_byte*)m_Packet.Get_BufferPtr(), m_Packet.Get_DataSize());
    if (iResult < m_Packet.Get_DataSize())
    {
        wprintf_s(L"sendQ.Enqueue() error\n");
        return E_FAIL;
    }

	return S_OK;
}
