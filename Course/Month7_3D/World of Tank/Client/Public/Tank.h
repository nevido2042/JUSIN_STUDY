#pragma once

#include "Client_Defines.h"
#include "LandObject.h"

NS_BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
class CSoundController;
NS_END

NS_BEGIN(Client)

class CTank abstract : public CLandObject
{
public:
	typedef struct tagTankDesc : public LANDOBJECT_DESC
	{
		_float4 vBodyColor =	{ 1.f, 1.f, 1.f, 1.f };
		_float4 vTurretColor =	{ 1.f, 1.f, 1.f, 1.f };
		_float4 vGunColor =		{ 1.f, 1.f, 1.f, 1.f };
	} TANK_DESC;

protected:
	CTank(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTank(const CTank& Prototype);
	virtual ~CTank() = default;

public:
	virtual HRESULT Initialize_Prototype() = 0;
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();
	virtual void On_Collision_Stay(CGameObject* pGameObject) override;

public:
	HRESULT Set_State_Module(MODULE eModule, MODULE_STATE eState);
public:
	HRESULT Try_Fire();
public:
	HRESULT Take_Damage(_float fDamage);
public:
	void Repair_All();

protected:
	HRESULT Store_Modules();
private:
	void Input();
private:
	void Move(_float fTimeDelta);
private:
	void Destroyed();
private:
	void ApplyRecoil(_float fTimeDelta);
private:
	void SendMatrixSync(_float fTimeDelta);

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
	_float			m_fTimeAcc = { 0 };

private:
	_bool	m_bisDestroyed = { false };

private:
	//_bool	m_bIsBattleStartVoice = { false }; //배틀 보이스 했는지 여부

protected:
	CCollider*			m_pColliderCom = { nullptr };
	CShader*			m_pShaderCom = { nullptr };
	CModel*				m_pModelCom = { nullptr };
	CModel*				m_pModelCom_Destroyed = { nullptr };
	//CSoundController*	m_pSoundCom_Voice = { nullptr };
	//CSoundController*	m_pSoundCom_TankSound2D = { nullptr };

private:
	HRESULT Bind_ShaderResources();

public:
	virtual void Free() override;

};

NS_END