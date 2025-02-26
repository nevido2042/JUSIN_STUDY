#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vector>
#include "MSG.h"
#include "RingBuffer.h" 
#include "Packet.h"

using namespace std;

#define WINCX 800
#define WINCY 600

#define BUF_SIZE 1024

#define PORT L"2042"
#define SESSION_MAX 30

typedef struct tagSession
{
	tagSession(SOCKADDR_IN _clntAdr, SOCKET _clntSock, int _id , int _x, int _y)
	{
		clntAdr = _clntAdr;
		clntSock = _clntSock;
		iID = _id;
		iX = _x;
		iY = _y;
	}
	SOCKADDR_IN clntAdr{};
	SOCKET clntSock = INVALID_SOCKET;
	int iID = 0;
	int iX = 0;
	int iY = 0;
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
	void Send_Unicast(SESSION* pSession, const char* tMSG, const int iSize);
	void Send_Broadcast(SESSION* pSession, const char* tMSG, const int iSize);
	void Read_Proc(SESSION* pSession);
	void Decode_Message(int iType, SESSION* _pSession);
	void Recieve_Message();
	void Send_Message();
	void Delete_Session(SESSION* pSession);

private:
	SOCKET						m_ServSock;
	vector<SESSION*>			m_vecSession;
	//int						m_iSessionCnt;
	int							m_iID;
	fd_set						m_tReadSet;
	CPacket						m_CPacket;
};

