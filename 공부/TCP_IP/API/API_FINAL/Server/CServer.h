#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vector>
#include "MSG.h"
#include "CRingBuffer.h" 

using namespace std;

#define BUF_SIZE 1024

#define PORT L"2042"
#define SESSION_MAX 30

typedef struct tagSession
{
	tagSession(SOCKADDR_IN _clntAdr, SOCKET _clntSock, int _id , int _x, int _y)
	{
		clntAdr = _clntAdr;
		clntSock = _clntSock;
		id = _id;
		x = _x;
		y = _y;
	}
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
	void Decode_Message(int iType, char* pMsg);
	void Recieve_Message();
	void Send_Message();
private:
	SOCKET						m_ServSock;
	vector<SESSION*>				m_vecSession;
	//int							m_iSessionCnt;
	int							m_iID;
	fd_set						m_tReadSet;
};

