#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CSoundController;
NS_END

#define TURN_POWER_MAX 0.2f
#define MOVE_POWER_MAX 0.5f
#define RPM_MAX 1.f

NS_BEGIN(Client)

class CEngine final : public CGameObject
{
public:
	typedef struct tagEngineDesc : public GAMEOBJECT_DESC
	{
		_uint	iID = { SESSION_MAX };
	}ENGINE_DESC;
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

	void	Set_PressW(_bool bPressW) { m_bPressW = bPressW; }

private:
	void	Input(_float fTimeDelta);
	void	Input_Network(_float fTimeDelta);

private:
	_bool	m_bPressW = { false };
	_uint	m_iID = { SESSION_MAX };
private:
	string		m_EngineSound_Start = {};
	string		m_EngineSound_Loop = {};
	string		m_EngineSound_End = {};

	_bool		m_bIsPressAccel = { false };
	_bool		m_IsOn = { false };

	_float		m_fRPM = { 0.f };
	_float		m_fMoveSpeed = { 0.f };
	_float		m_fTurnSpeed = { 0.f };

	GEAR		m_eGear = { GEAR::END };

	_float		m_fVolumeValue = { 0.4f };
	_float		m_fPitchValue = { 1.1f };

private:
	CSoundController* m_pSoundCom{ nullptr };
private:
	HRESULT Ready_Components();

public:
	static CEngine* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END