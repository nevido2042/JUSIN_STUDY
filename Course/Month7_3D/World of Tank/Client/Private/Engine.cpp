#include "Engine.h"

#include "GameInstance.h"
#include "SoundController.h"

CEngine::CEngine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CEngine::CEngine(const CEngine& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CEngine::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEngine::Initialize(void* pArg)
{
	GAMEOBJECT_DESC			Desc{};

	Desc.fRotationPerSec = 0.f;
	Desc.fSpeedPerSec = 0.f;
	lstrcpy(Desc.szName, TEXT("Engine"));

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_EngineSound_Start = "engines_87~start_engine";
	m_EngineSound_Loop = "engines_13";
	m_EngineSound_End = "engines_14";

	return S_OK;
}

void CEngine::Start_Engine()
{
	Set_RPM(RPM_MIN);
	m_IsOn = true;
	m_pSoundCom->Play(m_EngineSound_Start);

	m_pSoundCom->Play(m_EngineSound_Loop);
	m_pSoundCom->Set_Loop(m_EngineSound_Loop);
}

void CEngine::End_Engine()
{
	Set_RPM(RPM_MIN);
	m_IsOn = false;
	m_pSoundCom->Stop(m_EngineSound_Loop);

	m_pSoundCom->Play(m_EngineSound_End);
}

void CEngine::Priority_Update(_float fTimeDelta)
{

}

void CEngine::Update(_float fTimeDelta)
{
	if (!m_IsOn && m_pGameInstance->Key_Pressing(DIK_W))
	{
		Start_Engine();
	}

	if (m_IsOn)
	{
		if (m_pGameInstance->Key_Pressing(DIK_W) )
		{
			if (m_eGear != GEAR::DRIVE)
			{
				m_fRPM = 0.f;
				m_eGear = GEAR::DRIVE;
			}
				
			Press_Accelerator(fTimeDelta);
		}
		else if (m_pGameInstance->Key_Pressing(DIK_S))
		{
			if (m_eGear != GEAR::REVERSE)
			{
				m_fRPM = 0.f;
				m_eGear = GEAR::REVERSE;
			}

			Press_Accelerator(-fTimeDelta);
		}
		else if (m_pGameInstance->Key_Pressing(DIK_D) )
		{
			if (m_eGear != GEAR::RIGHT)
			{
				m_eGear = GEAR::RIGHT;
				m_fRPM = 0.f;
			}


			Press_Accelerator(fTimeDelta * 2.f);
		}
		else if (m_pGameInstance->Key_Pressing(DIK_A))
		{
			if (m_eGear != GEAR::LEFT)
			{
				m_eGear = GEAR::LEFT;
				m_fRPM = 0.f;
			}

			Press_Accelerator(-fTimeDelta * 2.f);
		}
		else
		{
			m_eGear = GEAR::END;

			m_bIsPressAccel = false;

			const _float fLerpSpeed = 3.f;
			m_fRPM += -m_fRPM * fTimeDelta * fLerpSpeed;

			if (fabsf(m_fRPM) < 0.1f)
				m_fRPM = 0.f;

		}

		if (m_pGameInstance->Key_Pressing(DIK_SPACE))
		{
			End_Engine();
		}
	}

}

void CEngine::Late_Update(_float fTimeDelta)
{
	m_pSoundCom->SetVolume(m_EngineSound_Loop, 1.f + abs(m_fRPM) * m_fVolumeValue);
	m_pSoundCom->Set_Pitch(m_EngineSound_Loop, 1.f + abs(m_fRPM) * m_fPitchValue);
}

HRESULT CEngine::Render()
{

	return S_OK;
}

void CEngine::Press_Accelerator(_float fTimeDelta)
{
	m_bIsPressAccel = true;
	Set_RPM(m_fRPM += fTimeDelta * 10.f);
}

void CEngine::Set_RPM(_float _fValue)
{
	if (m_fRPM > RPM_MAX)
		m_fRPM = RPM_MAX;
	else if (m_fRPM < RPM_MIN)
		m_fRPM = RPM_MIN;
}

void CEngine::Set_RPM_Max(_float _fValue)
{
	//m_fRPM_Max = _fValue;
}

HRESULT CEngine::Ready_Components()
{
	/* For.Com_Sound */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController"),
		TEXT("Com_Sound"), reinterpret_cast<CComponent**>(&m_pSoundCom))))
		return E_FAIL;

	return S_OK;
}

CEngine* CEngine::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEngine* pInstance = new CEngine(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEngine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEngine::Clone(void* pArg)
{
	CEngine* pInstance = new CEngine(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEngine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEngine::Free()
{
	__super::Free();
	Safe_Release(m_pSoundCom);
}
