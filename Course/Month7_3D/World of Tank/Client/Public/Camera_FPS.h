#pragma once

#include "Client_Defines.h"
#include "Camera.h"

NS_BEGIN(Client)

class CCamera_FPS final : public CCamera
{
public:
	typedef struct tagCameraFPSDesc : public CCamera::CAMERA_DESC
	{
		_float			fSensor = { 0.1f };
		CGameObject*	pTarget = { nullptr };
	}CAMERA_FPS_DESC;

private:
	CCamera_FPS(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_FPS(const CCamera_FPS& Prototype);
	virtual ~CCamera_FPS() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	void Picking();

private:
	const float m_fMinFov = XMConvertToRadians(5.0f);
	const float m_fMaxFov = XMConvertToRadians(60.0f);

private:
	_float				m_fSensor = { 1.0f };
	CGameObject*		m_pTarget = { nullptr };
	_float4				m_vLook = {};

public:
	static CCamera_FPS* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END