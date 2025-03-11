#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "RingBuffer.h"
#include "Packet.h"
#include <locale>

#define BUF_SIZE 1024
#define IP L"127.0.0.1"
#define PORT 2042

BEGIN(Client)

class CNetwork : public CBase
{
private:
	CNetwork();
	virtual ~CNetwork() = default;
public:
	HRESULT Initialize();
	void Update();
	//void Release();
public:
	void Send_Message();

private:
	void Receive_Message();
	void Decode_Message(const tagPACKET_HEADER& _tHeader);
private:
	SOCKET  m_hSocket;
	fd_set	m_ReadSet;
	int		m_iMyID;
	CRingBuffer m_sendQ;
	CRingBuffer m_recvQ;

	CPacket		m_Packet;
public:
	static CNetwork* Create();
	virtual void Free();

};

END

