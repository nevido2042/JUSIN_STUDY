#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Client)

class CGameManager final : public CGameObject
{
private:
	CGameManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameManager(const CGameManager& Prototype);
	virtual ~CGameManager() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void		Set_Select_Tank(TANK eTank) { m_eSelectTank = eTank; }
	TANK		Get_Select_Tank() { return m_eSelectTank; }

private:
	TANK		m_eSelectTank = { TANK::FURY };

public:
	static CGameManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END