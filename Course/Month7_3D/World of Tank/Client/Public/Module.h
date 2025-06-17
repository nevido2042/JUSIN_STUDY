#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Client)

class CModule abstract : public CGameObject
{
public:
	typedef struct tagModuleDesc : public GAMEOBJECT_DESC
	{
		class CTank*  pOwner = { nullptr };
		MODULE	eModuleType = { MODULE::END };
	}MODULE_DESC;

protected:
	CModule(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModule(const CModule& Prototype);
	virtual ~CModule() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void On_RaycastHit(CGameObject* pOther) override;

public:
	virtual void	Set_ModuleState(MODULE_STATE eState);
	MODULE_STATE	Get_ModuleState() const { return m_eModuleState; }

public:
	void TakeDamage(_float fDamage);

protected:
	MODULE_STATE m_eModuleState = { MODULE_STATE::FUNCTIONAL };
	MODULE		 m_eModuleType = { MODULE::END };
	class CTank* m_pOwner = { nullptr };
private:
	HRESULT Ready_Components();

public:
	virtual void Free() override;

};

NS_END