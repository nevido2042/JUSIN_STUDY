#include "pch.h"
#include "CNetwork.h"

void CNetwork::Initialize()
{
	WSADATA wsaData;
	SOCKADDR_IN servAdr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "WSAStartup() error!" << endl;
	}

	m_hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (m_hSocket == INVALID_SOCKET)
	{
		cout << "sock() error" << endl;
	}

	u_long mode = 1;  // 1로 설정하면 비동기 모드
	ioctlsocket(m_hSocket, FIONBIO, &mode);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	/*servAdr.sin_addr.s_addr = inet_pton(AF_INET, IP, &servAdr.sin_addr);*/
	if (InetPton(AF_INET, IP, &servAdr.sin_addr) != 1)
	{
		cout << "Invalid address or address not supported" << endl;
	}

	servAdr.sin_port = htons(PORT);

	if (connect(m_hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != 10035)
		{
			cout << "connect() error: " << WSAGetLastError() << endl;
		}
		else
		{
			cout << "connected..." << endl;
		}
	}

	FD_ZERO(&m_ReadSet);
	FD_SET(m_hSocket, &m_ReadSet);
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
        }
    }
}

void CNetwork::Release()
{
	closesocket(m_hSocket);
	WSACleanup();
}
