#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Client)

class CCountdownTimer final : public CUIObject
{
private:
	CCountdownTimer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCountdownTimer(const CCountdownTimer& Prototype);
	virtual ~CCountdownTimer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	//_bool	m_bisCounting = { false };

private:
	//chrono::high_resolution_clock::time_point m_CurretTime = {};
	chrono::high_resolution_clock::time_point m_StartTime = {};

public:
	static CCountdownTimer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END