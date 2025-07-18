#pragma once

#include "Client_Defines.h"
#include "LandObject.h"

NS_BEGIN(Engine)
class CCollider;
class CShader;
NS_END

NS_BEGIN(Client)

class CDamageCollider final : public CLandObject
{
public:
	typedef struct tagDamageColliderDesc : public LANDOBJECT_DESC
	{
		MODULE eDamageModule = MODULE::END;
	}DAMAGECOLLIDER_DESC;

private:
	CDamageCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDamageCollider(const CDamageCollider& Prototype);
	virtual ~CDamageCollider() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();
	virtual void    On_Collision_Stay(CGameObject* pOther, _fvector vNormal) override;

private:
	wstring	m_strText = {};
private:
	MODULE m_eDamageModule = { MODULE::END };
private:
	_float m_fAccTime = 0.f;
private:
	CCollider* m_pColliderCom = { nullptr };
private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CDamageCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END