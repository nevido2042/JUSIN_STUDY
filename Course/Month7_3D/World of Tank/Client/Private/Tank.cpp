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

	//������ ���� ����
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

	//if (m_pBoundary)
	//{
	//	_float3 vPos = {};
	//	XMStoreFloat3(&vPos, m_pTransformCom->Get_State(STATE::POSITION));

	//	if (!m_pBoundary->IsPointInBoundary(vPos))
	//	{
	//		_float3 vClosestPos = m_pBoundary->ClosestPointOnBoundary(vPos);
	//		m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&vClosestPos), 1.f));

	//	}
	//}

	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));

	OnGround(fTimeDelta);

	// �ݵ� ����
#pragma message ("FPS ī�޶� �ݵ� �ִ� ���� ���⿡ ���� �޶����� �ذ� ����(���� �׵� ���ؼ� �ȳ������ϰž�)")
	ApplyRecoil(fTimeDelta);

	SendMatrixSync(fTimeDelta);

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

	if (m_bisDie)
		return;

	CGameObject::Late_Update(fTimeDelta);

}


HRESULT CTank::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (m_pModelCom && !m_bisDie)
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
	//_int i = 0;
}

void CTank::Damage_Module(MODULE eModule, _float fDamage)
{
	if (ENUM_CLASS(eModule) < 0 || ENUM_CLASS(eModule) >= ENUM_CLASS(MODULE::END))
		return;

	m_Modules[ENUM_CLASS(eModule)]->TakeDamage(fDamage);

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

		//���� �Ϸ� ���� Ȯ��
		if (FAILED(pAmmoBay->is_Load_Complete()))
			return E_FAIL;

		//�߻�
		if (FAILED(static_cast<CGun*>(m_Modules.at(ENUM_CLASS(MODULE::GUN)))->Fire()))
			return E_FAIL;

		static_cast<CGun*>(m_Modules[ENUM_CLASS(MODULE::GUN)])->Set_AngleDegree_Max();

		//�ܷ���
		m_fRecoilTime = m_fMaxRecoilTime;

		pAmmoBay->Start_Load();

	}
	else
	{
		//�߻�
		if (FAILED(static_cast<CGun*>(m_Modules.at(ENUM_CLASS(MODULE::GUN)))->Fire()))
			return E_FAIL;

		static_cast<CGun*>(m_Modules[ENUM_CLASS(MODULE::GUN)])->Set_AngleDegree_Max();

		//�ܷ���
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
		//�÷��̾� ��ũ��� ü�¹� ã�Ƽ� ��ƶ� && �´� �Ҹ� (��)
		CGameObject* pDamagePanel = m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_DamagePanel"));

		if (pDamagePanel)
		{
			CDamageBar* pDamageBar = static_cast<CDamageBar*>(pDamagePanel->Find_PartObject(TEXT("Part_DamageBar")));
			if (pDamageBar)
			{
				pDamageBar->Fill(m_fHP / m_fMaxHP);
			}
		}
	}

	static_cast<CDamageBar_World*> (Find_PartObject(TEXT("Part_DamageBar")))->Fill(m_fHP / m_fMaxHP);

	return S_OK;
}

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
#pragma region �븻 ���
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

	// ���� Up ���� ��������
	_vector vCurrentUp = m_pTransformCom->Get_State(STATE::UP);

	// ���� �������� �ε巴�� ��ȭ
	const _float fLerpSpeed = 5.f; // Ŭ���� ������ ������
	vTargetUp = XMVector3Normalize(XMVectorLerp(vCurrentUp, vTargetUp, fLerpSpeed * fTimeDelta));

	// Look ���ʹ� ���� ����
	_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(STATE::LOOK));

	// Up�� Look���κ��� Right ����
	_vector vRight = XMVector3Normalize(XMVector3Cross(vTargetUp, vLook));

	// Right�� Up���� Look ����
	vLook = XMVector3Normalize(XMVector3Cross(vRight, vTargetUp));

	// ���� ���� ����
	m_pTransformCom->Set_State(STATE::RIGHT, vRight);
	m_pTransformCom->Set_State(STATE::UP, vTargetUp);
	m_pTransformCom->Set_State(STATE::LOOK, vLook);

#pragma endregion

#pragma region ���� ���
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

	// ���� ��ġ ����
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
	CEngine* pEngin = static_cast<CEngine*>(m_Modules.at(ENUM_CLASS(MODULE::ENGINE))); //static_cast<CEngine*>(Find_PartObject(TEXT("Part_Engine")));

	if (!pEngin->Get_isOn())
		return;

#pragma message ("Ʈ�� �����°� ���� �̻ڰ� �ϰ�ʹ�. ������ ���")

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

	if (fRPMPower > 1.f) // rpm �ּڰ��� 1.f
	{
		static_cast<CGun*>(m_Modules[ENUM_CLASS(MODULE::GUN)])->Set_AngleDegree_Max();
	}

	_float SpeedTrackLeft = 0.f;
	_float SpeedTrackRight = 0.f;

#pragma message ("Ʈ�� ������ �� �޽��� ���� Ʈ������ ó���ؾ߰ڴ�.")
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

	//�̵��ϱ��� ��ġ ����
	_vector BeforePos = m_pTransformCom->Get_State(STATE::POSITION);


	_float fSpeed_TrackState = { 1.f };
	if (pChassis && static_cast<CModule*>(pChassis)->Get_ModuleState() == MODULE_STATE::FUNCTIONAL)
		fSpeed_TrackState = 1.f;
	else if (pChassis && static_cast<CModule*>(pChassis)->Get_ModuleState() == MODULE_STATE::DAMAGED)
		fSpeed_TrackState = 0.5f;
	else if (pChassis && static_cast<CModule*>(pChassis)->Get_ModuleState() == MODULE_STATE::DESTROYED)
		fSpeed_TrackState = 0.f;
	

	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTurnSpeed * fTimeDelta * fSpeed_TrackState, m_pColliderCom, 0.01f);

	if (fMovePower > 0.f)
		m_pTransformCom->Go_Straight(fMovePower * fTimeDelta * fSpeed_TrackState, m_pColliderCom, 0.5f);
	else
		m_pTransformCom->Go_Backward(-fMovePower * fTimeDelta * fSpeed_TrackState, m_pColliderCom, 0.5f);


	//�ٿ���� �˻��� ��� ���̸� �̵� ���
	//�ƴϸ� ���� ��ġ�� ������
	if (m_pBoundary)
	{
		_float3 vPos = {};
		XMStoreFloat3(&vPos, m_pTransformCom->Get_State(STATE::POSITION));

		if (!m_pBoundary->IsPointInBoundary(vPos))
			m_pTransformCom->Set_State(STATE::POSITION, BeforePos);
	}

	pTrackLeft->Set_Speed(SpeedTrackLeft);
	pTrackRight->Set_Speed(SpeedTrackRight);
}

void CTank::Destroyed()
{
	if (m_bisDie)
		return;

	m_bisDie = true;

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
	_float fOscillation = sinf(fProgress * XM_PI * 3.f); // 1.5ȸ ����
	_float fDamping = (1.f - fProgress); // ����
	_float fRecoilStrength = XMConvertToRadians(0.5f); // �ִ� ȸ�� ����

	// ȸ�� ���� ���
	_float fAngle = fOscillation * fDamping * fRecoilStrength;

	// ��ž�� Right ���� ��������
	_vector vTurretRight = Find_PartObject(TEXT("Part_Turret"))->Get_CombinedWorldMatrix().r[0];
	vTurretRight = XMVector3Normalize(vTurretRight);

	// ��ũ ��ü�� ���� ����
	_vector vRight = m_pTransformCom->Get_State(STATE::RIGHT);
	_vector vLook = m_pTransformCom->Get_State(STATE::LOOK);
	_vector vUp = m_pTransformCom->Get_State(STATE::UP);

	// ȸ�� ��: ��ž�� Right
	_matrix matRot = XMMatrixRotationAxis(vTurretRight, -fAngle); // ���̳ʽ��� �ڷ� �ܷ��̱�

	// ��ü ���� ȸ�� ����
	vLook = XMVector3TransformNormal(vLook, matRot);
	vUp = XMVector3TransformNormal(vUp, matRot);

	// ����ȭ �� ����ȭ
	vLook = XMVector3Normalize(vLook);
	vUp = XMVector3Normalize(vUp);
	vRight = XMVector3Normalize(XMVector3Cross(vUp, vLook));

	// ��ü�� ����
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
	CDamageBar_World::DAMAGEBAR_WORLD_DESC DamageBarWorldDesc = {};
	DamageBarWorldDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	DamageBarWorldDesc.iID = m_iID;
	DamageBarWorldDesc.eTeam = pDesc->eTeam;
	DamageBarWorldDesc.fDepth = DEPTH_BACKGROUND - 0.01f;
	DamageBarWorldDesc.fSizeX = 120.f * UI_RATIO;
	DamageBarWorldDesc.fSizeY = 60.f * UI_RATIO;


	/* ��������_���带 �߰��Ѵ�. */
	lstrcpy(DamageBarWorldDesc.szName, TEXT("DamageBar"));
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DamageBar_World"), TEXT("Part_DamageBar"), &DamageBarWorldDesc)))
		return E_FAIL;

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

	//�ݳ��� �����̶�� �ǽð����� ���ӸŴ�������, ������ �����ͼ� ���ε��Ѵ�.
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

