#include "Turret.h"

#include "GameInstance.h"

#include "GameManager.h"
#include "PickedManager.h"
#include "DamagePanel.h"
#include "Icon_Module.h"
#include "Tank.h"

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

	return S_OK;
}

void CTurret::Priority_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_CollisionGroup(ENUM_CLASS(COLLISION_GROUP::TURRET), this, TEXT("Com_Collider"));

	// 게임플레이 일때는 자신의 엔진충돌은 메시지로부터만 받는다.
	//if (m_pGameInstance->Get_NewLevel_Index() != ENUM_CLASS(LEVEL::GAMEPLAY) || m_pGameInstance->Get_ID() != m_iID)
	//{
	//	m_pGameInstance->Add_CollisionGroup(ENUM_CLASS(COLLISION_GROUP::MODULE), this, TEXT("Com_Collider"));
	//}

	Input(fTimeDelta);

	if (m_pGameInstance->Get_ID() != m_iID && m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::GAMEPLAY))
	{
		if (m_bLeft)
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -fTimeDelta /** m_fRotateSpeed*/);
		else if (m_bRight)
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta /** m_fRotateSpeed*/);
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

	// 게임플레이 일때는 자신의 엔진충돌은 메시지로부터만 받는다.
	if (m_pGameInstance->Get_NewLevel_Index() != ENUM_CLASS(LEVEL::GAMEPLAY) || m_pGameInstance->Get_ID() != m_iID)
	{
		m_pGameInstance->Check_Collision(ENUM_CLASS(COLLISION_GROUP::SHELL), this, TEXT("Com_Collider"), TEXT("Com_Collider"));
	}

	//if (m_pGameInstance->Get_ID() != m_iID)
	//{
	//	Picked_Ray_ScreenCenter();
	//	Picked_Ray_Gun();
	//}

	CGameObject::Update(fTimeDelta);

}

HRESULT CTurret::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (m_pModelCom)
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

#ifdef _DEBUG
	m_pColliderCom->Render();
#endif
	return S_OK;
}

void CTurret::On_Collision_Stay(CGameObject* pOther, _fvector vNormal)
{
	cout << "Collision_Turret" << endl;

	CModule::On_Collision_Stay(pOther, vNormal);
}

void CTurret::Set_ModuleState(MODULE_STATE eState)
{
	m_eModuleState = eState;

	switch (m_eModuleState)
	{
	case MODULE_STATE::DESTROYED:
		m_pTransformCom->Set_RotationPerSec(0.f);
		break;
	case MODULE_STATE::DAMAGED:
		m_pTransformCom->Set_RotationPerSec(0.5f);
		break;
	case MODULE_STATE::FUNCTIONAL:
		m_pTransformCom->Set_RotationPerSec(1.f);
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
			if (!m_pOwner->Get_isTankDestroyed())
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
	//if (GetForegroundWindow() != g_hWnd)
	//	return;

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
		// 타겟 위치
		//CTerrain* pTerrain = static_cast<CTerrain*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Terrain")));
		//if (pTerrain == nullptr)
		//	return;

		_float3 vTargetPos = {};
		CPickedManager* pPickedManager = static_cast<CPickedManager*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_PickedManager")));
		if (pPickedManager)
			vTargetPos = pPickedManager->Get_ScreenCenterPickedPos();

		//_float3 vTargetPos = pTerrain->Get_PickedPos();
		vTargetPos.y = m_CombinedWorldMatrix.m[3][1]; // 자기와 y만 맞추기

		// 내 위치
		_float3 vMyPos = {
			m_CombinedWorldMatrix.m[3][0],
			m_CombinedWorldMatrix.m[3][1],
			m_CombinedWorldMatrix.m[3][2]
		};

		// 방향 벡터
		_vector vToTarget = XMVector3Normalize(XMLoadFloat3(&vTargetPos) - XMLoadFloat3(&vMyPos));
		_vector vRight = XMLoadFloat4(reinterpret_cast<const _float4*>(&m_CombinedWorldMatrix.m[0]));

		// 좌/우 판별
		_float fRightDot = XMVectorGetX(XMVector3Dot(vRight, vToTarget));

		if (fRightDot > 0.01f) //오른쪽으로 돌기
		{
			if (m_bRight == false)
			{
				m_bLeft = false;
				m_bRight = true;

				if (m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::GAMEPLAY))
				{
					BOOL_DESC Desc{};
					Desc.iID = m_iID;
					Desc.bBool = true;
					m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_RIGHT), &Desc);
				}
			}

#pragma message ("델타값이 너무크면 넘어가버려서 바들바들 떤다. ex) 30프레임으로 낮추니까 발견하였음")
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta /** m_fRotateSpeed*//** abs(fRightDot)*/);
		}
		else if (fRightDot < -0.01f) //왼쪽으로 돌기
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

			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -fTimeDelta /** m_fRotateSpeed *//** abs(fRightDot)*/);
		}
		else
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


				// 회전이 멈췄을 때, 정확히 바라보도록 Look을 설정
				// 새로운 Look 벡터 설정
				// 회전 멈출 때 정확히 타겟을 바라보게 Look 보정
				_float3 vScaled = m_pTransformCom->Get_Scaled(); // 현재 스케일 유지
				// 정면 방향 (Look)
				_vector vLook = XMVector3Normalize(vToTarget); // 목표 방향 정규화
				// 오른쪽 방향
				_vector vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook));
				// 위 방향
				_vector vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));
				// 스케일 적용
				vLook *= vScaled.z;
				vRight *= vScaled.x;
				vUp *= vScaled.y;
				// CombinedWorldMatrix에 직접 반영
				XMStoreFloat4(reinterpret_cast<_float4*>(&m_CombinedWorldMatrix.m[0]), vRight);
				XMStoreFloat4(reinterpret_cast<_float4*>(&m_CombinedWorldMatrix.m[1]), vUp);
				XMStoreFloat4(reinterpret_cast<_float4*>(&m_CombinedWorldMatrix.m[2]), vLook);

			}
		}
	}
}

//void CTurret::Picked_Ray_ScreenCenter()
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
//void CTurret::Picked_Ray_Gun()
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
//	_vector vGunLook = pGun->Get_CombinedWorldMatrix().r[2];
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

HRESULT CTurret::Bind_ShaderResources()
{
	//파트 오브젝트는 자기 트랜스폼 안써야한다
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vBaseColor", &m_vBaseColor, sizeof(_float4))))
		return E_FAIL;

	//격납고 레벨이라면 실시간으로 게임매니저에서, 색깔을 가져와서 바인딩한다.
	if (m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::HANGER))
	{
		CGameManager* pGameManager = GET_GAMEMANAGER;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_vBaseColor", &pGameManager->Get_TurretColor(), sizeof(_float4))))
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
