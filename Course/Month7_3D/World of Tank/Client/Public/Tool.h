#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Client)

class CTool abstract : public CUIObject
{
protected:
	CTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTool(const CTool& Prototype);
	virtual ~CTool() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void Free() override;

};

NS_END