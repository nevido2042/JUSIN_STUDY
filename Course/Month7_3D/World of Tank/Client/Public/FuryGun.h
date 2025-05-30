#pragma once

#include "Client_Defines.h"
#include "Gun.h"

NS_BEGIN(Client)

class CFuryGun final : public CGun
{
private:
	CFuryGun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFuryGun(const CFuryGun& Prototype);
	virtual ~CFuryGun() = default;

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
	static CFuryGun* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END