#include "pch.h"
#include "CNetwork.h"
#include "CAbstractFactory.h"
#include "CPlayer.h"
#include "CObjMgr.h"

CNetwork* CNetwork::m_pInstance = nullptr;

CNetwork::CNetwork()
	:m_iMyID(0), m_iClientCnt(0),m_hSocket(0)
{
	ZeroMemory(&m_ReadSet, sizeof(fd_set));
}

CNetwork::~CNetwork()
{
	Release();
}

bool CNetwork::Initialize()
{
	WSADATA wsaData;
	SOCKADDR_IN servAdr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "WSAStartup() error!" << endl;
		return false;
	}

	m_hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (m_hSocket == INVALID_SOCKET)
	{
		cout << "sock() error" << endl;
		return false;
	}

	u_long mode = 1;  // 1로 설정하면 비동기 모드
	ioctlsocket(m_hSocket, FIONBIO, &mode);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	/*servAdr.sin_addr.s_addr = inet_pton(AF_INET, IP, &servAdr.sin_addr);*/
	if (InetPton(AF_INET, IP, &servAdr.sin_addr) != 1)
	{
		cout << "Invalid address or address not supported" << endl;
		return false;
	}

	servAdr.sin_port = htons(PORT);

	if (connect(m_hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == 10035)
		{
			cout << "connected..." << endl;
		}
		else
		{
			cout << "connect() error: " << WSAGetLastError() << endl;
			return false;
		}
	}


	FD_ZERO(&m_ReadSet);
	FD_SET(m_hSocket, &m_ReadSet);

	return true;
}

void CNetwork::Update()
{
	Receive_Message();
	Send_Message();
}

void CNetwork::Release()
{
	MSG_DELETE_PLAYER tMsg;
	tMsg.type = DELETE_PLAYER;
	tMsg.id = m_iMyID;

	CNetwork::Get_Instance()->Enqueue_SendQ((MSG_ID&)tMsg, sizeof(MSG_DELETE_PLAYER));
	//Send_Message((MSG_ID&)tMsg);

	closesocket(m_hSocket);
	WSACleanup();
}

void CNetwork::Send_Message(MSG_ID& tMsg)
{
	tMsg.iID = m_iMyID;

	if (send(m_hSocket, (char*)&tMsg, sizeof(tMsg), 0) == SOCKET_ERROR)
	{
		wprintf_s(L"send() error:%d", WSAGetLastError());
	}
}

void CNetwork::Send_Message()
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

	char sendBuffer[BUF_SIZE];
	int retPeek = m_sendQ.Peek(sendBuffer, m_sendQ.GetUseSize());
	if (retPeek <= 0)
		return;

	int retSend = send(m_hSocket, sendBuffer, retPeek, 0);
	if (retSend == SOCKET_ERROR)
	{
		wprintf_s(L"send() error: %d\n", WSAGetLastError());
		return;
	}

	// 보낸 만큼만 MoveFront()
	m_sendQ.MoveFront(retSend);
}


int CNetwork::Enqueue_SendQ(MSG_ID& tMsg, int iSize)
{
	tMsg.iID = m_iMyID;
	int iResult = m_sendQ.Enqueue((char*)&tMsg, iSize);

	if (iResult < iSize)
		wprintf_s(L"sendQ.Enqueue() error\n");

	return iResult;
}

void CNetwork::Receive_Message()
{
	fd_set TempSet;
	timeval TimeOut{ 0,0 };
	char Buffer[BUF_SIZE] = { 0, };
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
		int iResultRecv = recv(m_hSocket, Buffer, sizeof(Buffer), 0);
		if (iResultRecv == SOCKET_ERROR)
		{
			wprintf_s(L"recv() error:%d\n", WSAGetLastError());
			return;
		}

		m_recvQ.Enqueue(Buffer, iResultRecv);

		while (true)
		{
			if (m_recvQ.GetUseSize() < MSG_SIZE)
				break;//중단

			char Msg[16];
			int iResult = m_recvQ.Dequeue(Msg, MSG_SIZE);
			if (iResult < MSG_SIZE)
				exit(1);//결함

			int iType{ 0 };
			if (iResult > 0)
			{
				memcpy(&iType, Msg, sizeof(int));
				Decode_Message(iType, Msg);
			}
		}
	}
}

void CNetwork::Decode_Message(int iType, char* pMsg)
{
	switch (iType)
	{
	case ALLOC_ID:
	{
		MSG_ALLOC_ID* msgAllocID = (MSG_ALLOC_ID*)pMsg;
		m_iMyID = msgAllocID->id;
		wprintf_s(L"MyID %d\n", msgAllocID->id);
		break;
	}
	case CREATE_PLAYER:
	{
		MSG_CREATE_PLAYER* msgCreatePlayer = (MSG_CREATE_PLAYER*)pMsg;
		m_ClientArr[m_iClientCnt].id = msgCreatePlayer->id;
		m_ClientArr[m_iClientCnt].x = msgCreatePlayer->x;
		m_ClientArr[m_iClientCnt].y = msgCreatePlayer->y;

		CObj* pObj = CAbstractFactory<CPlayer>::Create((float)msgCreatePlayer->x, (float)msgCreatePlayer->y);
		static_cast<CPlayer*>(pObj)->Set_ID(msgCreatePlayer->id);
		CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, pObj);

		wprintf_s(L"Create ID: %d\n", msgCreatePlayer->id);

		m_iClientCnt++;
		break;
	}
	case MOVE_RIGHT_PLAYER:
	{
		MSG_MOVE_RIGHT_PLAYER* msgMoveRight = (MSG_MOVE_RIGHT_PLAYER*)pMsg;
		CObj* pPlayer = CObjMgr::Get_Instance()->Find_Player(msgMoveRight->id);

		if (pPlayer)
		{
			static_cast<CPlayer*>(pPlayer)->Set_MoveRight(true);
		}
		break;
	}
	case MOVE_LEFT_PLAYER:
	{
		MSG_MOVE_LEFT_PLAYER* msgMoveLeft = (MSG_MOVE_LEFT_PLAYER*)pMsg;
		CObj* pPlayer = CObjMgr::Get_Instance()->Find_Player(msgMoveLeft->id);

		if (pPlayer)
		{
			static_cast<CPlayer*>(pPlayer)->Set_MoveLeft(true);
		}
		break;
	}
	case STOP_RIGHT_PLAYER:
	{
		MSG_STOP_RIGHT_PLAYER* msgStopRight = (MSG_STOP_RIGHT_PLAYER*)pMsg;
		CObj* pPlayer = CObjMgr::Get_Instance()->Find_Player(msgStopRight->id);

		if (pPlayer)
		{
			static_cast<CPlayer*>(pPlayer)->Set_MoveRight(false);
		}
		break;
	}
	case STOP_LEFT_PLAYER:
	{
		MSG_STOP_LEFT_PLAYER* msgStopLeft = (MSG_STOP_LEFT_PLAYER*)pMsg;
		CObj* pPlayer = CObjMgr::Get_Instance()->Find_Player(msgStopLeft->id);

		if (pPlayer)
		{
			static_cast<CPlayer*>(pPlayer)->Set_MoveLeft(false);
		}
		break;
	}
	case DELETE_PLAYER:
	{
		MSG_DELETE_PLAYER* msgDelete = (MSG_DELETE_PLAYER*)pMsg;

		cout << "Delete Player ID: " << msgDelete->id;
		CObj* pPlayer = CObjMgr::Get_Instance()->Find_Player(msgDelete->id);
		if (pPlayer)
		{
			pPlayer->Set_Dead();
		}
		break;
	}
	default:
	{
		wprintf(L"Unknown msg type: %d\n", iType);
		Release();
		return;
	}
	}
}

