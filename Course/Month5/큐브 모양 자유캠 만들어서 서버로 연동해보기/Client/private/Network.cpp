#include "Network.h"
#include "Packet.h"
#include "PacketHandler.h"
#include "Layer.h"
#include "Cube.h"

//BEGIN(Engine)
//class CLayer;
//class CGameObject;
//END


IMPLEMENT_SINGLETON(CNetwork);

CNetwork::CNetwork()
	:m_iMyID(0)/*m_iClientCnt(0)*/, m_hSocket(0)
{
	ZeroMemory(&m_ReadSet, sizeof(fd_set));
	m_pGameInstance = { CGameInstance::Get_Instance() };
}

HRESULT CNetwork::Initialize()
{
	m_tServerConfig = Load_Config_File(TEXT("config.txt"));

	WSADATA wsaData;
	SOCKADDR_IN servAdr;

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

	u_long mode = 1;  // 1로 설정하면 비동기 모드
	ioctlsocket(m_hSocket, FIONBIO, &mode);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	/*servAdr.sin_addr.s_addr = inet_pton(AF_INET, IP, &servAdr.sin_addr);*/
	if (InetPton(AF_INET, m_tServerConfig.strIP.c_str(), &servAdr.sin_addr) != 1)
	{
		MSG_BOX("Invalid address or address not supported");
		return E_FAIL;
	}

	servAdr.sin_port = htons(m_tServerConfig.iPort);

	if (connect(m_hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
	{
		/*if (WSAGetLastError() == 10035)
		{
			MSG_BOX("connected...");
		}*/
		if (WSAGetLastError() != 10035)
		{
			MSG_BOX("connect() error:"/*, WSAGetLastError()*/);

			return E_FAIL;
		}
	}


	FD_ZERO(&m_ReadSet);
	FD_SET(m_hSocket, &m_ReadSet);

	return S_OK;
}

void CNetwork::Update()
{
	Receive_Packet();
	Send_Packet();
}

//void CNetwork::Release()
//{
//	//CPacketHandler::CS_DeleteMyCharacter(&m_Packet);
//	m_sendQ.Enqueue((_byte*)m_Packet.Get_BufferPtr(), m_Packet.Get_DataSize());
//
//	//tagPACKET_CS_DELETE_CHARACTER tCS_Delete_Character;
//	//tCS_Delete_Character.iID = m_iMyID;
//	//tagPACKET_HEADER tHeader;
//	//tHeader.BYTEbyCode = PACKET_CODE;
//	//tHeader.BYTEbySize = sizeof(tCS_Delete_Character);
//	//tHeader.BYTEbyType = PACKET_CS_DELETE_CHARACTER;
//
//	//m_sendQ.Enqueue((char*)&tHeader, sizeof(tHeader));
//	//m_sendQ.Enqueue((char*)&tCS_Delete_Character, sizeof(tCS_Delete_Character));
//
//	Send_Message();
//
//	closesocket(m_hSocket);
//	WSACleanup();
//}

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
	CPacketHandler::mp_CS_Move_Start(&m_Packet, MoveStartPos, MoveDir);
	Send_To_Server();
}

void CNetwork::mp_CS_Move_Stop(_float3 MoveStartPos)
{
	CPacketHandler::mp_CS_Move_Stop(&m_Packet, MoveStartPos);
	Send_To_Server();
}

void CNetwork::mp_CS_Position(_float3 Pos)
{
	CPacketHandler::mp_CS_Position(&m_Packet, Pos);
	Send_To_Server();
}

void CNetwork::Send_To_Server(/*const _byte* pMSG, const int iSize*/)
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


//int CNetwork::Enqueue_SendQ(char* tMsg, int iSize)
//{
//	//tMsg.iID = m_iMyID;
//	int iResult = m_sendQ.Enqueue((char*)&tMsg, iSize);
//
//	if (iResult < iSize)
//		wprintf_s(L"sendQ.Enqueue() error\n");
//
//	return iResult;
//}

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
			//if (m_recvQ.GetUseSize() < MSG_SIZE)
			//	break;//중단

			//char Msg[16];
			//int iResult = m_recvQ.Dequeue(Msg, MSG_SIZE);
			//if (iResult < MSG_SIZE)
			//	exit(1);//결함

			//int iType{ 0 };
			//if (iResult > 0)
			//{
			//	memcpy(&iType, Msg, sizeof(int));
			//	Decode_Message(iType, Msg);
			//}
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

	switch (_tHeader.byType)
	{
	case PACKET_SC_CREATE_MY_CHARACTER:
	{
		CCube::CUBE_DESC tDesc;
		CPacketHandler::net_CreateMyCharacter(&m_Packet, tDesc.iID, tDesc.Postion);

		m_iMyID = tDesc.iID;

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Cube"),
			LEVEL_GAMEPLAY, TEXT("Layer_Cube"), &tDesc)))
			return;
		//CObj* pObj = CAbstractFactory<CPlayer>::Create((float)iX, (float)iY);
		//static_cast<CPlayer*>(pObj)->Set_ID(m_iMyID);
		//CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, pObj);
		//wprintf_s(L"Create ID: %d\n", m_iMyID);
		break;
	}
	case PACKET_SC_CREATE_OTHER_CHARACTER:
	{
		CCube::CUBE_DESC tDesc;
		CPacketHandler::net_CreateOtherCharacter(&m_Packet, tDesc.iID, tDesc.Postion);

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Cube"),
			LEVEL_GAMEPLAY, TEXT("Layer_Cube"), &tDesc)))
			return;

		/*CObj* pObj = CAbstractFactory<CPlayer>::Create((float)iX, (float)iY);
		static_cast<CPlayer*>(pObj)->Set_ID(iID);
		CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, pObj);*/

		//wprintf_s(L"Create ID: %d\n", iID);
		break;
	}
	case PACKET_SC_MOVE_START:
	{
		int iID;
		_float3 Postion;
		_float3 Dir;
		CPacketHandler::net_Move_Start(&m_Packet, Postion, Dir, iID);

		//해당 아이디를 가지고있는 큐브의 위치를 바꾼다
		class CLayer* pLayer = m_pGameInstance->Get_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Cube"));
		if (pLayer)
		{
			for (auto iter : pLayer->Get_GameObjects())
			{
				CCube* pCube = dynamic_cast<CCube*>(iter);
				if (pCube->Get_ID() == iID)
				{
					//방향으로 이동시작해라
					pCube->Set_Move(true);
					pCube->Set_MoveDirection(Dir);
					break;
				}
			}
		}

		break;
	}
	case PACKET_SC_MOVE_STOP:
	{
		int iID;
		_float3 Postion;
		CPacketHandler::net_Move_Stop(&m_Packet, Postion, iID);

		//해당 아이디를 가지고있는 큐브의 위치를 바꾼다
		class CLayer* pLayer = m_pGameInstance->Get_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Cube"));
		if (pLayer)
		{
			for (auto iter : pLayer->Get_GameObjects())
			{
				CCube* pCube = dynamic_cast<CCube*>(iter);
				if (pCube->Get_ID() == iID)
				{
					pCube->Set_Move(false);
					//pCube->Set_MoveDirection({ 0.f,0.f, 0.f });
					pCube->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, Postion);
					break;
				}
			}
		}

		break;
	}
	case PACKET_SC_DELETE_CHARACTER:
	{
		int iID;
		CPacketHandler::net_DeleteCharacter(&m_Packet, iID);

		//해당 아이디를 가지고있는 큐브의 위치를 바꾼다
		class CLayer* pLayer = m_pGameInstance->Get_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Cube"));
		if (pLayer)
		{
			for (auto iter : pLayer->Get_GameObjects())
			{
				CCube* pCube = dynamic_cast<CCube*>(iter);
				if (pCube->Get_ID() == iID)
				{
					//pCube->삭제
					break;
				}
			}
		}
		break;
	}
	case PACKET_SC_POSITION:
	{
		int iID; 
		_float3 vPos;
		CPacketHandler::net_Position(&m_Packet, vPos, iID);

		//해당 아이디를 가지고있는 큐브의 위치를 바꾼다
		class CLayer* pLayer = m_pGameInstance->Get_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Cube"));
		if (pLayer)
		{
			for (auto iter : pLayer->Get_GameObjects())
			{
				CCube* pCube = dynamic_cast<CCube*>(iter);
				if (pCube->Get_ID() == iID)
				{
					pCube->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
					break;
				}
			}
		}
		break;
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


void CNetwork::Free()
{
	__super::Free();

	//CPacketHandler::CS_DeleteMyCharacter(&m_Packet);
	m_sendQ.Enqueue((_byte*)m_Packet.Get_BufferPtr(), m_Packet.Get_DataSize());

	//tagPACKET_CS_DELETE_CHARACTER tCS_Delete_Character;
	//tCS_Delete_Character.iID = m_iMyID;
	//tagPACKET_HEADER tHeader;
	//tHeader.BYTEbyCode = PACKET_CODE;
	//tHeader.BYTEbySize = sizeof(tCS_Delete_Character);
	//tHeader.BYTEbyType = PACKET_CS_DELETE_CHARACTER;

	//m_sendQ.Enqueue((char*)&tHeader, sizeof(tHeader));
	//m_sendQ.Enqueue((char*)&tCS_Delete_Character, sizeof(tCS_Delete_Character));

	Send_Packet();

	closesocket(m_hSocket);
	WSACleanup();
}


