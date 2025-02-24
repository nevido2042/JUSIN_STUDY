#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <array>
#include "MSG.h"
#include "CRingBuffer.h" 

using namespace std;

#define BUF_SIZE 1024

#define PORT L"2042"
#define SESSION_MAX 30

typedef struct tagSession
{
	SOCKADDR_IN clntAdr{};
	SOCKET clntSock = INVALID_SOCKET;
	int id = 0;
	int x = 0;
	int y = 0;
	//¸µ¹öÆÛ
	CRingBuffer recvQ;
	CRingBuffer sendQ;
}SESSION;

class CServer
{
public:
	CServer();
	~CServer();
public:
	bool Initialize();
	bool Update();
	void Release();
private:
	bool Network();
	void AcceptProc();
	void Send_Unicast(SESSION* pSession, const MSG_BASE* tMSG, const int iSize);
	void Send_Broadcast(SESSION* pSession, const MSG_BASE* tMSG, const int iSize);
	void Read_Proc(SESSION* pSession);
	void Decode_Message(int iType, char* pBuffer);
	void Send_Message();
private:
	SOCKET						m_ServSock;
	array<SESSION, SESSION_MAX> m_arrSession;
	int							m_iSessionCnt;
	int							m_iID;
	fd_set						m_tReadSet;
};

