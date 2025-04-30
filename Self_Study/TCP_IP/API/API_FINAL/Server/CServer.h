#pragma once
#include <vector>
#include "Packet.h"
#include "Session.h"

using namespace std;

#define WINCX 800
#define WINCY 600

#define BUF_SIZE 1024
#define PORT L"2042"

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
	void Send_Unicast(CSession* pSession, const _byte* tMSG, const int iSize);
	void Send_Broadcast(CSession* pSession, const _byte* tMSG, const int iSize);
	void Read_Proc(CSession* pSession);
	void Decode_Message(const tagPACKET_HEADER& _Header, CSession* _pSession);
	void Recieve_Message();
	void Send_Message();
	void Delete_Session(CSession* pSession);

private:
	SOCKET						m_ServSock;
	vector<CSession*>			m_vecSession;
	int							m_iID;
	fd_set						m_tReadSet;
	CPacket						m_Packet;
};

