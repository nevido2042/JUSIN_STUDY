#pragma once

#include "Client_Defines.h"
#include "Turret.h"

NS_BEGIN(Client)

class CTigerTurret final : public CTurret
{
private:
	CTigerTurret(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTigerTurret(const CTigerTurret& Prototype);
	virtual ~CTigerTurret() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();

public:
	static CTigerTurret* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END