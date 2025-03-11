#include "Network.h"
#include "Packet.h"
#include "PacketHandler.h"

CNetwork::CNetwork()
	:m_iMyID(0)/*m_iClientCnt(0)*/, m_hSocket(0)
{
	ZeroMemory(&m_ReadSet, sizeof(fd_set));
}

HRESULT CNetwork::Initialize()
{
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
	if (InetPton(AF_INET, IP, &servAdr.sin_addr) != 1)
	{
		MSG_BOX("Invalid address or address not supported");
		return E_FAIL;
	}

	servAdr.sin_port = htons(PORT);

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
//	m_sendQ.Enqueue((_BYTE*)m_Packet.Get_BufferPtr(), m_Packet.Get_DataSize());
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

void CNetwork::Send_To_Server(_float3 MoveStartPos/*const _byte* pMSG, const int iSize*/)
{
	CPacketHandler::mp_CS_Move_Start(&m_Packet, MoveStartPos);

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
		/*int iX;
		int iY;
		CPacketHandler::Recive_CreateMyCharacter(&m_Packet, m_iMyID, iX, iY);

		CObj* pObj = CAbstractFactory<CPlayer>::Create((float)iX, (float)iY);
		static_cast<CPlayer*>(pObj)->Set_ID(m_iMyID);
		CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, pObj);
		wprintf_s(L"Create ID: %d\n", m_iMyID);*/
		break;
	}
	case PACKET_SC_CREATE_OTHER_CHARACTER:
	{
		/*int iID;
		int iX;
		int iY;
		CPacketHandler::Recive_CreateOtherCharacter(&m_Packet, iID, iX, iY);

		CObj* pObj = CAbstractFactory<CPlayer>::Create((float)iX, (float)iY);
		static_cast<CPlayer*>(pObj)->Set_ID(iID);
		CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, pObj);

		wprintf_s(L"Create ID: %d\n", iID);*/
		break;
	}
	case PACKET_SC_DELETE_CHARACTER:
	{
		/*int iID;
		CPacketHandler::Recive_DeleteCharacter(&m_Packet, iID);

		cout << "Delete Player ID: " << iID << endl;
		CObj* pPlayer = CObjMgr::Get_Instance()->Find_Player(iID);
		if (pPlayer)
		{
			pPlayer->Set_Dead();
		}*/
		break;
	}
	}
}

CNetwork* CNetwork::Create()
{
	CNetwork* pInstance = new CNetwork();

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

	//CPacketHandler::CS_DeleteMyCharacter(&m_Packet);
	m_sendQ.Enqueue((_BYTE*)m_Packet.Get_BufferPtr(), m_Packet.Get_DataSize());

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


