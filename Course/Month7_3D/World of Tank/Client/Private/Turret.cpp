#include "Turret.h"

#include "GameInstance.h"

#include "GameManager.h"
#include "PickedManager.h"
#include "DamagePanel.h"
#include "Icon_Module.h"
#include "Tank.h"
#include "Gun.h"

CTurret::CTurret(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CModule(pDevice, pContext)
{
}

CTurret::CTurret(const CTurret& Prototype)
	:CModule(Prototype)
{
}

HRESULT CTurret::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	TURRET_DESC* pDesc = static_cast<TURRET_DESC*>(pArg);
	m_vBaseColor = pDesc->vBaseColor;
	m_eCustom3D = pDesc->e3DCustom;

	return S_OK;
}

void CTurret::Priority_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_CollisionGroup(ENUM_CLASS(COLLISION_GROUP::TURRET), this, TEXT("Com_Collider"));

	Input(fTimeDelta);

	if (m_pGameInstance->Get_ID() != m_iID && m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::GAMEPLAY))
	{
		if (m_bLeft)
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -fTimeDelta * m_fRotateSpeed);
		else if (m_bRight)
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * m_fRotateSpeed);
		else if (!m_bLeft && !m_bRight)
		{
			//int a = 10;
		}
	}

	CGameObject::Priority_Update(fTimeDelta);
}

void CTurret::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_ID() == m_iID && GetForegroundWindow() == g_hWnd && m_pGameInstance->Get_NewLevel_Index() != ENUM_CLASS(LEVEL::PRACTICE))
	{
		m_fTimeAcc += fTimeDelta;
		if (m_fTimeAcc > m_fSyncInterval)
		{
			m_fTimeAcc = 0.f;

			MATRIX_DESC Desc{};
			Desc.iID = m_iID;
			XMStoreFloat4x4(&Desc.Matrix, m_pTransformCom->Get_WorldMatrix());

			m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_MATRIX_TURRET), &Desc);
		}
	}

	//부모의 월드 행렬을 가져와서 자신의 월드 행렬과 곱해준다.
	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixMultiply(m_pTransformCom->Get_WorldMatrix(), XMLoadFloat4x4(m_pParentWorldMatrix)));

	m_pColliderCom->Update(XMLoadFloat4x4(&m_CombinedWorldMatrix));

	CGameObject::Update(fTimeDelta);

}

void CTurret::Late_Update(_float fTimeDelta)
{
	// 위치만 추출
	_vector vPos = XMVectorSet(
		m_CombinedWorldMatrix._41,
		m_CombinedWorldMatrix._42,
		m_CombinedWorldMatrix._43,
		1.f
	);

	if (m_pOwner->Get_IsPicked() && m_pGameInstance->Is_In_Frustum(vPos, 5.f))
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_OUTLINE, this);

	if (m_pGameInstance->Is_In_Frustum(vPos, 5.f) && m_pOwner->Get_isVisible())
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_SHADOW, this);

	// 프러스텀 안에 있으면 렌더링 추가
	if (m_pGameInstance->Is_In_Frustum(vPos, 5.f) && m_pOwner->Get_isDie() == false && m_pOwner->Get_isVisible())
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);

#ifdef _DEBUG
	//m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif

	CGameObject::Late_Update(fTimeDelta);
}

HRESULT CTurret::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

#pragma region 외곽선
	//if (m_pModelCom && m_pOwner->Get_IsPicked())
	//{
	//	_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

	//	for (_uint i = 0; i < iNumMesh; i++)
	//	{
	//		if (FAILED(m_pShaderCom->Begin(3)))
	//			return E_FAIL;

	//		if (FAILED(m_pModelCom->Render(i)))
	//			return E_FAIL;
	//	}
	//}
#pragma endregion

	if (m_pModelCom)
	{
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

#ifdef _DEBUG
	m_pColliderCom->Render();
#endif
	return S_OK;
}

HRESULT CTurret::Render_Outline()
{
	//파트 오브젝트는 자기 트랜스폼 안써야한다
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMesh; i++)
	{
		if (FAILED(m_pShaderCom->Begin(3)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CTurret::Render_Shadow()
{
	//파트 오브젝트는 자기 트랜스폼 안써야한다
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Light_ViewMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Light_ProjMatrix())))
		return E_FAIL;

	_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMesh; i++)
	{
		if (FAILED(m_pShaderCom->Begin(4)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

void CTurret::On_RaycastHit(CGameObject* pOther)
{
	CModule::On_RaycastHit(pOther);
}


void CTurret::Set_ModuleState(MODULE_STATE eState)
{
	m_eModuleState = eState;

	switch (m_eModuleState)
	{
	case MODULE_STATE::DESTROYED:
		//m_pTransformCom->Set_RotationPerSec(0.f);
		m_fRotateSpeed = 0.f;
		break;
	case MODULE_STATE::DAMAGED:
		//m_pTransformCom->Set_RotationPerSec(0.5f);
		m_fRotateSpeed = 0.5f;
		break;
	case MODULE_STATE::FUNCTIONAL:
		//m_pTransformCom->Set_RotationPerSec(1.f);
		m_fRotateSpeed = 1.f;
		break;
	case MODULE_STATE::END:
		break;
	default:
		break;
	}

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

		CIcon_Module* pIcon = static_cast<CIcon_Module*>(pDamagePanel->Find_PartObject(TEXT("Part_TurretRotator")));
		if (pIcon == nullptr)
			return;

		pIcon->Set_ModuleState(m_eModuleState);
	}

}

void CTurret::Input(_float fTimeDelta)
{
	if (m_pGameInstance->Mouse_Pressing(ENUM_CLASS(DIMK::RBUTTON)))
	{
		if (m_bLeft || m_bRight)
		{
			m_bLeft = m_bRight = false;

			if (m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::GAMEPLAY))
			{
				BOOL_DESC Desc{};
				Desc.iID = m_iID;
				Desc.bBool = false;
				m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_LEFT), &Desc);
				Desc.bBool = false;
				m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_RIGHT), &Desc);
			}
		}

		return;
	}

	if (m_pGameInstance->Get_ID() == m_iID)
	{

#pragma region 포탑 좌우 회전
		// 내 위치
		_float3 vMyPos = {
			m_CombinedWorldMatrix.m[3][0],
			m_CombinedWorldMatrix.m[3][1],
			m_CombinedWorldMatrix.m[3][2]
		};

		// Pick된 위치
		_float3 vPicked = {};
		CPickedManager* pPickedManager = static_cast<CPickedManager*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_PickedManager")));
		if (pPickedManager)
			vPicked = pPickedManager->Get_ScreenCenterPickedPos();

		// 방향 벡터
		_vector vToPicked = XMLoadFloat3(&vPicked) - XMLoadFloat3(&vMyPos);

		// 내 기준 벡터
		_vector vLook = XMVector3Normalize(XMLoadFloat3((_float3*)&m_CombinedWorldMatrix.m[2]));
		_vector vUp = XMVector3Normalize(XMLoadFloat3((_float3*)&m_CombinedWorldMatrix.m[1]));
		_vector vRight = XMVector3Normalize(XMLoadFloat3((_float3*)&m_CombinedWorldMatrix.m[0]));

		// Up-Look 평면에 투영 = Up 축 제거 (Yaw 기준 평면)
		_vector vProjected = vToPicked - XMVector3Dot(vToPicked, vUp) * vUp;

		vProjected = XMVector3Normalize(vProjected);

		// Look 기준으로 방향 비교 (좌우)
		_float fRightDot = XMVectorGetX(XMVector3Dot(vRight, vProjected));

		// 회전 축 (y축)
		_vector vAxis = XMVectorSet(0.f, 1.f, 0.f, 0.f);

		if (fRightDot > 0.01f) // 오른쪽 회전
		{
			if (m_bRight == false)
			{
				m_bRight = true;
				m_bLeft = false;

				if (m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::GAMEPLAY))
				{
					BOOL_DESC Desc{};
					Desc.iID = m_iID;
					Desc.bBool = true;
					m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_RIGHT), &Desc);
				}
			}

			//주포 명중률 하락
			CModule* pModule = m_pOwner->Find_Module(MODULE::GUN);
			if (pModule)
			{
				CGun* pGun = static_cast<CGun*>(pModule);
				if (pGun)
					pGun->Set_AngleDegree_Max();
			}

			m_pTransformCom->Turn(vAxis, fTimeDelta * m_fRotateSpeed);
		}
		else if (fRightDot < -0.01f) // 왼쪽 회전
		{
			if (m_bLeft == false)
			{
				m_bLeft = true;
				m_bRight = false;

				if (m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::GAMEPLAY))
				{
					BOOL_DESC Desc{};
					Desc.iID = m_iID;
					Desc.bBool = true;
					m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_LEFT), &Desc);
				}
			}

			//주포 명중률 하락
			CModule* pModule = m_pOwner->Find_Module(MODULE::GUN);
			if (pModule)
			{
				CGun* pGun = static_cast<CGun*>(pModule);
				if (pGun)
					pGun->Set_AngleDegree_Max();
			}

			m_pTransformCom->Turn(vAxis, -fTimeDelta * m_fRotateSpeed);
		}
		else // 정면
		{
			// 터렛이 pickedPos를 정확히 바라보도록 회전 조정 (Yaw 방향만 고려)
			// 목표 방향 (Up 축 평면에 투영한 방향) - 이미 vProjected에 있음
			// 현재 터렛의 Look 방향 (Yaw 평면)
			_vector vLook = XMVector3Normalize(XMLoadFloat3((_float3*)&m_CombinedWorldMatrix.m[2]));

			// Y축 회전 행렬을 통해 Look 방향을 vProjected 방향으로 천천히 맞춤
			// 두 벡터 사이의 각도 구하기
			_float fDot = XMVectorGetX(XMVector3Dot(vLook, vProjected));
			fDot = clamp(fDot, -1.0f, 1.0f);
			_float fAngle = acosf(fDot); // 라디안

			// 회전 방향 판단 (Y축 기준)
			_vector vRight = XMVector3Normalize(XMLoadFloat3((_float3*)&m_CombinedWorldMatrix.m[0]));
			_float fSign = XMVectorGetX(XMVector3Dot(vRight, vProjected));

			if (fabsf(fAngle) > 0.001f) // 아주 작은 각도는 무시
			{
				_float fRotateAngle = fAngle;

				if (fSign < 0)
					fRotateAngle = -fRotateAngle;

				// 제한된 속도로 회전
				_float fRotationSpeed = 0.01f; // 필요시 조절
				_float fDeltaAngle = min(fRotationSpeed * fTimeDelta, fabsf(fRotateAngle));

				if (fRotateAngle < 0)
					fDeltaAngle = -fDeltaAngle;

				_vector vAxis = XMVectorSet(0.f, 1.f, 0.f, 0.f);
				m_pTransformCom->Turn(vAxis, fDeltaAngle);
			}

			if (m_bLeft || m_bRight)
			{
				m_bLeft = m_bRight = false;

				if (m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::GAMEPLAY))
				{
					BOOL_DESC Desc{};
					Desc.iID = m_iID;
					Desc.bBool = false;
					m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_LEFT), &Desc);
					Desc.bBool = false;
					m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_RIGHT), &Desc);
				}
			}
		}
#pragma endregion
	}

}

HRESULT CTurret::Bind_ShaderResources()
{
	//파트 오브젝트는 자기 트랜스폼 안써야한다
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
	//	return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vBaseColor", &m_vBaseColor, sizeof(_float4))))
		return E_FAIL;

	//격납고 레벨이라면 실시간으로 게임매니저에서, 색깔을 가져와서 바인딩한다.
	if (m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::HANGER))
	{
		CGameManager* pGameManager = GET_GAMEMANAGER;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_vBaseColor", &pGameManager->Get_TurretColor(), sizeof(_float4))))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CTurret::Ready_PartObjects()
{
	/* 3D커스텀 파츠를 추가한다. */
	CGameObject::GAMEOBJECT_DESC Desc{};
	Desc.pParentWorldMatrix = &m_CombinedWorldMatrix;
	Desc.iID = m_iID;
	Desc.vInitPosition = _float3(0.f, 3.f, 0.f);
	lstrcpy(Desc.szName, TEXT("3DCustom"));
	Desc.bVisible = false;

	//격납고 일 때는 3D 커스텀 파츠 다 추가하고 껏다 키자
	if(m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::HANGER))
	{
		if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Monkey"), TEXT("Part_Monkey"), &Desc)))
			return E_FAIL;

		if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DragonHead"), TEXT("Part_DragonHead"), &Desc)))
			return E_FAIL;

		if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Drum"), TEXT("Part_Drum"), &Desc)))
			return E_FAIL;

		CGameManager* pGameManager = GET_GAMEMANAGER;
		switch (pGameManager->Get_3DCustom())
		{
		case CUSTOM3D::MONKEY:
			Find_PartObject(TEXT("Part_Monkey"))->Set_Visible(true);
			break;
		case CUSTOM3D::DRAGONHEAD:
			Find_PartObject(TEXT("Part_DragonHead"))->Set_Visible(true);
			break;
		case CUSTOM3D::DRUM:
			Find_PartObject(TEXT("Part_Drum"))->Set_Visible(true);
			break;
		}
	}

	Desc.bVisible = true;
	switch (m_eCustom3D)
	{
	case CUSTOM3D::MONKEY:
		if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Monkey"), TEXT("Part_3DCustom"), &Desc)))
			return E_FAIL;
		break;
	case CUSTOM3D::DRAGONHEAD:
		if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DragonHead"), TEXT("Part_3DCustom"), &Desc)))
			return E_FAIL;
		break;
	case CUSTOM3D::DRUM:
		if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Drum"), TEXT("Part_3DCustom"), &Desc)))
			return E_FAIL;
		break;
	}
	

	return S_OK;
}

void CTurret::Free()
{
	__super::Free();

	for (auto& Pair : m_PartObjects)
		Safe_Release(Pair.second);
	m_PartObjects.clear();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);

}
