#include "Network.h"
#include "Packet.h"
#include "Layer.h"

//CNetwork::CNetwork(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
//	:CGameObject{ pDevice, pContext }
//{
//
// }
//
//CNetwork::CNetwork(const CNetwork& Prototype)
//	:CGameObject(Prototype),
//	m_iMyID(Prototype.m_iMyID),
//	m_hSocket(Prototype.m_hSocket),
//	m_sendQ(Prototype.m_sendQ),
//	m_recvQ(Prototype.m_recvQ),
//	m_Packet(Prototype.m_Packet),
//	m_tServerConfig(Prototype.m_tServerConfig)
//{
//	ZeroMemory(&m_ReadSet, sizeof(fd_set));
//}

//HRESULT CNetwork::Initialize_Prototype()
//{
//	return S_OK;
//}

CNetwork::CNetwork()
{
}

HRESULT CNetwork::Initialize(void* pArg)
{
	Clear_Packet();

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


//HRESULT CNetwork::Ready_Components()
//{
//	return S_OK;
//}

void CNetwork::Priority_Update(_float fTimeDelta)
{
	Flush_ReceiveBuffer();
}

void CNetwork::Update(_float fTimeDelta)
{
	if (m_eStatus != NETWORK_STATUS::CONNECTED && m_eStatus != NETWORK_STATUS::WARNING)
	{
		m_fConnectTryElapsed += fTimeDelta;
		if (m_fConnectTryElapsed > 1.f)
		{
			m_fConnectTryElapsed = 0.f;
			Try_Connect();
			cout << "Try_Connect()" << endl;
		}
	}

	//m_fPingElapsed += fTimeDelta;
	//if (m_fPingElapsed  > PING_TIME && m_isPing == false)
	//{
	//	Send_Packet(CPacketHandler::mp_CS_Ping);
	//	m_isPing = true;
	//	//Send_Ping();
	//}
}

void CNetwork::Late_Update(_float fTimeDelta)
{
	Flush_SendBuffer();
}

HRESULT CNetwork::Render()
{
	return S_OK;
}

void CNetwork::Flush_SendBuffer()
{
	fd_set writeSet;
	FD_ZERO(&writeSet);
	FD_SET(m_hSocket, &writeSet);

	timeval timeout{ 0, 0 }; // 즉시 반환

	int retSelect = select(0, NULL, &writeSet, NULL, &timeout);
	if (retSelect == SOCKET_ERROR)
	{
		m_eStatus = NETWORK_STATUS::DISCONNECTED;
		wprintf_s(L"select() error: %d\n", WSAGetLastError());
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
		m_eStatus = NETWORK_STATUS::DISCONNECTED;
		wprintf_s(L"send() error: %d\n", WSAGetLastError());
		return;
	}

	// 보낸 만큼만 MoveFront()
	m_sendQ.Move_Front(retSend);
}

HRESULT CNetwork::Send_Packet(_uint iPacketType, void* pArg)
{
	auto it = m_PacketTypes.find(iPacketType);
	if (it == m_PacketTypes.end())
		return E_FAIL;

	it->second(pArg);
	Enqueue_SendBuffer();
	return S_OK;
}

HRESULT CNetwork::Define_Packet(_uint iPacketType, function<void(void*)> pFunction)
{
	if (!pFunction)
		return E_INVALIDARG;

	// 중복 등록 방지 (선택적)
	if (m_PacketTypes.find(iPacketType) != m_PacketTypes.end())
		return E_FAIL;

	m_PacketTypes[iPacketType] = std::move(pFunction);
	return S_OK;
}

void CNetwork::Enqueue_SendBuffer()
{
	_int iResult{ 0 };
	iResult = m_sendQ.Enqueue((_byte*)m_Packet.Get_BufferPtr(), m_Packet.Get_DataSize());
	if (iResult < m_Packet.Get_DataSize())
	{
		wprintf_s(L"sendQ.Enqueue() error\n");
		exit(1);
	}

	m_Packet.Clear();
}

void CNetwork::Flush_ReceiveBuffer()
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
		m_eStatus = NETWORK_STATUS::DISCONNECTED;
		wprintf_s(L"select() error:%d\n", WSAGetLastError());
		return;
	}

	if (FD_ISSET(m_hSocket, &TempSet))
	{
		int iResultRecv = recv(m_hSocket, (char*)Buffer, sizeof(Buffer), 0);
		if (iResultRecv == SOCKET_ERROR)
		{
			m_eStatus = NETWORK_STATUS::DISCONNECTED;
			wprintf_s(L"recv() error:%d\n", WSAGetLastError());
			return;
		}

		m_recvQ.Enqueue(Buffer, iResultRecv);

		while (true)
		{
			//메시지 분석
			PACKET_HEADER tHeader;

			//헤더도 못 뽑는 정도 밖에 안들어왔다면
			if (sizeof(PACKET_HEADER) > m_recvQ.Get_UseSize())
			{
				break;
			}

			int retPeek = m_recvQ.Peek((_byte*)&tHeader, sizeof(PACKET_HEADER));
			if (retPeek != sizeof(tHeader))
			{
				wprintf_s(L"Peek() Error:%d\n", retPeek);
				return;
			}
			if (tHeader.byCode != PACKET_CODE)
			{
				wprintf_s(L"BYTEbyCode Error:%d\n", tHeader.byCode);
				return;
			}
			if (tHeader.bySize + sizeof(PACKET_HEADER) > size_t(m_recvQ.Get_UseSize()))
			{
				break;
			}

			m_recvQ.Move_Front(sizeof(PACKET_HEADER));

			Decode_Packet(tHeader);
		}
	}
}

void CNetwork::Decode_Packet(const PACKET_HEADER& _tHeader)
{
	int iResult = m_recvQ.Dequeue((_byte*)m_Packet.Get_BufferPtr(), _tHeader.bySize);
	if (iResult != _tHeader.bySize)
	{
		wprintf_s(L"Dequeue() Error:%d\n", iResult);
		MSG_BOX("Dequeue() Error");
	}
	m_Packet.Move_WritePos(iResult);

	auto it = m_PacketTypes.find(_tHeader.byType);
	if (it != m_PacketTypes.end())
	{
		//들어온 패킷 데이터는 OutPutData()에서 처리
		it->second(nullptr);
	}
	else
	{
		wprintf_s(L"Unknown packet type: %d\n", _tHeader.byType);
	}
}

_bool CNetwork::Try_Connect()
{
	if (m_eStatus == NETWORK_STATUS::CONNECTED) return true;

	SOCKADDR_IN servAdr{};
	servAdr.sin_family = AF_INET;
	InetPton(AF_INET, m_tServerConfig.strIP.c_str(), &servAdr.sin_addr);
	servAdr.sin_port = htons(m_tServerConfig.iPort);

	_int ret = connect(m_hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr));
	_int err = WSAGetLastError();
	if (err == WSAEWOULDBLOCK)
	{
		return false;
	}

	return false;
}

HRESULT CNetwork::Clear_Packet()
{
	m_Packet.Clear();
	return S_OK;
}

HRESULT CNetwork::Input_Data(_byte* pByte, _int iSize)
{
	m_Packet.Enqueue(pByte, iSize);
	return S_OK;
}

HRESULT CNetwork::Output_Data(_byte* pByte, _int iSize)
{
	m_Packet.Dequeue(pByte, iSize);
	return S_OK;
}

HRESULT CNetwork::Update_Header()
{
	m_Packet.Update_HeaderSize(m_Packet.Get_DataSize() - sizeof(PACKET_HEADER));
	return S_OK;
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

//CNetwork* CNetwork::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
//{
//	CNetwork* pInstance = new CNetwork(pDevice, pContext);
//
//	if (FAILED(pInstance->Initialize_Prototype()))
//	{
//		MSG_BOX("Failed to Created : CNetwork");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}
//
//CGameObject* CNetwork::Clone(void* pArg)
//{
//	CNetwork* pInstance = new CNetwork(*this);
//
//	if (FAILED(pInstance->Initialize(pArg)))
//	{
//		MSG_BOX("Failed to Cloned : CNetwork");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}

CNetwork* CNetwork::Create()
{
	CNetwork* pInstance = new CNetwork();
#pragma message ("나중에 전투시작 같은 거 누르면 접속하게끔 Initialize를 겜인스턴스로 호출할 수 있도록 하면 좋을 듯?")
	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CNetwork");
		Safe_Release(pInstance);
	}
	
	return pInstance;
}

void CNetwork::Free()
{
	__super::Free();

	m_sendQ.Enqueue((_byte*)m_Packet.Get_BufferPtr(), m_Packet.Get_DataSize());

	Flush_SendBuffer();

	closesocket(m_hSocket);
	WSACleanup();
}


