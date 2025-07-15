#pragma once

#include "Client_Defines.h"
#include "Camera.h"

NS_BEGIN(Client)

class CCamera_Missile final : public CCamera
{
public:
	typedef struct tagCameraMissileDesc : public CCamera::CAMERA_DESC
	{
		_float			fSensor = { 0.1f };
		CGameObject*	pTarget = { nullptr };
	}CAMERA_MISSILE_DESC;
private:
	CCamera_Missile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Missile(const CCamera_Missile& Prototype);
	virtual ~CCamera_Missile() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	_float m_fTimeAcc = { 0.f };

private:
	_float m_fArmLength = { 30.f };

private:
	_float m_fSensor = { 0.1f };
private:
	CGameObject*		m_pTarget = { nullptr };

public:
	static CCamera_Missile* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END