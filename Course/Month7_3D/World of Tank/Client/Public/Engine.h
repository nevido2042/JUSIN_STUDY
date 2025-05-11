#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CSoundController;
NS_END

//#define RPM_MAX 10.f
#define RPM_MIN 1.f

NS_BEGIN(Client)

class CEngine final : public CGameObject
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

public:
	void Start_Engine();
	void Press_Accelerator(_float fTimeDelta);
public:
	_float	Get_RPM() { return m_fRPM; }
	_float*	Get_RPM_Ptr() { return &m_fRPM; }
	void	Set_RPM(_float _fValue);

	_float	Get_RPM_Max() { return m_fRPM_Max; }
	void	Set_RPM_Max(_float _fValue);

	_float* Get_PitchValue_Ptr() { return &m_fPitchValue; }
	_float* Get_VolumeValue_Ptr() { return &m_fVolumeValue; }
private:
	string		m_EngineSound_Start = {};
	string		m_EngineSound_Loop = {};
	string		m_EngineSound_Accel = {};
	string		m_EngineSound_End = {};

	_bool		m_bIsPressAccel = { false };
	_bool		m_bIsEngineOn = { false };
	_bool		m_bIsEngineSoundLoopOn = { false };
	_float		m_fRPM = { 0.f };
	_float		m_fRPM_Max = { 10.f };

	_float		m_fPitchValue = { 2.f };
	_float		m_fVolumeValue = { 0.1f };
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