#include "Network.h"
#include "Packet.h"
#include "PacketHandler.h"
#include "Layer.h"

CNetwork::CNetwork(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{ pDevice, pContext }
{

 }

CNetwork::CNetwork(const CNetwork& Prototype)
	:CGameObject(Prototype),
	m_iMyID(Prototype.m_iMyID),
	m_hSocket(Prototype.m_hSocket),
	m_sendQ(Prototype.m_sendQ),
	m_recvQ(Prototype.m_recvQ),
	m_Packet(Prototype.m_Packet),
	m_tServerConfig(Prototype.m_tServerConfig)
{
	ZeroMemory(&m_ReadSet, sizeof(fd_set));
}

HRESULT CNetwork::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNetwork::Initialize(void* pArg)
{
	m_tServerConfig = Load_Config_File(TEXT("../bin/config.txt"));

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		MSG_BOX("WSAStartup() error!");
		return E_FAIL;
	}

	m_hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (m_hSocket == INVALID_SOCKET)
	{
		MSG_BOX("sock() error");
		return E_FAIL;
	}

	u_long mode = 1;  // 비동기
	ioctlsocket(m_hSocket, FIONBIO, &mode);

	FD_ZERO(&m_ReadSet);
	FD_SET(m_hSocket, &m_ReadSet);

	return S_OK;
}


HRESULT CNetwork::Ready_Components()
{
	return S_OK;
}

void CNetwork::Priority_Update(_float fTimeDelta)
{
}

void CNetwork::Update(_float fTimeDelta)
{
	if (!m_bConnected)
	{
		Try_Connect();

		cout << "Try_Connect()" << endl;

		return; // 아직 연결 안 됨
	}

	Receive_Packet();
	Send_Packet();
}

void CNetwork::Late_Update(_float fTimeDelta)
{
}

HRESULT CNetwork::Render()
{
	return S_OK;
}

void CNetwork::Send_Packet()
{
	fd_set writeSet;
	FD_ZERO(&writeSet);
	FD_SET(m_hSocket, &writeSet);

	timeval timeout{ 0, 0 }; // 즉시 반환

	int retSelect = select(0, NULL, &writeSet, NULL, &timeout);
	if (retSelect == SOCKET_ERROR)
	{
		wprintf_s(L"select() error: %d\n", WSAGetLastError());
		exit(1);
	}

	if (retSelect == 0)
		return;

	_byte sendBuffer[BUF_SIZE];
	int retPeek = m_sendQ.Peek(sendBuffer, m_sendQ.Get_UseSize());
	if (retPeek <= 0)
		return;

	int retSend = send(m_hSocket, (char*)sendBuffer, retPeek, 0);
	if (retSend == SOCKET_ERROR)
	{
		wprintf_s(L"send() error: %d\n", WSAGetLastError());
		return;
	}

	// 보낸 만큼만 MoveFront()
	m_sendQ.Move_Front(retSend);
}

void CNetwork::mp_CS_Move_Start(_float3 MoveStartPos, _float3& MoveDir)
{
	//CPacketHandler::mp_CS_Move_Start(&m_Packet, MoveStartPos, MoveDir);
	Send_To_Server();
}

void CNetwork::mp_CS_Move_Stop(_float3 MoveStartPos)
{
	//CPacketHandler::mp_CS_Move_Stop(&m_Packet, MoveStartPos);
	Send_To_Server();
}

void CNetwork::mp_CS_Position(_float3 Pos)
{
	//CPacketHandler::mp_CS_Position(&m_Packet, Pos);
	//Send_To_Server();
}

void CNetwork::Send_To_Server()
{
	int iResult{ 0 };
	iResult = m_sendQ.Enqueue((_byte*)m_Packet.Get_BufferPtr(), m_Packet.Get_DataSize());
	if (iResult < m_Packet.Get_DataSize())
	{
		wprintf_s(L"sendQ.Enqueue() error\n");
		exit(1);
	}

	m_Packet.Clear();
}

void CNetwork::Receive_Packet()
{
	fd_set TempSet;
	timeval TimeOut{ 0,0 };
	_byte Buffer[BUF_SIZE] = { 0, };
	TempSet = m_ReadSet;

	int retSelect = select(0, &TempSet, 0, 0, &TimeOut);

	if (WSAGetLastError() == WSAEWOULDBLOCK || retSelect == 0)
		return;

	if (retSelect == SOCKET_ERROR)
	{
		wprintf_s(L"select() error:%d\n", WSAGetLastError());
		return;
	}

	if (FD_ISSET(m_hSocket, &TempSet))
	{
		int iResultRecv = recv(m_hSocket, (char*)Buffer, sizeof(Buffer), 0);
		if (iResultRecv == SOCKET_ERROR)
		{
			wprintf_s(L"recv() error:%d\n", WSAGetLastError());
			return;
		}

		m_recvQ.Enqueue(Buffer, iResultRecv);

		while (true)
		{
			//메시지 분석
			tagPACKET_HEADER tHeader;

			//헤더도 못 뽑는 정도 밖에 안들어왔다면
			if (sizeof(tagPACKET_HEADER) > m_recvQ.Get_UseSize())
			{
				break;
			}

			int retPeek = m_recvQ.Peek((_byte*)&tHeader, sizeof(tagPACKET_HEADER));
			if (retPeek != sizeof(tHeader))
			{
				wprintf_s(L"Peek() Error:%d\n", retPeek);
				return;// exit(1);
			}
			if (tHeader.byCode != PACKET_CODE)
			{
				wprintf_s(L"BYTEbyCode Error:%d\n", tHeader.byCode);
				return;// exit(1);
			}
			if (tHeader.bySize + sizeof(tagPACKET_HEADER) > size_t(m_recvQ.Get_UseSize()))
			{
				break;
			}

			m_recvQ.Move_Front(sizeof(tagPACKET_HEADER));

			Decode_Packet(tHeader);
		}
	}
}

void CNetwork::Decode_Packet(const tagPACKET_HEADER& _tHeader)
{
	int iResult = m_recvQ.Dequeue((_byte*)m_Packet.Get_BufferPtr(), _tHeader.bySize);
	if (iResult != _tHeader.bySize)
	{
		wprintf_s(L"Dequeue() Error:%d\n", iResult);
		exit(1);
	}
	m_Packet.Move_WritePos(iResult);

	//switch (_tHeader.byType)
	//{

	//}
}

_bool CNetwork::Try_Connect()
{
	if (m_bConnected) return true;

	SOCKADDR_IN servAdr{};
	servAdr.sin_family = AF_INET;
	InetPton(AF_INET, m_tServerConfig.strIP.c_str(), &servAdr.sin_addr);
	servAdr.sin_port = htons(m_tServerConfig.iPort);

	_int ret = connect(m_hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr));
	if (ret == 0)
	{
		m_bConnected = true;
		return true;
	}
	else
	{
		_int err = WSAGetLastError();
		if (err == WSAEWOULDBLOCK || err == WSAEALREADY || err == WSAEINPROGRESS)
		{
			// 아직 연결 중. select()로 확인하거나 다음 프레임에 재시도
			return false;
		}
		else if (err == WSAEISCONN)
		{
			// 이미 연결됨
			m_bConnected = true;
			return true;
		}
		else
		{
			// 다른 에러 -> 실패했으므로 소켓 다시 만들 준비
			closesocket(m_hSocket);
			m_hSocket = socket(PF_INET, SOCK_STREAM, 0);
			u_long mode = 1;
			ioctlsocket(m_hSocket, FIONBIO, &mode);
			FD_ZERO(&m_ReadSet);
			FD_SET(m_hSocket, &m_ReadSet);
			return false;
		}
	}
}


bool CNetwork::Compare_ID(const int iID) const
{
	return m_iMyID == iID;
}

CNetwork::SERVER_CONFIG CNetwork::Load_Config_File(const std::wstring& filename)
{
	SERVER_CONFIG tConfig;
	wifstream inFile(filename);
	if (!inFile.is_open())
	{
		MSG_BOX("Can't Open ServerConfigFile");
		tConfig.strIP = L"127.0.0.1";
		tConfig.iPort = 2042;
		return tConfig;
	}

	// 첫 줄에서 IP 읽기 (wstring으로 읽음)
	wstring ip;
	getline(inFile, ip);
	tConfig.strIP = ip;

	// 두 번째 줄에서 포트 번호 읽기
	wstring portLine;
	getline(inFile, portLine);
	wstringstream wss(portLine);
	wss >> tConfig.iPort;

	return tConfig;
}

CNetwork* CNetwork::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNetwork* pInstance = new CNetwork(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CNetwork");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CNetwork::Clone(void* pArg)
{
	CNetwork* pInstance = new CNetwork(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CNetwork");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNetwork::Free()
{
	__super::Free();

	m_sendQ.Enqueue((_byte*)m_Packet.Get_BufferPtr(), m_Packet.Get_DataSize());

	Send_Packet();

	closesocket(m_hSocket);
	WSACleanup();
}


