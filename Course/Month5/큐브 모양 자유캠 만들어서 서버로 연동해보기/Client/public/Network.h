#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "GameInstance.h"

#include "RingBuffer.h"
#include "Packet.h"
#include <locale>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#define BUF_SIZE 1024
//#define IP L"127.0.0.1"
//#define PORT 2042

BEGIN(Client)

class CNetwork : public CBase
{
	DECLARE_SINGLETON(CNetwork);

	typedef struct tagServerConfig
	{
		wstring		strIP{};
		int			iPort{};
	}SERVER_CONFIG;

private:
	CNetwork();
	virtual ~CNetwork() = default;
public:
	HRESULT Initialize();
	void Update();
public:
	void mp_CS_Move_Start(_float3 MoveStartPos, _float3& MoveDir);
	void mp_CS_Move_Stop(_float3 MoveStartPos);
	void mp_CS_Position(_float3 Pos);
public:
	void Send_To_Server(/*const _byte* pMSG, const int iSize*/);
private:
	void Send_Packet();
	void Receive_Packet();
	void Decode_Packet(const tagPACKET_HEADER& _tHeader);
public:
	bool Compare_ID(const int iID) const;
	SERVER_CONFIG Load_Config_File(const wstring& filename);
private:
	SOCKET  m_hSocket;
	fd_set	m_ReadSet;
	int		m_iMyID;
	CRingBuffer m_sendQ;
	CRingBuffer m_recvQ;

	CPacket			m_Packet;
	SERVER_CONFIG	m_tServerConfig;
private:
	CGameInstance* m_pGameInstance = { nullptr };

	virtual void Free();
};

END

