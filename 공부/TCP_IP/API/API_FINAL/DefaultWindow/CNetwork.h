#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <locale>

#define IP L"127.0.0.1"
#define PORT L"3000"
#define BUF_SIZE 1024
#define IP L"127.0.0.1"
#define PORT 2042

class CNetwork
{
public:
	void Initialize();
	void Update();
	void Release();
private:
	SOCKET  m_hSocket;
	fd_set	m_ReadSet;
};

