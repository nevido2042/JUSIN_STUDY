#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "GameInstance.h"

#include "RingBuffer.h"
#include "Packet.h"

#define PING_TIME 1.f
#define BUF_SIZE 1024
//#define IP L"127.0.0.1"
//#define PORT 2042

BEGIN(Client)

class CNetwork final : public CBase
{
	DECLARE_SINGLETON(CNetwork);

	typedef struct tagServerConfig
	{
		wstring		strIP{};
		_int		iPort{};
	}SERVER_CONFIG;

private:
	CNetwork();
	//CNetwork(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	//CNetwork(const CNetwork& Prototype);
	virtual ~CNetwork() = default;

public:
	//virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg = nullptr);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void mp_CS_Move_Start(_float3 MoveStartPos, _float3& MoveDir);
	void mp_CS_Move_Stop(_float3 MoveStartPos);
	void mp_CS_Position(_float3 Pos);
private:
	void Enqueue_To_SendQ();

public:
	bool Compare_ID(const int iID) const;
	SERVER_CONFIG Load_Config_File(const wstring& filename);

public:
	NETWORK_STATUS Get_isConnected() const { return m_eStatus; }

private:
	void	Send_Packet();
	void	Receive_Packet();
	void	Decode_Packet(const tagPACKET_HEADER& _tHeader);
	_bool	Try_Connect();
	void	Send_Ping();

private:
	SOCKET				m_hSocket;
	fd_set				m_ReadSet;
	_uint				m_iMyID;
	CRingBuffer			m_sendQ;
	CRingBuffer			m_recvQ;
	NETWORK_STATUS		m_eStatus = NETWORK_STATUS::DISCONNECTED;

	CPacket				m_Packet;
	SERVER_CONFIG		m_tServerConfig;

	_float				m_fPingElapsed = { 0.f };
	_bool				m_isPing = { false };

private:
	//HRESULT Ready_Components();

public:
	//static CNetwork* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	//virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END

