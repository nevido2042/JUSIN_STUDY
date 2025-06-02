#pragma once

#include "Client_Defines.h"
#include "Gun.h"

NS_BEGIN(Client)

class CTigerGunSkin final : public CGun
{
private:
	CTigerGunSkin(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTigerGunSkin(const CTigerGunSkin& Prototype);
	virtual ~CTigerGunSkin() = default;

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
	static CTigerGunSkin* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END