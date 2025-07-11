#pragma once

#include "Client_Defines.h"
#include "Module.h"

NS_BEGIN(Engine)
class CCollider;
class CSoundController;
NS_END

NS_BEGIN(Client)

class CEngine final : public CModule
{
private:
	CEngine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEngine(const CEngine& Prototype);
	virtual ~CEngine() = default;

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

public:
	//HRESULT Damage_Engine();

public:
	void Start_Engine();
	void End_Engine();

	void Accel_Move(_float fTimeDelta);
	void Accel_Turn(_float fTimeDelta);
	void Accel_RPM(_float fTimeDelta);

public:
	_bool	Get_isOn() const { return m_IsOn; }
	GEAR	Get_Gear() const { return m_eGear; }

	_float	Get_TurnPower() const { return m_fTurnSpeed; }
	_float	Get_MovePower() const { return m_fMoveSpeed; }
	_float	Get_RPM() const { return m_fRPM; }

	void	Set_PressW(_bool bPress) { m_bPressW = bPress; }
	void	Set_PressS(_bool bPress) { m_bPressS = bPress; }
	void	Set_PressA(_bool bPress) { m_bPressA = bPress; }
	void	Set_PressD(_bool bPress) { m_bPressD = bPress; }

private:
	void	Input(_float fTimeDelta);
	void	Send_Packet();
	void	Input_Network(_float fTimeDelta);

private:
	const _float m_fTurnPower_Max = 1.5f;
	const _float m_fMovePower_Max = 10.f;
	const _float m_fRPM_Max = 1.5f;
	const _float m_fRPM_Min = 1.f;



private:
	_bool	m_bPressW = { false };
	_bool	m_bPressS = { false };
	_bool	m_bPressA = { false };
	_bool	m_bPressD = { false };

private:
	string		m_EngineSound_Start = {};
	string		m_EngineSound_Loop = {};
	string		m_EngineSound_End = {};

	_bool		m_IsOn = { false };

	_float		m_fRPM = { 0.f };
	_float		m_fMoveSpeed = { 0.f };
	_float		m_fTurnSpeed = { 0.f };

	GEAR		m_eGear = { GEAR::END };

private:
	CCollider*			m_pColliderCom = { nullptr };
	CSoundController*	m_pSoundCom{ nullptr };
private:
	HRESULT Ready_Components();

public:
	static CEngine* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END