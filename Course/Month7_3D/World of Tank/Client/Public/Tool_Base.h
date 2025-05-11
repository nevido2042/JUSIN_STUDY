#pragma once

#include "Client_Defines.h"
#include "Tool.h"

NS_BEGIN(Client)

class CTool_Base final : public CTool
{
private:
	CTool_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTool_Base(const CTool_Base& Prototype);
	virtual ~CTool_Base() = default;

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
	static CTool_Base* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END