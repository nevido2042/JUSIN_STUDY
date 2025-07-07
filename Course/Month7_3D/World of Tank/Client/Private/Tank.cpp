#include "Tank.h"

#include "Turret.h"
#include "Gun.h"
#include "Track.h"
#include "GameInstance.h"
#include "Engine.h"
#include "GameManager.h"
#include "AmmoBay.h"
#include "Boundary.h"
#include "Layer.h"
#include "Camera_TPS.h"
#include "Smoke.h"

#pragma region UI
#include "Icon_Module.h"
#include "DamageBar_World.h"
#include "DamageBar.h"
#include "DamagePanel.h"
#include "PickedManager.h"
#include "Score.h"
#include "DamageIndicator.h"
#include "Shell.h"
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

	return S_OK;
}

void CTank::Priority_Update(_float fTimeDelta)
{
	m_bisPicked = false;

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

		if (m_bisDie)
			return;

		Input();
	}

	//연습용 더미 조작
	if (m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::PRACTICE))
	{
		if (m_pGameInstance->Get_ID() != m_iID)
		{
			if (m_pGameInstance->Key_Down(DIK_F))
			{
				if (FAILED(Try_Fire()))
					return;
			}
		}
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
			_float fTerrainHalf = TERRAIN_SIZE * TERRAIN_OFFSET_WIDTH * 0.5f;
			_float3 vMapCenter = { fTerrainHalf , 0.f, fTerrainHalf };
			_float3 vSafePoint = m_pBoundary->SafeInsideBoundary(vClosestPos, vMapCenter, 0.1f);

			m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&vSafePoint), 1.f));

		}
	}

	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));

	OnGround(fTimeDelta);

	// 반동 적용
#pragma message ("FPS 카메라 반동 주는 것이 방향에 따라 달라진다 해결 못함(원본 겜도 못해서 안넣은거일거야)")
	ApplyRecoil(fTimeDelta);

	SendMatrixSync(fTimeDelta);

#pragma region 죽었을 때 루프 이펙트 좀 이따 추가 블렌드이펙트 때문에
	//if (m_bisDie)
	//{
	//	m_fDieTimeAcc += fTimeDelta;

	//	if (m_bIsCreateDieEffect == false && m_fDieTimeAcc > 1.f)
	//	{
	//		m_bIsCreateDieEffect = true;

	//		//몇 초 뒤에 재생하자
	//		//파괴된 전차의 불(루프) 이펙트 재생
	//		CGameObject* pDeadFireEffect = Find_PartObject(TEXT("Part_DeadFireEffect"));
	//		if (pDeadFireEffect)
	//		{
	//			pDeadFireEffect->Set_Active(true);
	//		}

	//		//파괴된 전차의 연기 파티클 재생
	//		CGameObject* pDeadSmoke = Find_PartObject(TEXT("Part_DeadSmoke"));
	//		if (pDeadSmoke)
	//		{
	//			pDeadSmoke->Set_Active(true);
	//		}
	//	}
	//}
#pragma endregion

	CGameObject::Update(fTimeDelta);
}

void CTank::Late_Update(_float fTimeDelta)
{

	if(m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(STATE::POSITION), 2.f))
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);

#ifdef _DEBUG
	//m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif

	CGameObject::Late_Update(fTimeDelta);

}


HRESULT CTank::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (m_pModelCom && !m_bisDie)
	{
#pragma region 외곽선
		if (m_bisPicked)
		{
			_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

			for (_uint i = 0; i < iNumMesh; i++)
			{
				if (FAILED(m_pShaderCom->Begin(3)))
					return E_FAIL;

				if (FAILED(m_pModelCom->Render(i)))
					return E_FAIL;
			}
		}
#pragma endregion

		_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

		for (_uint i = 0; i < iNumMesh; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
				return E_FAIL;
			
			if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS, 0)))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Begin(2)))
				return E_FAIL;

			if (FAILED(m_pModelCom->Render(i)))
				return E_FAIL;
		}
	}
	else if (m_pModelCom_Destroyed)
	{

#pragma region 외곽선
		if (m_bisPicked)
		{
			_uint		iNumMesh = m_pModelCom_Destroyed->Get_NumMeshes();

			for (_uint i = 0; i < iNumMesh; i++)
			{
				if (FAILED(m_pShaderCom->Begin(3)))
					return E_FAIL;

				if (FAILED(m_pModelCom_Destroyed->Render(i)))
					return E_FAIL;
			}
		}
#pragma endregion

		_uint		iNumMesh = m_pModelCom_Destroyed->Get_NumMeshes();

		for (_uint i = 0; i < iNumMesh; i++)
		{
			if (FAILED(m_pModelCom_Destroyed->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
				return E_FAIL;

			if (FAILED(m_pModelCom_Destroyed->Bind_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS, 0)))
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
	//_int i = 0;
}

void CTank::Damage_Module(MODULE eModule)
{
	if (ENUM_CLASS(eModule) < 0 || ENUM_CLASS(eModule) >= ENUM_CLASS(MODULE::END))
		return;

	m_Modules[ENUM_CLASS(eModule)]->DamageModule();

}


HRESULT CTank::Set_State_Module(MODULE eModule, MODULE_STATE eState)
{
	m_Modules[ENUM_CLASS(eModule)]->Set_ModuleState(eState);

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

		static_cast<CGun*>(m_Modules[ENUM_CLASS(MODULE::GUN)])->Set_AngleDegree_Max();

		//꿀렁임
		m_fRecoilTime = m_fMaxRecoilTime;

		pAmmoBay->Start_Load();

		//카메라 셰이크
		CLayer* pLayer = nullptr;
		pLayer = m_pGameInstance->Find_Layer(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Camera"));
		if (pLayer)
		{
			CGameObject* pObject = pLayer->Find_GameObject_By_Name(TEXT("Camera_TPS"));

			if (pObject && pObject->Get_isActive())
			{
				static_cast<CCamera_TPS*>(pObject)->Start_CameraShake(0.1f, 0.5f);
			}
		}

	}
	else
	{
		//발사
		if (FAILED(static_cast<CGun*>(m_Modules.at(ENUM_CLASS(MODULE::GUN)))->Fire()))
			return E_FAIL;

		static_cast<CGun*>(m_Modules[ENUM_CLASS(MODULE::GUN)])->Set_AngleDegree_Max();

		//꿀렁임
		m_fRecoilTime = m_fMaxRecoilTime;

	}

	return S_OK;

}

HRESULT CTank::Take_Damage(_float fDamage, CShell* pShell, _float3 vFirePos)
{
	m_fHP -= fDamage;

	if (m_fHP <= 0.f)
	{
		m_fHP = 0.f;

		Die();
	}

	//네트워크로 받은경우는 메시지 보내면 안됨
	if (pShell)
	{
		//데미지를 받았다는 메시지를 보내자
		TAKEDAMAGE_DESC TakeDamageDesc = {};
		TakeDamageDesc.iID = m_pGameInstance->Get_ID();
		TakeDamageDesc.iTargetID = m_iID;
		TakeDamageDesc.fDamage = fDamage;
		TakeDamageDesc.vFirePos = pShell->Get_FirePos();
		m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_TAKE_DAMAGE), &TakeDamageDesc);
	}

	if (m_pGameInstance->Get_ID() == m_iID)
	{
		//플레이어 탱크라면 체력바 찾아서 깎아라 && 맞는 소리 (꽝)
		CGameObject* pDamagePanel = m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_DamagePanel"));
		if (pDamagePanel)
		{
			//피해 받았다는 소리 데미지 판넬한테 요청
			static_cast<CDamagePanel*>(pDamagePanel)->Play_Sound_TakeDamage();

			CDamageBar* pDamageBar = static_cast<CDamageBar*>(pDamagePanel->Find_PartObject(TEXT("Part_DamageBar")));
			if (pDamageBar)
			{
				pDamageBar->Fill(m_fHP / m_fMaxHP);
				//pDamageBar->Set_Text(m_fHP, m_fMaxHP);
			}
		}


		

		//카메라 셰이크
		CLayer* pLayer = nullptr;
		pLayer = m_pGameInstance->Find_Layer(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Camera"));
		if (pLayer)
		{
			CGameObject* pObject = pLayer->Find_GameObject_By_Name(TEXT("Camera_TPS"));

			if (pObject && pObject->Get_isActive())
			{
				static_cast<CCamera_TPS*>(pObject)->Start_CameraShake(0.1f, 2.f);
			}
		}

		if (m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::PRACTICE) && m_iID == m_pGameInstance->Get_ID())
		{
			//데미지 인디케이터 띄우자 여기서
			CDamageIndicator::DAMAGE_INDICATOR_DESC		Desc{};

			Desc.fSizeX = 248.f * UI_RATIO;
			Desc.fSizeY = 716.f * UI_RATIO;
			Desc.fX = g_iWinSizeX * 0.5f;
			Desc.fY = g_iWinSizeY * 0.5f;
			Desc.fDepth = DEPTH_BACKGROUND - 0.01f;
			Desc.fRotationPerSec = 1.f;

			if (pShell)
			{
				Desc.vFirePos = pShell->Get_FirePos();
			}
			else
			{
				Desc.vFirePos = vFirePos;
			}

			if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DamageIndicator"),
				ENUM_CLASS(LEVEL::PRACTICE), TEXT("Layer_DamageIndicator"), &Desc)))
				return E_FAIL;
		}
	}

	CGameObject* pObject = Find_PartObject(TEXT("Part_DamageBar"));
	if (pObject)
	{
		CDamageBar_World* pWorldDamageBar = static_cast<CDamageBar_World*> (pObject);
		pWorldDamageBar->Fill(m_fHP / m_fMaxHP);
		//pWorldDamageBar->Set_Text(m_fHP, m_fMaxHP);

	}
	return S_OK;
}

//HRESULT CTank::Take_Damage(_float fDamage)
//{
//	m_fHP -= fDamage;
//
//	if (m_fHP <= 0.f)
//	{
//		m_fHP = 0.f;
//
//		Die();
//	}
//
//	//데미지를 받았다는 메시지를 보내자
//
//	//TAKEDAMAGE_DESC TakeDamageDesc = {};
//	//TakeDamageDesc.iID = m_pGameInstance->Get_ID();
//	//TakeDamageDesc.iTargetID = m_iID;
//	//TakeDamageDesc.fDamage = fDamage;
//	//m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_TAKE_DAMAGE), &TakeDamageDesc);
//
//	if (m_pGameInstance->Get_ID() == m_iID)
//	{
//		//플레이어 탱크라면 체력바 찾아서 깎아라 && 맞는 소리 (꽝)
//		CGameObject* pDamagePanel = m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_DamagePanel"));
//
//		if (pDamagePanel)
//		{
//			CDamageBar* pDamageBar = static_cast<CDamageBar*>(pDamagePanel->Find_PartObject(TEXT("Part_DamageBar")));
//			if (pDamageBar)
//			{
//				pDamageBar->Fill(m_fHP / m_fMaxHP);
//				//pDamageBar->Set_Text(m_fHP, m_fMaxHP);
//			}
//		}
//
//		//카메라 셰이크
//		CLayer* pLayer = nullptr;
//		pLayer = m_pGameInstance->Find_Layer(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Camera"));
//		if(pLayer)
//		{
//			CGameObject* pObject = pLayer->Find_GameObject_By_Name(TEXT("Camera_TPS"));
//
//			if (pObject && pObject->Get_isActive())
//			{
//				static_cast<CCamera_TPS*>(pObject)->Start_CameraShake(0.1f, 2.f);
//			}		
//		}
//	}
//
//	CGameObject* pObject = Find_PartObject(TEXT("Part_DamageBar"));
//	if (pObject)
//	{
//		CDamageBar_World* pWorldDamageBar = static_cast<CDamageBar_World*> (pObject);
//		pWorldDamageBar->Fill(m_fHP / m_fMaxHP);
//		//pWorldDamageBar->Set_Text(m_fHP, m_fMaxHP);
//
//	}
//	return S_OK;
//}

void CTank::Repair_All()
{
	if (m_bisDie == true)
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

void CTank::OnGround(_float fTimeDelta)
{
#pragma region 노말 평균
	_vector vLocalPoint[4] =
	{
		XMVectorSet(1.3f, 0.f, 2.f, 1.f),
		XMVectorSet(-1.3f, 0.f, 2.f, 1.f),
		XMVectorSet(1.3f, 0.f, -2.f, 1.f),
		XMVectorSet(-1.3f, 0.f, -2.f, 1.f),
	};

	_vector vNormalSum = XMVectorZero();
	for (_int i = 0; i < 4; ++i)
	{
		vLocalPoint[i] = XMVector3TransformCoord(vLocalPoint[i], m_pTransformCom->Get_WorldMatrix());
		vNormalSum += m_pTargetBuffer->Compute_NormalPosition(vLocalPoint[i]);
	}
	_vector vTargetUp = XMVector3Normalize(vNormalSum / 4.f);

	// 이전 Up 벡터 가져오기
	_vector vCurrentUp = m_pTransformCom->Get_State(STATE::UP);

	// 선형 보간으로 부드럽게 변화
	const _float fLerpSpeed = 5.f; // 클수록 빠르게 보간됨
	vTargetUp = XMVector3Normalize(XMVectorLerp(vCurrentUp, vTargetUp, fLerpSpeed * fTimeDelta));

	// Look 벡터는 이전 유지
	_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(STATE::LOOK));

	// Up과 Look으로부터 Right 재계산
	_vector vRight = XMVector3Normalize(XMVector3Cross(vTargetUp, vLook));

	// Right와 Up으로 Look 재계산
	vLook = XMVector3Normalize(XMVector3Cross(vRight, vTargetUp));

	// 방향 벡터 적용
	m_pTransformCom->Set_State(STATE::RIGHT, vRight);
	m_pTransformCom->Set_State(STATE::UP, vTargetUp);
	m_pTransformCom->Set_State(STATE::LOOK, vLook);

#pragma endregion

#pragma region 높이 평균
	_vector vLocalHeightPoint[2] =
	{
		XMVectorSet(1.3f, 0.f, 0.f, 1.f),
		XMVectorSet(-1.3f, 0.f, 0.f, 1.f),
	};

	_float fHeightSum = {};
	for (_int i = 0; i < 2; ++i)
	{
		vLocalHeightPoint[i] = XMVector3TransformCoord(vLocalHeightPoint[i], m_pTransformCom->Get_WorldMatrix());
		fHeightSum += XMVectorGetY(m_pTargetBuffer->Compute_HeightPosition(vLocalHeightPoint[i]));
	}

	_float fHeight = fHeightSum / 2.f;

	// 최종 위치 설정
	_vector vPosition = m_pTransformCom->Get_State(STATE::POSITION);
	vPosition = XMVectorSetY(vPosition, fHeight + 0.5f);

	m_pTransformCom->Set_State(STATE::POSITION, vPosition);
#pragma endregion
}

HRESULT CTank::Store_Modules()
{
	CModule* pEngine = static_cast<CModule*>(Find_PartObject(TEXT("Part_Engine")));
	CModule* pTurret = static_cast<CModule*>(Find_PartObject(TEXT("Part_Turret")));
	CModule* pGun = static_cast<CModule*>(Find_PartObject(TEXT("Part_Turret"))->Find_PartObject(TEXT("Part_Gun")));
	//CModule* pTrackLeft = static_cast<CModule*>(Find_PartObject(TEXT("Part_TrackLeft")));
	//CModule* pTrackRight = static_cast<CModule*>(Find_PartObject(TEXT("Part_TrackRight")));
	CModule* pChassis = static_cast<CModule*>(Find_PartObject(TEXT("Part_Chassis")));
	CModule* pAmmoBay = static_cast<CModule*>(Find_PartObject(TEXT("Part_AmmoBay")));



	m_Modules.resize(ENUM_CLASS(MODULE::END));
	m_Modules.at(ENUM_CLASS(MODULE::ENGINE)) = pEngine;
	m_Modules.at(ENUM_CLASS(MODULE::TURRET)) = pTurret;
	m_Modules.at(ENUM_CLASS(MODULE::GUN)) = pGun;
	//m_Modules.at(ENUM_CLASS(MODULE::TRACK_LEFT)) = pTrackLeft;
	//m_Modules.at(ENUM_CLASS(MODULE::TRACK_RIGHT)) = pTrackRight;
	m_Modules.at(ENUM_CLASS(MODULE::CHASSIS)) = pChassis;
	m_Modules.at(ENUM_CLASS(MODULE::AMMO_BAY)) = pAmmoBay;

	Safe_AddRef(pEngine);
	Safe_AddRef(pTurret);
	Safe_AddRef(pGun);
	Safe_AddRef(pChassis);
	//Safe_AddRef(pTrackLeft);
	//Safe_AddRef(pTrackRight);
	Safe_AddRef(pAmmoBay);

	return S_OK;
}

void CTank::Move(_float fTimeDelta)
{
	//엔진 연기 흔들림
	CSmoke* pSmokeLeft = static_cast<CSmoke*>(Find_PartObject(TEXT("Part_EngineSmokeParticle_Left")));
	CSmoke* pSmokeRight = static_cast<CSmoke*>(Find_PartObject(TEXT("Part_EngineSmokeParticle_Right")));

	CEngine* pEngin = static_cast<CEngine*>(m_Modules.at(ENUM_CLASS(MODULE::ENGINE))); //static_cast<CEngine*>(Find_PartObject(TEXT("Part_Engine")));

	if (!pEngin->Get_isOn())
		return;

#pragma message ("트랙 돌리는거 구조 이쁘게 하고싶다. 엔진과 엮어서")

	CTrack* pTrackLeft = { nullptr }; //static_cast<CTrack*>(m_Modules.at(ENUM_CLASS(MODULE::CHASSIS))); //static_cast<CTrack*>(Find_PartObject(TEXT("Part_TrackLeft")));
	CTrack* pTrackRight = { nullptr }; //static_cast<CTrack*>(m_Modules.at(ENUM_CLASS(MODULE::TRACK_RIGHT))); //static_cast<CTrack*>(Find_PartObject(TEXT("Part_TrackRight")));

	CGameObject* pChassis = m_Modules.at(ENUM_CLASS(MODULE::CHASSIS));
	if (pChassis)
	{
		pTrackLeft = static_cast<CTrack*>(pChassis->Find_PartObject(TEXT("Part_TrackLeft")));
		pTrackRight = static_cast<CTrack*>(pChassis->Find_PartObject(TEXT("Part_TrackRight")));
	}

	pTrackLeft->Set_Speed(0.f);
	pTrackRight->Set_Speed(0.f);

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

	//이동하기전 위치 저장
	//_vector BeforePos = m_pTransformCom->Get_State(STATE::POSITION);


	_float fSpeed_TrackState = { 1.f };
	if (pChassis && static_cast<CModule*>(pChassis)->Get_ModuleState() == MODULE_STATE::FUNCTIONAL)
		fSpeed_TrackState = 1.f;
	else if (pChassis && static_cast<CModule*>(pChassis)->Get_ModuleState() == MODULE_STATE::DAMAGED)
		fSpeed_TrackState = 0.5f;
	else if (pChassis && static_cast<CModule*>(pChassis)->Get_ModuleState() == MODULE_STATE::DESTROYED)
		fSpeed_TrackState = 0.f;
	

	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTurnSpeed * fTimeDelta * fSpeed_TrackState, m_pColliderCom, 0.01f);

	if (fMovePower > 0.f)
	{
		m_pTransformCom->Go_Straight(fMovePower * fTimeDelta * fSpeed_TrackState, m_pColliderCom, 0.5f);
	}
	else
	{
		m_pTransformCom->Go_Backward(-fMovePower * fTimeDelta * fSpeed_TrackState, m_pColliderCom, 0.5f);
	}

	if (pSmokeLeft && pSmokeRight)
	{
		if (abs(fMovePower) < 1.f && abs(fTurnSpeed) < 1.f)
		{
			//연기 끄고
			pSmokeLeft->Set_Loop(false);
			pSmokeRight->Set_Loop(false);
			pSmokeLeft->Reset_Smoke_Pivot();
			pSmokeRight->Reset_Smoke_Pivot();
		}	
		else
		{
			pSmokeLeft->Set_Loop(true);
			pSmokeRight->Set_Loop(true);

			pSmokeLeft->Add_Smoke_Pivot(fMovePower * fTimeDelta * fSpeed_TrackState);
			pSmokeRight->Add_Smoke_Pivot(fMovePower * fTimeDelta * fSpeed_TrackState);
		}
	}

	//바운더리 검사후 경계 안이면 이동 허용
	//아니면 이전 위치로 돌리기
	/*if (m_pBoundary)
	{
		_float3 vPos = {};
		XMStoreFloat3(&vPos, m_pTransformCom->Get_State(STATE::POSITION));

		if (!m_pBoundary->IsPointInBoundary(vPos))
			m_pTransformCom->Set_State(STATE::POSITION, BeforePos);
	}*/

	pTrackLeft->Set_Speed(SpeedTrackLeft * 0.3f);
	pTrackRight->Set_Speed(SpeedTrackRight * 0.3f);
}

void CTank::Die()
{
	//한 번만 죽도록
	if (m_bisDie)
		return;

	m_bisDie = true;

	//엔진 연기 비활성화
	CGameObject* pRight = Find_PartObject(TEXT("Part_EngineSmokeParticle_Right"));
	CGameObject* pLeft = Find_PartObject(TEXT("Part_EngineSmokeParticle_Left"));
	if (pRight && pLeft)
	{
		pLeft->Set_Active(false);
		pRight->Set_Active(false);
	}
	

	//몇 초 뒤에 재생하자
			//파괴된 전차의 불(루프) 이펙트 재생
	CGameObject* pDeadFireEffect = Find_PartObject(TEXT("Part_DeadFireEffect"));
	if (pDeadFireEffect)
		pDeadFireEffect->Set_Active(true);

	//파괴된 전차의 연기 파티클 재생
	CGameObject* pDeadSmoke = Find_PartObject(TEXT("Part_DeadSmoke"));
	if (pDeadSmoke)
		pDeadSmoke->Set_Active(true);

	//폭발 파티클
	GAMEOBJECT_DESC Desc{};
	XMStoreFloat3(&Desc.vInitPosition, m_pTransformCom->Get_State(STATE::POSITION));
	Desc.vInitPosition.y += 2.f;
	m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_TankExplosionEffect"), m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_TankExplosionEffect"), &Desc);

	
	if (m_pGameInstance->Get_ID() == m_iID)
	{
		//파괴 보이스 재생
		CDamagePanel* pDamagePanel = static_cast<CDamagePanel*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_DamagePanel")));
		if (pDamagePanel == nullptr)
			return;
		pDamagePanel->Play_Voice_Destroyed();
	}

	//점수판 변경
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

	//모든 모듈 파괴
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
	_float fRecoilStrength = XMConvertToRadians(0.5f); // 최대 회전 각도

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

HRESULT CTank::Ready_Components()
{
	/* For.Com_Sound_TankSound3D */
	//if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController_TankSound3D"),
	//	TEXT("Com_Sound_TankSound3D"), reinterpret_cast<CComponent**>(&m_pSoundCom_TankSound3D))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CTank::Ready_PartObjects(TANK_DESC* pDesc)
{
	/* DeadFireEffect 추가한다. */
	GAMEOBJECT_DESC DeadFire{};
	DeadFire.vInitPosition = { 0.f, 2.f, 0.f };
	DeadFire.pParent = this;
	DeadFire.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	DeadFire.bActive = false;
	lstrcpy(DeadFire.szName, TEXT("DeadFireEffect"));
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DeadFireEffect"), TEXT("Part_DeadFireEffect"), &DeadFire)))
		return E_FAIL;


	/* DeadSmoke 파티클을 추가한다. */
	GAMEOBJECT_DESC DeadSmokeDesc{};
	DeadSmokeDesc.pParent = this;
	DeadSmokeDesc.vInitPosition = { 0.f, 2.f, 0.f };
	DeadSmokeDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	DeadSmokeDesc.bActive = false;
	lstrcpy(DeadSmokeDesc.szName, TEXT("DeadSmoke"));
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DeadSmoke"), TEXT("Part_DeadSmoke"), &DeadSmokeDesc)))
		return E_FAIL;

	//플레이어 탱크는 월드 데미지바 없어도 된다.
	if (m_pGameInstance->Get_ID() != m_iID)
	{
		/* 데미지바_월드를 추가한다. */
		CDamageBar_World::DAMAGEBAR_WORLD_DESC DamageBarWorldDesc = {};
		DamageBarWorldDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
		DamageBarWorldDesc.pParent = this;
		DamageBarWorldDesc.iID = m_iID;
		DamageBarWorldDesc.eTeam = pDesc->eTeam;
		DamageBarWorldDesc.fDepth = DEPTH_BACKGROUND - 0.01f;
		DamageBarWorldDesc.fSizeX = 120.f * UI_RATIO;
		DamageBarWorldDesc.fSizeY = 60.f * UI_RATIO;
		lstrcpy(DamageBarWorldDesc.szName, TEXT("DamageBar"));
		if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DamageBar_World"), TEXT("Part_DamageBar"), &DamageBarWorldDesc)))
			return E_FAIL;
	}

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

	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
	//	return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vBaseColor", &m_vBodyColor, sizeof(_float4))))
		return E_FAIL;

	//격납고 레벨이라면 실시간으로 게임매니저에서, 색깔을 가져와서 바인딩한다.
	if(m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::HANGER))
	{
		CGameManager* pGameManager = GET_GAMEMANAGER;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_vBaseColor", &pGameManager->Get_BodyColor(), sizeof(_float4))))
			return E_FAIL;
	}

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

