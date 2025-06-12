#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Client)

class CDamageIndicator final : public CUIObject
{
private:
	CDamageIndicator(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDamageIndicator(const CDamageIndicator& Prototype);
	virtual ~CDamageIndicator() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

	_uint m_iNum = {};

private:
	HRESULT Ready_Components();

public:
	static CDamageIndicator* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END