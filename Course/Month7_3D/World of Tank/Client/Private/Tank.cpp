#include "Tank.h"

#include "Turret.h"
#include "Gun.h"
#include "Track.h"
#include "GameInstance.h"
#include "Engine.h"
#include "GameManager.h"
#include "AmmoBay.h"
#include "Boundary.h"

#pragma region UI
#include "Icon_Module.h"
#include "DamageBar_World.h"
#include "DamageBar.h"
#include "DamagePanel.h"
#include "PickedManager.h"
#include "Score.h"
#pragma endregion

CTank::CTank(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CLandObject(pDevice, pContext)
{
}

CTank::CTank(const CTank& Prototype)
	:CLandObject(Prototype)
{
}

HRESULT CTank::Initialize(void* pArg)
{
	TANK_DESC* pDesc = static_cast<TANK_DESC*>(pArg);
	//(*pDesc).fRotationPerSec = 0.1f;
	//(*pDesc).fSpeedPerSec = 0.5f;
	(*pDesc).iLevelIndex = m_pGameInstance->Get_NewLevel_Index();
	(*pDesc).strLayerTag = TEXT("Layer_Terrain");
	(*pDesc).strComponentTag = TEXT("Com_VIBuffer");
	(*pDesc).iIndex = 0;

	m_vBodyColor = pDesc->vBodyColor;
	m_iID = pDesc->iID;
	m_eTeam = pDesc->eTeam;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pBoundary = static_cast<CBoundary*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Boundary")));
	if(m_pBoundary)
		Safe_AddRef(m_pBoundary);

	//m_pSoundCom_TankSound3D->Set3DState(0.f, 30.f);


	return S_OK;
}

void CTank::Priority_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::HANGER))
		return;

	m_pGameInstance->Add_CollisionGroup(ENUM_CLASS(COLLISION_GROUP::BODY), this, TEXT("Com_Collider"));
	
	if (m_pGameInstance->Get_ID() == m_iID)
	{
		CGameObject* pCountdownTimer = m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_CountdownTimer"));
		if (pCountdownTimer)
		{
			if (pCountdownTimer->Get_isActive())
				return;
		}

		if (m_bisTankDestroyed)
			return;

		Input();
	}

	CGameObject::Priority_Update(fTimeDelta);
}

void CTank::Update(_float fTimeDelta)
{
	m_pGameInstance->Check_Collision(ENUM_CLASS(COLLISION_GROUP::BODY), this, TEXT("Com_Collider"), TEXT("Com_Collider"));
	m_pGameInstance->Check_Collision(ENUM_CLASS(COLLISION_GROUP::BUILDING), this, TEXT("Com_Collider"), TEXT("Com_Collider"));

	Move(fTimeDelta);

	if (m_pBoundary)
	{
		_float3 vPos = {};
		XMStoreFloat3(&vPos, m_pTransformCom->Get_State(STATE::POSITION));

		if (!m_pBoundary->IsPointInBoundary(vPos))
		{
			_float3 vClosestPos = m_pBoundary->ClosestPointOnBoundary(vPos);
			m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&vClosestPos), 1.f));

		}
	}

	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));

	//m_pSoundCom_TankSound3D->Update3DPosition(m_pTransformCom->Get_State(STATE::POSITION));

	CLandObject::SetUp_Height_Normal(m_pTransformCom, fTimeDelta, 0.5f);

	// 반동 적용
#pragma message ("FPS 카메라 반동 주는 것이 방향에 따라 달라진다 해결 못함(원본 겜도 못해서 안넣은거일거야)")
	ApplyRecoil(fTimeDelta);

	SendMatrixSync(fTimeDelta);

	//if (m_pGameInstance->Get_ID() != m_iID)
	//{
	//	Picked_Ray_ScreenCenter();
	//	Picked_Ray_Gun();
	//}


	CGameObject::Update(fTimeDelta);
}

void CTank::Late_Update(_float fTimeDelta)
{
	if (!m_bVisible)
	{
		m_Modules.at(ENUM_CLASS(MODULE::AMMO_BAY))->Late_Update(fTimeDelta);
		return;
	}

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);

	if (m_bisTankDestroyed)
		return;

	CGameObject::Late_Update(fTimeDelta);

}


HRESULT CTank::Render()
{

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (m_pModelCom && !m_bisTankDestroyed)
	{
		_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

		for (_uint i = 0; i < iNumMesh; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Begin(2)))
				return E_FAIL;

			if (FAILED(m_pModelCom->Render(i)))
				return E_FAIL;
		}
	}
	else if (m_pModelCom_Destroyed)
	{
		_uint		iNumMesh = m_pModelCom_Destroyed->Get_NumMeshes();

		for (_uint i = 0; i < iNumMesh; i++)
		{
			if (FAILED(m_pModelCom_Destroyed->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Begin(2)))
				return E_FAIL;

			if (FAILED(m_pModelCom_Destroyed->Render(i)))
				return E_FAIL;
		}
	}

#ifdef _DEBUG
	m_pColliderCom->Render();
#endif

	return S_OK;
}

void CTank::On_Collision_Stay(CGameObject* pGameObject, _fvector vNormal)
{
	//if(!m_pSoundCom_TankSound3D->IsPlaying("phys_coll_83"))
	//	m_pSoundCom_TankSound3D->Play("phys_coll_83");
}

void CTank::Damage_Module(MODULE eModule, _float fDamage)
{
	if (ENUM_CLASS(eModule) < 0 || ENUM_CLASS(eModule) >= ENUM_CLASS(MODULE::END))
		return;

	m_Modules[ENUM_CLASS(eModule)]->TakeDamage(fDamage);

	//switch (eModule)
	//{
	//	case MODULE::ENGINE:
	//	static_cast<CEngine*>(m_Modules[ENUM_CLASS(MODULE::ENGINE)])->Damage_Engine();
	//		break;
	//	case MODULE::AMMO_BAY:
	//		m_Modules[ENUM_CLASS(eModule)]->TakeDamage();
	//		break;
	//}

}


HRESULT CTank::Set_State_Module(MODULE eModule, MODULE_STATE eState)
{
	m_Modules[ENUM_CLASS(eModule)]->Set_ModuleState(eState);

	//switch (eModule)
	//{
	//case MODULE::ENGINE:
	//	m_Modules[ENUM_CLASS(MODULE::ENGINE)]->Set_ModuleState(eState);
	//	break;
	//case MODULE::AMMO_BAY:
	//	m_Modules[ENUM_CLASS(MODULE::AMMO_BAY)]->Set_ModuleState(eState);
	//		break;
	//}

	return S_OK;
}

void CTank::Input()
{
	if (m_pGameInstance->Mouse_Down(ENUM_CLASS(DIMK::LBUTTON)))
	{
		if (FAILED(Try_Fire()))
			return;

		if (m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::GAMEPLAY))
		{
			PACKET_DESC Desc{};
			Desc.iID = m_iID;
			m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_FIRE), &Desc);
		}
	}
}

HRESULT CTank::Try_Fire()
{
	if (m_pGameInstance->Get_ID() == m_iID)
	{
		CAmmoBay* pAmmoBay = static_cast<CAmmoBay*>(m_Modules.at(ENUM_CLASS(MODULE::AMMO_BAY)));

		//장전 완료 여부 확인
		if (FAILED(pAmmoBay->is_Load_Complete()))
			return E_FAIL;

		//발사
		if (FAILED(static_cast<CGun*>(m_Modules.at(ENUM_CLASS(MODULE::GUN)))->Fire()))
			return E_FAIL;

		//꿀렁임
		m_fRecoilTime = m_fMaxRecoilTime;

		pAmmoBay->Start_Load();

	}
	else
	{
		//발사
		if (FAILED(static_cast<CGun*>(m_Modules.at(ENUM_CLASS(MODULE::GUN)))->Fire()))
			return E_FAIL;

		//꿀렁임
		m_fRecoilTime = m_fMaxRecoilTime;

	}

	return S_OK;

}

HRESULT CTank::Take_Damage(_float fDamage)
{
	m_fHP -= fDamage;

	if (m_fHP <= 0.f)
	{
		m_fHP = 0.f;

		Destroyed();
	}

	if (m_pGameInstance->Get_ID() == m_iID)
	{
		//플레이어 탱크라면 체력바 찾아서 깎아라 && 맞는 소리 (꽝)
		CDamageBar* pDamageBar = static_cast<CDamageBar*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_DamageBar")));
		if (nullptr == pDamageBar)
			return E_FAIL;
	
		pDamageBar->Fill(m_fHP / m_fMaxHP);
	}

	static_cast<CDamageBar_World*> (Find_PartObject(TEXT("Part_DamageBar")))->Fill(m_fHP / m_fMaxHP);

	return S_OK;
}

void CTank::Repair_All()
{
	if (m_bisTankDestroyed == true)
		return;

	for(_uint i = 0; i < ENUM_CLASS(MODULE::END); ++i)
	{
		if (nullptr == m_Modules[i])
			continue;

		m_Modules[i]->Set_ModuleState(MODULE_STATE::FUNCTIONAL);
		//Set_State_Module(static_cast<MODULE>(i), MODULE_STATE::FUNCTIONAL);

		if(m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::GAMEPLAY))
		{
			MODULE_STATE_DESC Desc{};
			Desc.iID = m_iID;
			Desc.iTargetID = m_iID;
			Desc.eModule = static_cast<MODULE>(i);
			Desc.eState = MODULE_STATE::FUNCTIONAL;
			m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_MODULE_STATE), &Desc);
		}
	}

	CDamagePanel* pDamagePanel = static_cast<CDamagePanel*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_DamagePanel")));
	if (pDamagePanel)
		pDamagePanel->Repair_All();

	static_cast<CEngine*>(m_Modules[ENUM_CLASS(MODULE::ENGINE)])->Start_Engine();
}

HRESULT CTank::Store_Modules()
{
	CModule* pEngine = static_cast<CModule*>(Find_PartObject(TEXT("Part_Engine")));
	CModule* pTurret = static_cast<CModule*>(Find_PartObject(TEXT("Part_Turret")));
	CModule* pGun = static_cast<CModule*>(Find_PartObject(TEXT("Part_Turret"))->Find_PartObject(TEXT("Part_Gun")));
	CModule* pTrackLeft = static_cast<CModule*>(Find_PartObject(TEXT("Part_TrackLeft")));
	CModule* pTrackRight = static_cast<CModule*>(Find_PartObject(TEXT("Part_TrackRight")));
	CModule* pAmmoBay = static_cast<CModule*>(Find_PartObject(TEXT("Part_AmmoBay")));

	m_Modules.resize(ENUM_CLASS(MODULE::END));
	m_Modules.at(ENUM_CLASS(MODULE::ENGINE)) = pEngine;
	m_Modules.at(ENUM_CLASS(MODULE::TURRET)) = pTurret;
	m_Modules.at(ENUM_CLASS(MODULE::GUN)) = pGun;
	m_Modules.at(ENUM_CLASS(MODULE::TRACK_LEFT)) = pTrackLeft;
	m_Modules.at(ENUM_CLASS(MODULE::TRACK_RIGHT)) = pTrackRight;
	m_Modules.at(ENUM_CLASS(MODULE::AMMO_BAY)) = pAmmoBay;

	Safe_AddRef(pEngine);
	Safe_AddRef(pTurret);
	Safe_AddRef(pGun);
	Safe_AddRef(pTrackLeft);
	Safe_AddRef(pTrackRight);
	Safe_AddRef(pAmmoBay);

	return S_OK;
}

void CTank::Move(_float fTimeDelta)
{
	CEngine* pEngin = static_cast<CEngine*>(m_Modules.at(ENUM_CLASS(MODULE::ENGINE))); //static_cast<CEngine*>(Find_PartObject(TEXT("Part_Engine")));

	if (!pEngin->Get_isOn())
		return;

	CTrack* pTrackLeft = static_cast<CTrack*>(m_Modules.at(ENUM_CLASS(MODULE::TRACK_LEFT))); //static_cast<CTrack*>(Find_PartObject(TEXT("Part_TrackLeft")));
	CTrack* pTrackRight = static_cast<CTrack*>(m_Modules.at(ENUM_CLASS(MODULE::TRACK_RIGHT))); //static_cast<CTrack*>(Find_PartObject(TEXT("Part_TrackRight")));

	pTrackLeft->Set_Speed(0.f);
	pTrackRight->Set_Speed(0.f);
	//m_fTurnDirection = 0.f;

	_float fMovePower = { pEngin->Get_MovePower() };
	_float fRPMPower = { pEngin->Get_RPM() };

	if (fRPMPower > 1.f) // rpm 최솟값이 1.f
	{
		static_cast<CGun*>(m_Modules[ENUM_CLASS(MODULE::GUN)])->Set_AngleDegree_Max();
	}

	_float SpeedTrackLeft = 0.f;
	_float SpeedTrackRight = 0.f;

#pragma message ("트랙 돌리는 거 메시지 따로 트랙에서 처리해야겠다.")
	if (m_pGameInstance->Get_ID() == m_iID)
	{
		if (m_pGameInstance->Key_Pressing(DIK_W))
		{
			SpeedTrackLeft += -fRPMPower;
			SpeedTrackRight += -fRPMPower;

			if (m_pGameInstance->Key_Pressing(DIK_A))
			{
				SpeedTrackRight += -fRPMPower;
			}
			else if (m_pGameInstance->Key_Pressing(DIK_D))
			{
				SpeedTrackLeft += -fRPMPower;
			}
		}
		else if (m_pGameInstance->Key_Pressing(DIK_S))
		{
			SpeedTrackLeft += fRPMPower;
			SpeedTrackRight += fRPMPower;

			if (m_pGameInstance->Key_Pressing(DIK_A))
			{
				SpeedTrackRight += fRPMPower;
			}
			else if (m_pGameInstance->Key_Pressing(DIK_D))
			{
				SpeedTrackLeft += fRPMPower;
			}
		}
		else
		{
			if (m_pGameInstance->Key_Pressing(DIK_A))
			{
				SpeedTrackLeft += fRPMPower * 0.3f;
				SpeedTrackRight += -fRPMPower;
			}
			else if (m_pGameInstance->Key_Pressing(DIK_D))
			{
				SpeedTrackLeft += -fRPMPower;
				SpeedTrackRight += fRPMPower * 0.3f;
			}
		}
	}
	
	_float fTurnSpeed = pEngin->Get_TurnPower();

	//if (abs(fTurnSpeed) > 0.f)
	//{

	//}

	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTurnSpeed * fTimeDelta, m_pColliderCom, 0.01f);

	//if(pEngin->Get_Gear() != GEAR::LEFT && pEngin->Get_Gear() != GEAR::RIGHT)
	if(fMovePower > 0.f)
		m_pTransformCom->Go_Straight(fMovePower * fTimeDelta, m_pColliderCom, 0.5f);
	else
		m_pTransformCom->Go_Backward(-fMovePower * fTimeDelta, m_pColliderCom, 0.5f);

	pTrackLeft->Set_Speed(SpeedTrackLeft);
	pTrackRight->Set_Speed(SpeedTrackRight);
}

void CTank::Destroyed()
{
	if (m_bisTankDestroyed)
		return;

	m_bisTankDestroyed = true;

	if (m_pGameInstance->Get_ID() == m_iID)
	{
		CDamagePanel* pDamagePanel = static_cast<CDamagePanel*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_DamagePanel")));
		if (pDamagePanel == nullptr)
			return;
		pDamagePanel->Play_Voice_Destroyed();
	}

	CGameObject* pScore = m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Score"));
	if (pScore)
	{
		CGameObject* pPlayerTank = m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_PlayerTank"));
		if (nullptr == pPlayerTank)
			return;

		TEAM ePlayerTeam = static_cast<CTank*>(pPlayerTank)->Get_Team();

		if (m_eTeam == ePlayerTeam)
			static_cast<CScore*>(pScore)->Set_Destroy_Green();
		else
			static_cast<CScore*>(pScore)->Set_Destroy_Red();
	}

	for(CModule * pModule : m_Modules)
	{
		if (pModule)
			pModule->Set_ModuleState(MODULE_STATE::DESTROYED);
	}
}

void CTank::ApplyRecoil(_float fTimeDelta)
{
	if (m_fRecoilTime <= 0.f)
		return;

	m_fRecoilTime -= fTimeDelta * 0.5f;

	_float fProgress = 1.f - (m_fRecoilTime / m_fMaxRecoilTime);
	_float fOscillation = sinf(fProgress * XM_PI * 3.f); // 1.5회 진동
	_float fDamping = (1.f - fProgress); // 감쇠
	_float fRecoilStrength = XMConvertToRadians(1.5f); // 최대 회전 각도

	// 회전 각도 계산
	_float fAngle = fOscillation * fDamping * fRecoilStrength;

	// 포탑의 Right 벡터 가져오기
	_vector vTurretRight = Find_PartObject(TEXT("Part_Turret"))->Get_CombinedWorldMatrix().r[0];
	vTurretRight = XMVector3Normalize(vTurretRight);

	// 탱크 본체의 방향 정보
	_vector vRight = m_pTransformCom->Get_State(STATE::RIGHT);
	_vector vLook = m_pTransformCom->Get_State(STATE::LOOK);
	_vector vUp = m_pTransformCom->Get_State(STATE::UP);

	// 회전 축: 포탑의 Right
	_matrix matRot = XMMatrixRotationAxis(vTurretRight, -fAngle); // 마이너스로 뒤로 꿀렁이기

	// 본체 기준 회전 적용
	vLook = XMVector3TransformNormal(vLook, matRot);
	vUp = XMVector3TransformNormal(vUp, matRot);

	// 정규화 및 직교화
	vLook = XMVector3Normalize(vLook);
	vUp = XMVector3Normalize(vUp);
	vRight = XMVector3Normalize(XMVector3Cross(vUp, vLook));

	// 본체에 적용
	m_pTransformCom->Set_State(STATE::RIGHT, vRight);
	m_pTransformCom->Set_State(STATE::UP, vUp);
	m_pTransformCom->Set_State(STATE::LOOK, vLook);
}

void CTank::SendMatrixSync(_float fTimeDelta)
{
	if (m_pGameInstance->Get_ID() == m_iID && m_pGameInstance->Get_NewLevel_Index() != ENUM_CLASS(LEVEL::PRACTICE))
	{
		m_fTimeAcc += fTimeDelta;
		if (m_fTimeAcc > m_fSyncInterval)
		{
			m_fTimeAcc = 0.f;

			MATRIX_DESC Desc{};
			Desc.iID = m_pGameInstance->Get_ID();
			XMStoreFloat4x4(&Desc.Matrix, m_pTransformCom->Get_WorldMatrix());

			m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_MATRIX_BODY), &Desc);
		}
	}
}

//void CTank::Picked_Ray_ScreenCenter()
//{
//	_float fDist = { 0 };
//
//	_bool bisColl = false;
//	bisColl = m_pColliderCom->Intersect_Ray(XMVectorSetW(XMLoadFloat3(&m_pGameInstance->Get_ScreenCenterPos()), 1.f),
//		XMVectorSetW(XMLoadFloat3(&m_pGameInstance->Get_ScreenCenterRay()), 1.f),
//		fDist);
//
//	if (bisColl)
//	{
//		_float3 vPickedPos = {};
//		_vector vOrigin = { XMLoadFloat3(&m_pGameInstance->Get_ScreenCenterPos()) };
//		_vector vDir = { XMLoadFloat3(&m_pGameInstance->Get_ScreenCenterRay()) };
//
//		XMStoreFloat3(&vPickedPos, vOrigin + vDir * fDist);
//
//		//여기서 픽된 포즈를 계산해서 올리자
//		CPickedManager* pPickedManager = static_cast<CPickedManager*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_PickedManager")));
//		if (pPickedManager)
//			pPickedManager->Add_ScreenCenterPickedPos(vPickedPos);
//	}
//
//}
//
//void CTank::Picked_Ray_Gun()
//{
//	_float fDist = { 0 };
//
//	_bool bisColl = false;
//
//	//포구 피킹
//	CGameObject* pPlayerTank = m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_PlayerTank"));
//	if (nullptr == pPlayerTank)
//		return;
//
//	CGameObject* pGun = pPlayerTank->Find_PartObject(TEXT("Part_Turret"))->Find_PartObject(TEXT("Part_Gun"));
//	if (nullptr == pGun)
//		return;
//
//	//카메라 위치르 가져와서, 현재 저장된 포즈의 거리와
//	_vector vGunPos = pGun->Get_CombinedWorldMatrix().r[3];
//	_vector vGunLook = pGun->Get_CombinedWorldMatrix().r[2]; //단위 벡터로 전달 안해서 터진듯?
//	vGunLook = XMVector3Normalize(vGunLook);
//
//	bisColl = m_pColliderCom->Intersect_Ray(vGunPos, vGunLook, fDist);
//
//	if (bisColl)
//	{
//		_float3 vPickedPos = {};
//		XMStoreFloat3(&vPickedPos, vGunPos + vGunLook * fDist);
//
//		//여기서 픽된 포즈를 계산해서 올리자
//		CPickedManager* pPickedManager = static_cast<CPickedManager*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_PickedManager")));
//		if (pPickedManager)
//			pPickedManager->Add_GunPickedPos(vPickedPos);
//	}
//}

HRESULT CTank::Ready_Components()
{
	/* For.Com_Sound_TankSound3D */
	//if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController_TankSound3D"),
	//	TEXT("Com_Sound_TankSound3D"), reinterpret_cast<CComponent**>(&m_pSoundCom_TankSound3D))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CTank::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vBaseColor", &m_vBodyColor, sizeof(_float4))))
		return E_FAIL;

	//격납고 레벨이라면 실시간으로 게임매니저에서, 색깔을 가져와서 바인딩한다.
	if(m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::HANGER))
	{
		CGameManager* pGameManager = GET_GAMEMANAGER;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_vBaseColor", &pGameManager->Get_BodyColor(), sizeof(_float4))))
			return E_FAIL;
	}

	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_Light(0);

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}



void CTank::Free()
{
	__super::Free();

	Safe_Release(m_pBoundary);
	Safe_Release(m_pColliderCom);
	//Safe_Release(m_pSoundCom_TankSound3D);

	for (CModule* pModule : m_Modules)
		Safe_Release(pModule);
	m_Modules.clear();

}

