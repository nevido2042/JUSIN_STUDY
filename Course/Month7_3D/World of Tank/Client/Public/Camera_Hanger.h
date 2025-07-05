#pragma once

#include "Client_Defines.h"
#include "Camera.h"

NS_BEGIN(Client)

class CCamera_Hanger final : public CCamera
{
public:
	typedef struct tagCameraHangerDesc : public CCamera::CAMERA_DESC
	{
		_float			fSensor = { 0.1f };
		CGameObject* pTarget = { nullptr };
	}CAMERA_HANGER_DESC;
private:
	CCamera_Hanger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Hanger(const CCamera_Hanger& Prototype);
	virtual ~CCamera_Hanger() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	_float m_fArmLength = { 8.f };

private:
	_float				m_fSensor = { 0.1f };
	CGameObject*		m_pTarget = { nullptr };

public:
	static CCamera_Hanger* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END