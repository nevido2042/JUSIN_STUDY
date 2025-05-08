#pragma once
#include "Base.h"
#include "RingBuffer.h"
#include "Packet.h"

#define PING_TIME 1.f
#define BUF_SIZE 1024
//#define IP L"127.0.0.1"
//#define PORT 2042

NS_BEGIN(Engine)

//#pragma message ("패킷* 빼보는 방향으로")
//typedef function<void(CPacket*, void*)> PacketFunc;

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
	//virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg = nullptr);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Send_Packet(_uint iPacketType, void* pArg);
	HRESULT Define_Packet(_uint iPacketType, function<void(void*)> pFunction);
private:
	void Enqueue_SendBuffer();

public:
	bool Compare_ID(const int iID) const;
	SERVER_CONFIG Load_Config_File(const wstring& filename);

public:
	void			Set_Network_Status(NETWORK_STATUS eStatus) { m_eStatus = eStatus; }
	NETWORK_STATUS	Get_Network_Status() const { return m_eStatus; }

private:
	void	Flush_SendBuffer();
	void	Flush_ReceiveBuffer();
	void	Decode_Packet(const tagPACKET_HEADER& _tHeader);
	_bool	Try_Connect();

public:
	HRESULT	Clear_Packet();
	HRESULT	Input_Data(_byte* pByte, _int iSize);
	HRESULT	Output_Data(_byte* pByte, _int iSize);
	HRESULT	Update_Header();

private:
	SOCKET				m_hSocket;
	fd_set				m_ReadSet;
	_uint				m_iMyID;
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

