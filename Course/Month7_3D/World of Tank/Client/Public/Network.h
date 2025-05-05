#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "GameInstance.h"

#include "RingBuffer.h"
#include "Packet.h"


#define BUF_SIZE 1024
//#define IP L"127.0.0.1"
//#define PORT 2042

BEGIN(Client)

class CNetwork : public CGameObject
{
	typedef struct tagServerConfig
	{
		wstring		strIP{};
		_int		iPort{};
	}SERVER_CONFIG;

private:
	CNetwork(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNetwork(const CNetwork& Prototype);
	virtual ~CNetwork() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void mp_CS_Move_Start(_float3 MoveStartPos, _float3& MoveDir);
	void mp_CS_Move_Stop(_float3 MoveStartPos);
	void mp_CS_Position(_float3 Pos);
public:
	void Send_To_Server();

public:
	bool Compare_ID(const int iID) const;
	SERVER_CONFIG Load_Config_File(const wstring& filename);

public:
	_bool Get_isConnected() const { return m_bConnected; }

private:
	void Send_Packet();
	void Receive_Packet();
	void Decode_Packet(const tagPACKET_HEADER& _tHeader);
	_bool Try_Connect();

private:
	SOCKET				m_hSocket;
	fd_set				m_ReadSet;
	_uint				m_iMyID;
	CRingBuffer			m_sendQ;
	CRingBuffer			m_recvQ;
	_bool				m_bConnected = { false };


	CPacket				m_Packet;
	SERVER_CONFIG		m_tServerConfig;
private:
	CShader*			m_pShaderCom = { nullptr };
	CTexture*			m_pTextureCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };


private:
	HRESULT Ready_Components();

public:
	static CNetwork* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END

