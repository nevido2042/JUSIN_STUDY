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
	void mp_CS_Move_Start(_float3 MoveStartPos);
	void mp_CS_Move_Stop(_float3 MoveStartPos);
public:
	void Send_To_Server(/*const _byte* pMSG, const int iSize*/);
private:
	void Send_Packet();
	void Receive_Packet();
	void Decode_Packet(const tagPACKET_HEADER& _tHeader);
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

