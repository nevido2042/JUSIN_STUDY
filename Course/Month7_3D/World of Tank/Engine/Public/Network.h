#pragma once
#include "Base.h"
#include "RingBuffer.h"
#include "Packet.h"
#include "NetCore_Define.h"

#define PING_TIME 1.f
#define BUF_SIZE 1024

NS_BEGIN(Engine)
class CNetwork final : public CBase
{
	typedef struct tagServerConfig
	{
		wstring		strIP{};
		_int		iPort{};
	}SERVER_CONFIG;

private:
	CNetwork();
	virtual ~CNetwork() = default;

public:
	virtual HRESULT Initialize(void* pArg = nullptr);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Login();

public:
	HRESULT Send_Packet(_uint iPacketType, void* pArg = nullptr);
	HRESULT Define_Packet(_uint iPacketType, function<void(void*)> pFunction);
private:
	void	Enqueue_SendBuffer();

public:
	bool Compare_ID(const int iID) const;
	SERVER_CONFIG Load_Config_File(const wstring& filename);

public:
	void			Set_Network_Status(NETWORK_STATUS eStatus) { m_eStatus = eStatus; }
	NETWORK_STATUS	Get_Network_Status() const { return m_eStatus; }

private:
	void	Flush_SendBuffer();
	void	Flush_ReceiveBuffer();
	void	Decode_Packet(const PACKET_HEADER& _tHeader);
	_bool	Try_Connect();

public:
	void	Set_ID(_int iID) { m_iMyID = iID; }
	_uint	Get_ID() { return m_iMyID; }
	HRESULT	Clear_Packet();
	HRESULT	Input_Data(_byte* pByte, _int iSize);
	HRESULT	Output_Data(_byte* pByte, _int iSize);
	HRESULT	Update_Header();

private:
	_bool	m_isLogin = { false };

private:
	SOCKET				m_hSocket{};
	fd_set				m_ReadSet{};

	_int				m_iMyID{ -1 };

	CRingBuffer			m_sendQ;
	CRingBuffer			m_recvQ;
	NETWORK_STATUS		m_eStatus = NETWORK_STATUS::DISCONNECTED;

	CPacket				m_Packet;
	SERVER_CONFIG		m_tServerConfig;

	_float				m_fConnectTryElapsed = { 0.f };
	_float				m_fPingElapsed = { 0.f };
	_bool				m_isPing = { false };

	unordered_map<_uint, function<void(void*)>> m_PacketTypes;

private:
	//HRESULT Ready_Components();

public:
	static CNetwork* Create();
	virtual void Free() override;

};

NS_END

