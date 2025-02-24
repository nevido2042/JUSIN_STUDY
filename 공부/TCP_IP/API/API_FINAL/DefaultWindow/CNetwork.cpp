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
    while (true) 
    {
        fd_set TempSet;
		timeval TimeOut;
		char msg[16] = { 0, };
        TempSet = m_ReadSet;
        TimeOut.tv_sec = 0;
        TimeOut.tv_usec = 0;

        int retSelect = select(0, &TempSet, 0, 0, &TimeOut);

        if (WSAGetLastError() == WSAEWOULDBLOCK || retSelect == 0) break;

        if (retSelect == SOCKET_ERROR) {
            wprintf_s(L"select() error:%d\n", WSAGetLastError());
            break;
        }

        if (FD_ISSET(m_hSocket, &TempSet))
		{
            int retRecv = recv(m_hSocket, msg, sizeof(msg), 0);
            if (retRecv == SOCKET_ERROR) 
			{
                wprintf_s(L"recv() error:%d\n", WSAGetLastError());
                break;
            }
            int type = *(int*)msg;

            switch (type)
            {
            case ALLOC_ID:
            {
                MSG_ALLOC_ID* msgAllocID = (MSG_ALLOC_ID*)msg;
				m_iMyID = msgAllocID->id;
                wprintf_s(L"MyID %d\n", msgAllocID->id);
                break;
            }
			case CREATE_PLAYER:
			{
				MSG_CREATE_PLAYER& msgCreatePlayer = (MSG_CREATE_PLAYER&)msg;
				m_ClientArr[m_iClientCnt].id = msgCreatePlayer.id;
				m_ClientArr[m_iClientCnt].x = msgCreatePlayer.x;
				m_ClientArr[m_iClientCnt].y = msgCreatePlayer.y;

				CObj* pObj = CAbstractFactory<CPlayer>::Create((float)msgCreatePlayer.x, (float)msgCreatePlayer.y);
				static_cast<CPlayer*>(pObj)->Set_ID(msgCreatePlayer.id);
				CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, pObj);

				wprintf_s(L"Create ID: %d\n", msgCreatePlayer.id);

				m_iClientCnt++;
				break;
			}
			case MOVE_RIGHT_PLAYER:
			{
				MSG_MOVE_RIGHT_PLAYER& msgMoveRight = (MSG_MOVE_RIGHT_PLAYER&)msg;
				CObj* pPlayer = CObjMgr::Get_Instance()->Find_Player(msgMoveRight.id);

				if (pPlayer) 
				{
					static_cast<CPlayer*>(pPlayer)->Set_MoveRight(true);
				}
				break;
			}
			case MOVE_LEFT_PLAYER:
			{
				MSG_MOVE_LEFT_PLAYER& msgMoveLeft = (MSG_MOVE_LEFT_PLAYER&)msg;
				CObj* pPlayer = CObjMgr::Get_Instance()->Find_Player(msgMoveLeft.id);

				if (pPlayer)
				{
					static_cast<CPlayer*>(pPlayer)->Set_MoveLeft(true);
				}
				break;
			}
			case STOP_RIGHT_PLAYER:
			{
				MSG_STOP_RIGHT_PLAYER& msgStopLeft = (MSG_STOP_RIGHT_PLAYER&)msg;
				CObj* pPlayer = CObjMgr::Get_Instance()->Find_Player(msgStopLeft.id);

				if (pPlayer)
				{
					static_cast<CPlayer*>(pPlayer)->Set_MoveRight(false);
				}
				break;
			}
			case STOP_LEFT_PLAYER:
			{
				MSG_STOP_LEFT_PLAYER& msgStopLeft = (MSG_STOP_LEFT_PLAYER&)msg;
				CObj* pPlayer = CObjMgr::Get_Instance()->Find_Player(msgStopLeft.id);

				if (pPlayer)
				{
					static_cast<CPlayer*>(pPlayer)->Set_MoveLeft(false);
				}
				break;
			}
			case DELETE_PLAYER:
			{
				MSG_DELETE_PLAYER& msgDelete = (MSG_DELETE_PLAYER&)msg;

				cout << "Delete Player ID:" << msgDelete.id;
				CObj* pPlayer = CObjMgr::Get_Instance()->Find_Player(msgDelete.id);
				if (pPlayer)
				{
					pPlayer->Set_Dead();
				}
				break;
			}
            default:
            {
                wprintf(L"Unknown msg type: %d\n", type);
				Release();
                return;
            }
            }
        }
    }
}

void CNetwork::Release()
{
	MSG_DELETE_PLAYER tMsg;
	tMsg.type = DELETE_PLAYER;
	tMsg.id = m_iMyID;
	Send_Message((MSG_ID&)tMsg);

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
