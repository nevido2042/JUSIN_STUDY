#pragma once
#include "../../NetCore/public/Packet.h"
#include "Session.h"

#include <vector>
#include <locale>
#include <iostream>

using namespace std;

#define BUF_SIZE 1024
//#define PORT L"2042"

class CServer final
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
	int Load_Config_File(const wstring& filename);

private:
	SOCKET						m_ServSock;
	vector<CSession*>			m_vecSession;
	int							m_iID;
	fd_set						m_tReadSet;
	CPacket						m_Packet;
	int							m_iPort;
};

