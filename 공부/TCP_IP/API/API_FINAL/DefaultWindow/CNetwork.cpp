#include "pch.h"
#include "CNetwork.h"
#include "CAbstractFactory.h"
#include "CPlayer.h"
#include "CObjMgr.h"

CNetwork::CNetwork()
	:m_iMyID(0), m_iClientCnt(0)
{
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
		if (WSAGetLastError() != 10035)
		{
			cout << "connect() error: " << WSAGetLastError() << endl;
			return false;
		}
		else
		{
			cout << "connected..." << endl;
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
			case CREATE_STAR:
			{
				MSG_CREATE_STAR* msgCreateStar = (MSG_CREATE_STAR*)msg;
				m_ClientArr[m_iClientCnt].id = msgCreateStar->id;
				m_ClientArr[m_iClientCnt].x = msgCreateStar->x;
				m_ClientArr[m_iClientCnt].y = msgCreateStar->y;
				CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create((float)msgCreateStar->x, (float)msgCreateStar->y));

				wprintf_s(L"Create ID: %d\n", msgCreateStar->id);

				m_iClientCnt++;
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
	closesocket(m_hSocket);
	WSACleanup();
}
