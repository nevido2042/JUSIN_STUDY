#include "Tank.h"

#include "Turret.h"
#include "Gun.h"
#include "Track.h"
#include "GameInstance.h"
#include "Engine.h"
#include "DamageBar.h"

#pragma region UI
#include "Icon_Engine.h"
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
	LANDOBJECT_DESC* pDesc = static_cast<LANDOBJECT_DESC*>(pArg);
	//(*pDesc).fRotationPerSec = 0.1f;
	//(*pDesc).fSpeedPerSec = 0.5f;
	(*pDesc).iLevelIndex = m_pGameInstance->Get_NewLevel_Index();
	(*pDesc).strLayerTag = TEXT("Layer_Terrain");
	(*pDesc).strComponentTag = TEXT("Com_VIBuffer");
	(*pDesc).iIndex = 0;

	m_iID = pDesc->iID;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pSoundCom->SetVolume(0.1f);

	return S_OK;
}

void CTank::Priority_Update(_float fTimeDelta)
{
	Input();

	Move(fTimeDelta);

	CLandObject::SetUp_Height_Normal(m_pTransformCom, fTimeDelta, 0.5f);

	// 반동 적용
	#pragma message ("FPS 카메라 반동 주는 것이 방향에 따라 달라진다 해결 못함(원본 겜도 못해서 안넣은거일거야)")
	ApplyRecoil(fTimeDelta);

	CGameObject::Priority_Update(fTimeDelta);
}

void CTank::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_ID() == m_iID && GetForegroundWindow() == g_hWnd && m_pGameInstance->Get_NewLevel_Index() != ENUM_CLASS(LEVEL::PRACTICE))
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

	CGameObject::Update(fTimeDelta);
}

void CTank::Late_Update(_float fTimeDelta)
{
	if (!m_bVisible)
	{
		return;
	}

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);

	if (m_bDestroyed)
		return;

	CGameObject::Late_Update(fTimeDelta);

}


HRESULT CTank::Render()
{

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (m_pModelCom && !m_bDestroyed)
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

	return S_OK;
}

void CTank::Input()
{
	if (GetForegroundWindow() != g_hWnd)
		return;

	if (m_pGameInstance->Get_NewLevel_Index() != ENUM_CLASS(LEVEL::PRACTICE) && m_pGameInstance->Get_ID() != m_iID)
		return;

	if (m_pGameInstance->Key_Down(DIK_F12))
		Destroyed();

	if (m_pGameInstance->Key_Down(DIK_F1))
		Set_State_Engine(MODULE_STATE::FUNCTIONAL);

	if (m_pGameInstance->Key_Down(DIK_F2))
	{
		Take_Damage(5.f);
		Set_State_Engine(MODULE_STATE::DAMAGED);
	}


	if (m_pGameInstance->Key_Down(DIK_F3))
		Set_State_Engine(MODULE_STATE::DESTROYED);

	if (m_pGameInstance->Mouse_Down(ENUM_CLASS(DIMK::LBUTTON)))
	{
		if (m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::GAMEPLAY))
		{
			PACKET_DESC Desc{};
			Desc.iID = m_iID;
			m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_FIRE), &Desc);
		}

		Try_Fire();
	}
}

void CTank::Try_Fire()
{
	//발사
	CTurret* pTurret = static_cast<CTurret*>(Find_PartObject(TEXT("Part_Turret")));
	CGun* pGun = static_cast<CGun*>(pTurret->Find_PartObject(TEXT("Part_Gun")));

	if (FAILED(pGun->Fire()))
		return;

	//꿀렁임
	m_fRecoilTime = m_fMaxRecoilTime;
}

HRESULT CTank::Take_Damage(_float fDamage)
{
	m_fHP -= fDamage;

	if (m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::PRACTICE) || m_pGameInstance->Get_ID() == m_iID)
	{
		//플레이어 탱크라면 체력바 찾아서 깎아라 && 맞는 소리 (꽝)
		
		CDamageBar* pDamageBar = static_cast<CDamageBar*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_DamageBar")));
		if (nullptr == pDamageBar)
			return E_FAIL;
	
		pDamageBar->Fill(m_fHP / m_fMaxHP);
	}

	return S_OK;
}

void CTank::Move(_float fTimeDelta)
{
	CEngine* pEngin = static_cast<CEngine*>(Find_PartObject(TEXT("Part_Engine")));

	if (!pEngin->Get_isOn())
		return;

	CTrack* pTrackLeft = static_cast<CTrack*>(Find_PartObject(TEXT("Part_TrackLeft")));
	CTrack* pTrackRight = static_cast<CTrack*>(Find_PartObject(TEXT("Part_TrackRight")));

	pTrackLeft->Set_Speed(0.f);
	pTrackRight->Set_Speed(0.f);
	//m_fTurnDirection = 0.f;

	_float fMovePower = { pEngin->Get_MovePower() };
	_float fRPMPower = { pEngin->Get_RPM() };

	_float SpeedTrackLeft = 0.f;
	_float SpeedTrackRight = 0.f;

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

	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), pEngin->Get_TurnPower() * fTimeDelta);

	//if(pEngin->Get_Gear() != GEAR::LEFT && pEngin->Get_Gear() != GEAR::RIGHT)
	m_pTransformCom->Go_Straight(fMovePower * fTimeDelta);

	pTrackLeft->Set_Speed(SpeedTrackLeft);
	pTrackRight->Set_Speed(SpeedTrackRight);
}

void CTank::Destroyed()
{
	m_bDestroyed = true;
}

HRESULT CTank::Set_State_Engine(MODULE_STATE eState)
{
	CIcon_Engine* pIcon = static_cast<CIcon_Engine*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Icon_Engine")));
	if (pIcon == nullptr)
		return E_FAIL;
	pIcon->Set_ModuleState(eState);


	switch (eState)
	{
	case Client::MODULE_STATE::FUNCTIONAL:
		m_pSoundCom->Play("engine_functional_1");
		break;
	case Client::MODULE_STATE::DAMAGED:
		m_pSoundCom->Play("engine_damaged_6");
		break;
	case Client::MODULE_STATE::DESTROYED:
		m_pSoundCom->Play("engine_destroyed_4");
		break;
	case Client::MODULE_STATE::END:
		break;
	default:
		break;
	}


	return S_OK;
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

}

