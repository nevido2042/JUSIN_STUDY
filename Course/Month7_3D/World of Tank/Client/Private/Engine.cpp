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
	GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
	m_iID = pDesc->iID;

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
	m_IsOn = true;
	m_pSoundCom->Play(m_EngineSound_Start);

	m_pSoundCom->Play(m_EngineSound_Loop);
	m_pSoundCom->Set_Loop(m_EngineSound_Loop);
}

void CEngine::End_Engine()
{
	m_IsOn = false;
	m_pSoundCom->Stop(m_EngineSound_Loop);

	m_pSoundCom->Play(m_EngineSound_End);
}

void CEngine::Priority_Update(_float fTimeDelta)
{
	if (GetForegroundWindow() != g_hWnd)
		return;

	if (m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::PRACTICE))
		return;

	if (m_pGameInstance->Get_ID() == m_iID )
	{
		if (m_pGameInstance->Key_Down(DIK_W))
		{
			BOOL_DESC Desc{};
			Desc.bBool = true;
			Desc.iID = m_iID;
			m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_PRESS_W),&Desc);
		}
		else if(m_pGameInstance->Key_Up(DIK_W))
		{
			BOOL_DESC Desc{};
			Desc.bBool = false;
			Desc.iID = m_iID;
			m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_PRESS_W), &Desc);
		}

		if (m_pGameInstance->Key_Down(DIK_S))
		{
			BOOL_DESC Desc{};
			Desc.bBool = true;
			Desc.iID = m_iID;
			m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_PRESS_S), &Desc);
		}
		else if (m_pGameInstance->Key_Up(DIK_S))
		{
			BOOL_DESC Desc{};
			Desc.bBool = false;
			Desc.iID = m_iID;
			m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_PRESS_S), &Desc);
		}

		if (m_pGameInstance->Key_Down(DIK_A))
		{
			BOOL_DESC Desc{};
			Desc.bBool = true;
			Desc.iID = m_iID;
			m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_PRESS_A), &Desc);
		}
		else if (m_pGameInstance->Key_Up(DIK_A))
		{
			BOOL_DESC Desc{};
			Desc.bBool = false;
			Desc.iID = m_iID;
			m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_PRESS_A), &Desc);
		}

		if (m_pGameInstance->Key_Down(DIK_D))
		{
			BOOL_DESC Desc{};
			Desc.bBool = true;
			Desc.iID = m_iID;
			m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_PRESS_D), &Desc);
		}
		else if (m_pGameInstance->Key_Up(DIK_D))
		{
			BOOL_DESC Desc{};
			Desc.bBool = false;
			Desc.iID = m_iID;
			m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_PRESS_D), &Desc);
		}
	}

}

void CEngine::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::PRACTICE) || (m_pGameInstance->Get_ID() == m_iID && GetForegroundWindow() == g_hWnd) )
		Input(fTimeDelta);
	else
		Input_Network(fTimeDelta);
}

void CEngine::Late_Update(_float fTimeDelta)
{
	m_pSoundCom->SetVolume(m_EngineSound_Loop, 0.7f + abs(m_fRPM) * m_fVolumeValue);
	m_pSoundCom->Set_Pitch(m_EngineSound_Loop, 1.f + abs(m_fRPM) * m_fPitchValue);
}

HRESULT CEngine::Render()
{

	return S_OK;
}

void CEngine::Accel_Move(_float fTimeDelta)
{
	m_bIsPressAccel = true;

	m_fMoveSpeed += fTimeDelta * 1.f;

	if (m_fMoveSpeed > MOVE_POWER_MAX)
		m_fMoveSpeed = MOVE_POWER_MAX;
	else if (m_fMoveSpeed < -MOVE_POWER_MAX)
		m_fMoveSpeed = -MOVE_POWER_MAX;

	Accel_RPM(fTimeDelta);
}

void CEngine::Accel_Turn(_float fTimeDelta)
{
	m_fTurnSpeed += fTimeDelta;

	if (m_fTurnSpeed > TURN_POWER_MAX)
		m_fTurnSpeed = TURN_POWER_MAX;
	else if (m_fTurnSpeed < -TURN_POWER_MAX)
		m_fTurnSpeed = -TURN_POWER_MAX;

	Accel_RPM(fTimeDelta);
}

void CEngine::Accel_RPM(_float fTimeDelta)
{
	m_fRPM += abs(fTimeDelta) * 1.f;

	if (m_fRPM > RPM_MAX)
		m_fRPM = RPM_MAX;
	else if (m_fRPM < 0.f)
		m_fRPM = 0.f;
}

void CEngine::Input(_float fTimeDelta)
{
	if (!m_IsOn && m_pGameInstance->Key_Pressing(DIK_W))
	{
		Start_Engine();
	}

	if (m_IsOn)
	{

		if (m_pGameInstance->Key_Pressing(DIK_W))
		{
			m_eGear = GEAR::DRIVE;

			Accel_Move(fTimeDelta);
		}
		else if (m_pGameInstance->Key_Pressing(DIK_S))
		{
			m_eGear = GEAR::REVERSE;

			Accel_Move(-fTimeDelta);
		}
		else
		{
			m_eGear = GEAR::END;

			const _float fLerpSpeed = 10.f;
			m_fMoveSpeed += -m_fMoveSpeed * fTimeDelta * fLerpSpeed;

			if (fabsf(m_fMoveSpeed) < 0.05f)
				m_fMoveSpeed = 0.f;
		}

		if (m_pGameInstance->Key_Pressing(DIK_A))
		{
			if (m_eGear == GEAR::REVERSE)
				Accel_Turn(fTimeDelta);
			else
				Accel_Turn(-fTimeDelta);
		}
		else if (m_pGameInstance->Key_Pressing(DIK_D))
		{
			if (m_eGear == GEAR::REVERSE)
				Accel_Turn(-fTimeDelta);
			else
				Accel_Turn(fTimeDelta);
		}
		else
		{
			const _float fLerpSpeed = 10.f;
			m_fTurnSpeed += -m_fTurnSpeed * fTimeDelta * fLerpSpeed;

			if (fabsf(m_fTurnSpeed) < 0.05f)
				m_fTurnSpeed = 0.f;
		}

		if (!m_pGameInstance->Key_Pressing(DIK_W) && !m_pGameInstance->Key_Pressing(DIK_A) && !m_pGameInstance->Key_Pressing(DIK_S) && !m_pGameInstance->Key_Pressing(DIK_D))
		{
			const _float fLerpSpeed = 10.f;
			m_fRPM += -m_fRPM * fTimeDelta * fLerpSpeed;

			if (fabsf(m_fRPM) < 0.05f)
				m_fRPM = 0.f;
		}


		if (m_pGameInstance->Key_Pressing(DIK_SPACE))
		{
			End_Engine();
		}
	}
}

void CEngine::Input_Network(_float fTimeDelta)
{
	if (!m_IsOn && m_bPressW)
	{
		Start_Engine();
	}

	if (m_IsOn)
	{
		const _float fLerpSpeed = 10.f;

		if (m_bPressW)
		{
			m_eGear = GEAR::DRIVE;

			Accel_Move(fTimeDelta);
		}
		else if (m_bPressS)
		{
			m_eGear = GEAR::REVERSE;

			Accel_Move(-fTimeDelta);
		}
		else
		{
			m_eGear = GEAR::END;

			m_fMoveSpeed += -m_fMoveSpeed * fTimeDelta * fLerpSpeed;

			if (fabsf(m_fMoveSpeed) < 0.05f)
				m_fMoveSpeed = 0.f;
		}

		
		if (m_bPressA)
		{
			if (m_eGear == GEAR::REVERSE)
				Accel_Turn(fTimeDelta);
			else
				Accel_Turn(-fTimeDelta);
		}
		else if (m_bPressD)
		{
			if (m_eGear == GEAR::REVERSE)
				Accel_Turn(-fTimeDelta);
			else
				Accel_Turn(fTimeDelta);
		}
		else
		{
			m_fTurnSpeed += -m_fTurnSpeed * fTimeDelta * fLerpSpeed;

			if (fabsf(m_fTurnSpeed) < 0.05f)
				m_fTurnSpeed = 0.f;
		}

		if (!m_bPressW && !m_bPressS && !m_bPressA && !m_bPressD)
		{
			//const _float fLerpSpeed = 10.f;
			m_fRPM += -m_fRPM * fTimeDelta * fLerpSpeed;

			if (fabsf(m_fRPM) < 0.05f)
				m_fRPM = 0.f;
		}
	}
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
