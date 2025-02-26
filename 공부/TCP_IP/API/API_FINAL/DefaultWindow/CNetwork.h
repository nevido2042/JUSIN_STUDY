#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <locale>
#include "RingBuffer.h"
#include "Packet.h"

#define BUF_SIZE 1024
#define IP L"127.0.0.1"
#define PORT 2042

class CNetwork
{
private:
	CNetwork();
	~CNetwork();
public:
	bool Initialize();
	void Update();
	void Release();
public:
	void Send_Message();

private:
	void Receive_Message();
	void Decode_Message(char iType);
private:
	SOCKET  m_hSocket;
	fd_set	m_ReadSet;
	int		m_iMyID;
	//CLIENT	m_ClientArr[30];
	//int		m_iClientCnt;
	CRingBuffer m_sendQ;
	CRingBuffer m_recvQ;

	CPacket		m_Packet;

public:
	static CNetwork* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CNetwork;

		return m_pInstance;
	}

	static void	Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CNetwork* m_pInstance;
};

