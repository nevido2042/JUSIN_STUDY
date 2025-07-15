#pragma once

#include "Client_Defines.h"
#include "LandObject.h"

NS_BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)

class CTank abstract : public CLandObject
{
	friend class CDamageBar_World;
public:
	typedef struct tagTankDesc : public LANDOBJECT_DESC
	{
		_float4 vBodyColor =	{ 1.f, 1.f, 1.f, 1.f };
		_float4 vTurretColor =	{ 1.f, 1.f, 1.f, 1.f };
		_float4 vGunColor =		{ 1.f, 1.f, 1.f, 1.f };

		TEAM	eTeam = { TEAM::END };

		CUSTOM3D e3DCustom = { CUSTOM3D::END };
	} TANK_DESC;

protected:
	CTank(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTank(const CTank& Prototype);
	virtual ~CTank() = default;

public:
	virtual HRESULT Initialize_Prototype() = 0;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Outline() override;
	virtual HRESULT Render_Shadow() override;
	virtual void On_Collision_Stay(CGameObject* pGameObject, _fvector vNormal) override;

public:
	class CModule* Find_Module(MODULE eModule) const { return m_Modules[ENUM_CLASS(eModule)]; }
public:
	_bool Get_isDie() const { return m_bisDie; }
public:
	TEAM Get_Team() const { return m_eTeam; }
public:
	void Damage_Module(MODULE eModule);
public:
	HRESULT Set_State_Module(MODULE eModule, MODULE_STATE eState);
public:
	HRESULT Try_Fire();
public:
	HRESULT Take_Damage(_float fDamage, class CShell* pShell, _float3 vFirePos = {});
public:
	void Repair_All();
private:
	void OnGround(_float fTimeDelta);

protected:
	HRESULT Store_Modules();
private:
	void Input();
private:
	void Move(_float fTimeDelta);
private:
	void Die();
private:
	void ApplyRecoil(_float fTimeDelta);
private:
	void SendMatrixSync(_float fTimeDelta);

public:
	_bool Get_IsPicked() const { return m_bisPicked; }
	void Set_IsPicked(_bool bIsPicked) { m_bisPicked = bIsPicked; }

private:
	_float m_fTimeAccDecal = { 0.f };

private:
	class CBoundary* m_pBoundary = { nullptr };

protected:
	TEAM	m_eTeam = { TEAM::END };

protected:
	vector<class CModule*>	m_Modules = {};

private:
	_float4 m_vBodyColor = { 1.f, 1.f, 1.f, 1.f };

protected:
	const	_float m_fMaxHP = { 100.f };
	_float	m_fHP = { m_fMaxHP };

private:
	_float			m_fRecoilTime = 0.f;
	const _float	m_fMaxRecoilTime = 0.3f;

private:
	const _float	m_fSyncInterval = { 0.5f };
	_float			m_fTimeAcc = { 0.f };

private:
	_bool	m_bisDie = { false };

private:
	_bool   m_bisPicked = { false };

private:

protected:
	CCollider*			m_pColliderCom = { nullptr };
	CShader*			m_pShaderCom = { nullptr };
	CModel*				m_pModelCom = { nullptr };
	CModel*				m_pModelCom_Destroyed = { nullptr };

protected:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects(TANK_DESC* pDesc);
	HRESULT Bind_ShaderResources();

public:
	virtual void Free() override;

};

NS_END