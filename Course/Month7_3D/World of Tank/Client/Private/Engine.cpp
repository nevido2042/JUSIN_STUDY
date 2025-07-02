#include "Engine.h"

#include "GameInstance.h"
#include "SoundController.h"
#include "Tank.h"
#include "Icon_Module.h"
#include "DamagePanel.h"
#include "Shell.h"

CEngine::CEngine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CModule{ pDevice, pContext }
{

}

CEngine::CEngine(const CEngine& Prototype)
	: CModule(Prototype)
{

}

HRESULT CEngine::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEngine::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_EngineSound_Start = "engines_87~start_engine";
	m_EngineSound_Loop = "engines_13";
	m_EngineSound_End = "engines_14";

	m_pSoundCom->Set3DState(0.f, 30.f);

	if(m_pGameInstance->Get_NewLevel_Index() != ENUM_CLASS(LEVEL::HANGER))
	{
		Start_Engine();
	}

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, 1.f, -2.f, 1.f));

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
#pragma message ("본 게임간에 엔진을 끈다라는 건 없는걸로하고 (패킷도 안보냄). 사운드 테스트로 놔두자.")

	m_IsOn = false;
	m_pSoundCom->Stop(m_EngineSound_Loop);

	m_pSoundCom->Play(m_EngineSound_End);
}

void CEngine::Priority_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_CollisionGroup(ENUM_CLASS(COLLISION_GROUP::MODULE), this, TEXT("Com_Collider"));

	if (m_eModuleState == MODULE_STATE::DESTROYED)
		return;

	if (m_pGameInstance->Get_ID() == m_iID)
		Input(fTimeDelta);
	else
		Input_Network(fTimeDelta);

}

void CEngine::Update(_float fTimeDelta)
{
	//부모의 월드 행렬을 가져와서 자신의 월드 행렬과 곱해준다.
	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixMultiply(m_pTransformCom->Get_WorldMatrix(), XMLoadFloat4x4(m_pParentWorldMatrix)));

	m_pColliderCom->Update(XMLoadFloat4x4(&m_CombinedWorldMatrix));

	if (m_eModuleState == MODULE_STATE::DESTROYED && m_IsOn)
	{
		m_IsOn = false;
		m_pSoundCom->Stop(m_EngineSound_Loop);
		return;
	}

	const _float fLerpSpeed = 10.f;
	m_fMoveSpeed += -m_fMoveSpeed * fTimeDelta * fLerpSpeed;

	if (fabsf(m_fMoveSpeed) < 0.05f)
		m_fMoveSpeed = 0.f;

	m_fTurnSpeed += -m_fTurnSpeed * fTimeDelta * fLerpSpeed;
	if (fabsf(m_fTurnSpeed) < 0.05f)
		m_fTurnSpeed = 0.f;

	m_fRPM += -m_fRPM * fTimeDelta * fLerpSpeed * 0.1f;
	if (m_eModuleState == MODULE_STATE::DESTROYED)
		m_fRPM = 0.f;
	else if (fabsf(m_fRPM) < m_fRPM_Min)
		m_fRPM = m_fRPM_Min;

	_vector vPos = XMVectorSet(m_CombinedWorldMatrix._41, m_CombinedWorldMatrix._42, m_CombinedWorldMatrix._43, 1.0f);
	m_pSoundCom->Update3DPosition(vPos);

	m_pSoundCom->SetVolume(m_EngineSound_Loop, abs(m_fRPM));
	m_pSoundCom->Set_Pitch(m_EngineSound_Loop, abs(m_fRPM));
}

void CEngine::Late_Update(_float fTimeDelta)
{
	// 위치만 추출
	_vector vPos = XMVectorSet(
		m_CombinedWorldMatrix._41,
		m_CombinedWorldMatrix._42,
		m_CombinedWorldMatrix._43,
		1.f
	);

	// 프러스텀 안에 있으면 렌더링 추가
	if (m_pGameInstance->Is_In_Frustum(vPos, 2.f))
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);

#ifdef _DEBUG
	//m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif

}

HRESULT CEngine::Render()
{
#ifdef _DEBUG
	m_pColliderCom->Render();
#endif

	return S_OK;
}

void CEngine::On_RaycastHit(CGameObject* pOther)
{
	CModule::On_RaycastHit(pOther);
}

void CEngine::Set_ModuleState(MODULE_STATE eState)
{
	m_eModuleState = eState;

	if (m_IsOn && m_eModuleState == MODULE_STATE::DESTROYED)
		End_Engine();

	if (!m_IsOn && m_eModuleState != MODULE_STATE::DESTROYED)
		Start_Engine();

	if (m_pGameInstance->Get_ID() == m_iID)
	{
		CDamagePanel* pDamagePanel = static_cast<CDamagePanel*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_DamagePanel")));
		if (pDamagePanel == nullptr)
			return;
		if (m_pOwner)
		{
			if (!m_pOwner->Get_isDie())
				pDamagePanel->Play_Voice_ModuleState(m_eModuleType, m_eModuleState);
		}
		CIcon_Module* pIcon = static_cast<CIcon_Module*>(pDamagePanel->Find_PartObject(TEXT("Part_Engine")));
		if (pIcon == nullptr)
			return;

		pIcon->Set_ModuleState(m_eModuleState);
	}

}

//HRESULT CEngine::Damage_Engine()
//{
//	m_pOwner->Take_Damage(30.f);
//
//	Set_ModuleState(static_cast<MODULE_STATE>(max(0, _int(ENUM_CLASS(m_eModuleState) - 1))));
//
//	return S_OK;
//}

void CEngine::Accel_Move(_float fTimeDelta)
{
	//m_bIsPressAccel = true;

	if(m_eModuleState == MODULE_STATE::DAMAGED)
		m_fMoveSpeed += fTimeDelta * 50.f;
	else if(m_eModuleState == MODULE_STATE::FUNCTIONAL)
		m_fMoveSpeed += fTimeDelta * 100.f;

	if (m_fMoveSpeed > m_fMovePower_Max)
		m_fMoveSpeed = m_fMovePower_Max;
	else if (m_fMoveSpeed < -m_fMovePower_Max)
		m_fMoveSpeed = -m_fMovePower_Max;

	Accel_RPM(fTimeDelta);
}

void CEngine::Accel_Turn(_float fTimeDelta)
{
	if (m_eModuleState == MODULE_STATE::DAMAGED)
		m_fTurnSpeed += fTimeDelta * 25.f;
	else if (m_eModuleState == MODULE_STATE::FUNCTIONAL)
		m_fTurnSpeed += fTimeDelta * 50.f;

	if (m_fTurnSpeed > m_fTurnPower_Max)
		m_fTurnSpeed = m_fTurnPower_Max;
	else if (m_fTurnSpeed < -m_fTurnPower_Max)
		m_fTurnSpeed = -m_fTurnPower_Max;

	Accel_RPM(fTimeDelta);
}

void CEngine::Accel_RPM(_float fTimeDelta)
{
	if (m_eModuleState == MODULE_STATE::DAMAGED)
		m_fRPM += abs(fTimeDelta) * 1.f;
	else if (m_eModuleState == MODULE_STATE::FUNCTIONAL)
		m_fRPM += abs(fTimeDelta) * 2.f;

	if (m_fRPM > m_fRPM_Max)
		m_fRPM = m_fRPM_Max;
	else if (m_eModuleState == MODULE_STATE::DESTROYED)
		m_fRPM = 0.f;
	else if (m_fRPM < m_fRPM_Min)
		m_fRPM = m_fRPM_Min;

}

void CEngine::Input(_float fTimeDelta)
{

#pragma region Local_Input
	//if (!m_IsOn && m_pGameInstance->Key_Pressing(DIK_W))
	//{
	//	Start_Engine();
	//}

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

		//if (m_pGameInstance->Key_Pressing(DIK_SPACE))
		//{
		//	End_Engine();
		//}
	}
#pragma endregion
	
	if(m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::GAMEPLAY))
		Send_Packet();
	
}

void CEngine::Send_Packet()
{
	if (!m_IsOn)
	{
		if (m_pGameInstance->Key_Down(DIK_W))
		{
			BOOL_DESC Desc{};
			Desc.bBool = true;
			Desc.iID = m_iID;
			m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_PRESS_W), &Desc);
		}

		return;
	}

	if (m_pGameInstance->Key_Down(DIK_W))
	{
		BOOL_DESC Desc{};
		Desc.bBool = true;
		Desc.iID = m_iID;
		m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_PRESS_W), &Desc);
	}
	else if (m_pGameInstance->Key_Up(DIK_W))
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

void CEngine::Input_Network(_float fTimeDelta)
{
	/*if (!m_IsOn && m_bPressW)
	{
		Start_Engine();
	}*/

	if (m_IsOn)
	{
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
	}
}

HRESULT CEngine::Ready_Components()
{
	/* For.Com_Sound */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController_TankSound3D"),
		TEXT("Com_Sound"), reinterpret_cast<CComponent**>(&m_pSoundCom))))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_Sphere::SPHERE_DESC	SphereDesc{};
	SphereDesc.fRadius = 0.5f;
	SphereDesc.vCenter = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
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

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pSoundCom);
}
