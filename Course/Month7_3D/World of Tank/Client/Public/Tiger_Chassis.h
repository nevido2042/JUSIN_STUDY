#pragma once

#include "Client_Defines.h"
#include "Chassis.h"


NS_BEGIN(Client)

class CTiger_Chassis final : public CChassis
{
protected:
	CTiger_Chassis(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTiger_Chassis(const CTiger_Chassis& Prototype);
	virtual ~CTiger_Chassis() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();
	virtual void On_RaycastHit(CGameObject* pOther) override;

protected:
	HRESULT Ready_PartObject();

public:
	static CTiger_Chassis* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END