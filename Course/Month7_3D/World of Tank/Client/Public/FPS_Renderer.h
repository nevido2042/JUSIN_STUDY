#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Client)

class CFPS_Renderer final : public CUIObject
{
private:
	CFPS_Renderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFPS_Renderer(const CFPS_Renderer& Prototype);
	virtual ~CFPS_Renderer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	_float	m_fAccTime = { 0.f };
	_uint	m_iFPS = { 0 };
	_uint	m_iFPS_Render = { 0 };

public:
	static CFPS_Renderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END