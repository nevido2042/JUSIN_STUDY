#pragma once

#include "RingBuffer.h"
#include "Packet.h"
#include "SessionInfo.h"

class CSession
{
public:
	CSession(SOCKADDR_IN _clntAdr, SOCKET _clntSock, _int _id, _float3 _Position);
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
	_bool				Get_IsDead() { return m_bIsDead; }
	void				Set_IsDead() { m_bIsDead = true; }


private:
	SOCKADDR_IN			m_ClientAddress{};
	SOCKET				m_ClientSocket = INVALID_SOCKET;
	//¸µ¹öÆÛ
	CRingBuffer			m_RecvQ;
	CRingBuffer			m_SendQ;

	SESSION_INFO		m_tSessionInfo;

	_bool				m_bIsDead = { false };
};
