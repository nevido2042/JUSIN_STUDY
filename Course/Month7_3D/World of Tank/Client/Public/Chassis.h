#pragma once

#include "Client_Defines.h"
#include "Module.h"

NS_BEGIN(Client)
class CTrack;
NS_END

NS_BEGIN(Client)

class CChassis abstract : public CModule
{
protected:
	CChassis(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CChassis(const CChassis& Prototype);
	virtual ~CChassis() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();
	virtual void On_RaycastHit(CGameObject* pOther) override;

public:
	virtual void	Set_ModuleState(MODULE_STATE eState) override;

protected:
	HRESULT Bind_ShaderResources();

public:
	virtual void Free() override;

};

NS_END