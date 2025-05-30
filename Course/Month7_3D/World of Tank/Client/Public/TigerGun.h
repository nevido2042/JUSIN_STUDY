#pragma once

#include "Client_Defines.h"
#include "Gun.h"

NS_BEGIN(Client)

class CTigerGun final : public CGun
{
private:
	CTigerGun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTigerGun(const CTigerGun& Prototype);
	virtual ~CTigerGun() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components();

public:
	static CTigerGun* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END