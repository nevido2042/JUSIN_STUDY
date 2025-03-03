#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "RingBuffer.h" 
#include "Packet.h"
#include "SessionInfo.h"

class CSession
{
public:
	CSession(SOCKADDR_IN _clntAdr, SOCKET _clntSock, int _id, int _x, int _y);
	~CSession();
public:
	void				Initialize();
	void				Release();
public:
	const SOCKET		Get_Socket() { return m_ClientSocket; }
	int					Receive_Data(CPacket& pPakcet, int Size);
	CRingBuffer&		Get_RecvQ() { return m_RecvQ; }
	CRingBuffer&		Get_SendQ() { return m_SendQ; }
	SESSION_INFO&		Get_SessionInfo() { return m_tSessionInfo; }

private:
	SOCKADDR_IN			m_ClientAddress{};
	SOCKET				m_ClientSocket = INVALID_SOCKET;
	//¸µ¹öÆÛ
	CRingBuffer			m_RecvQ;
	CRingBuffer			m_SendQ;

	SESSION_INFO		m_tSessionInfo;
};
