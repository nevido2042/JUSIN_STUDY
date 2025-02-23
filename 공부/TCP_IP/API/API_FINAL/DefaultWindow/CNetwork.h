#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <locale>
#include "MSG.h"

#define BUF_SIZE 1024
#define IP L"127.0.0.1"
#define PORT 2042

class CNetwork
{
public:
	CNetwork();
public:
	bool Initialize();
	void Update();
	void Release();
private:
	SOCKET  m_hSocket;
	fd_set	m_ReadSet;
	int		m_iMyID;
	CLIENT	m_ClientArr[30];
	int		m_iClientCnt;
};

