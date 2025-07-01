#include "Gun.h"

#include "GameInstance.h"
#include "Terrain.h"
#include "GameManager.h"
#include "PickedManager.h"
#include "Shell.h"
#include "AimCircle.h"
#include "DamagePanel.h"
#include "Icon_Module.h"
#include "Tank.h"

CGun::CGun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CModule(pDevice, pContext)
{
}

CGun::CGun(const CGun& Prototype)
	:CModule(Prototype)
{
}

HRESULT CGun::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	GUN_DESC* pGunDesc = static_cast<GUN_DESC*>(pArg);
	m_vBaseColor = pGunDesc->vBaseColor;

	return S_OK;
}

void CGun::Priority_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_CollisionGroup(ENUM_CLASS(COLLISION_GROUP::MODULE), this, TEXT("Com_Collider"));

	Input(fTimeDelta);

	if (m_pGameInstance->Get_ID() != m_iID && m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::GAMEPLAY))
	{
		// 회전 축 (x축)
		_vector vAxis = XMVectorSet(1.f, 0.f, 0.f, 0.f);

		// 현재 Look 벡터 가져오기
		_vector vLook = m_pTransformCom->Get_State(STATE::LOOK);
		vLook = XMVector3Normalize(vLook);

		// Pitch = Look 벡터와 세계 기준 (XZ 평면)의 각도
		// 여기선 Y값을 기준으로 판단 가능
		_float fDotY = XMVectorGetY(vLook); // = sin(pitch)

		// 상향 회전 시 제한
		if (m_bUp && fDotY <= m_fMaxPitch)
			m_pTransformCom->Turn(vAxis, -fTimeDelta/* * m_fRotateSpeed*/);
		// 하향 회전 시 제한
		else if (m_bDown && fDotY >= m_fMinPitch)
			m_pTransformCom->Turn(vAxis, fTimeDelta/* * m_fRotateSpeed*/);
	}
	
}

void CGun::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_ID() == m_iID && m_pGameInstance->Get_NewLevel_Index() != ENUM_CLASS(LEVEL::PRACTICE))
	{
		m_fTimeAcc += fTimeDelta;
		if (m_fTimeAcc > m_fSyncInterval)
		{
			m_fTimeAcc = 0.f;

			MATRIX_DESC Desc{};
			Desc.iID = m_iID;
			XMStoreFloat4x4(&Desc.Matrix, m_pTransformCom->Get_WorldMatrix());

			m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_MATRIX_GUN), &Desc);
		}
	}

	//부모의 월드 행렬을 가져와서 자신의 월드 행렬과 곱해준다.
	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixMultiply(m_pTransformCom->Get_WorldMatrix(), XMLoadFloat4x4(m_pParentWorldMatrix)));

	m_pColliderCom->Update(XMLoadFloat4x4(&m_CombinedWorldMatrix));

	m_fAngleDegree -= fTimeDelta * 2.f;

	if (m_eModuleState == MODULE_STATE::FUNCTIONAL)
	{
		if (m_fAngleDegree < m_fAngleDegree_Min)
			m_fAngleDegree = m_fAngleDegree_Min;
	}
	else
	{
		if (m_fAngleDegree < m_fAngleDegree_Min * 3.f)
			m_fAngleDegree = m_fAngleDegree_Min * 3.f;
	}

	if (m_pGameInstance->Get_ID() == m_iID)
	{
		Picking();

		CGameObject* pAimCircle = m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_AimCircle"));
		if (pAimCircle)
		{
			static_cast<CAimCircle*>(pAimCircle)->Set_AimRadius(m_fAngleDegree * 0.01f);
			//cout << m_fAngleDegree << endl;
		}
	}
}

void CGun::Late_Update(_float fTimeDelta)
{
	// 위치만 추출
	_vector vPos = XMVectorSet(
		m_CombinedWorldMatrix._41,
		m_CombinedWorldMatrix._42,
		m_CombinedWorldMatrix._43,
		1.f
	);

	// 프러스텀 안에 있으면 렌더링 추가
	if (m_pGameInstance->Is_In_Frustum(vPos, 2.f) && m_pOwner->Get_isDie() == false && m_pOwner->Get_isVisible())
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);


}

HRESULT CGun:: Render()
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

void CGun::On_RaycastHit(CGameObject* pOther)
{
	CModule::On_RaycastHit(pOther);
}

void CGun::Set_ModuleState(MODULE_STATE eState)
{
	m_eModuleState = eState;

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

		CIcon_Module* pIcon = static_cast<CIcon_Module*>(pDamagePanel->Find_PartObject(TEXT("Part_Gun")));
		if (pIcon == nullptr)
			return;

		pIcon->Set_ModuleState(m_eModuleState);
	}
}

// 랜덤 원뿔 퍼짐 함수
_vector CGun::GetRandomSpreadDirection(_fvector vLookDir, _float fMaxAngleDegree)
{
	_vector vDir = XMVector3Normalize(vLookDir);  // 복사 후 사용

	// 최대 회전 각도 (라디안)
	_float fMaxAngleRad = XMConvertToRadians(fMaxAngleDegree);

	// 1. 무작위 회전 축 생성 (룩벡터에 수직인 벡터)
	_vector vRandom = XMVectorSet(
		(rand() % 2001 - 1000) / 1000.f,
		(rand() % 2001 - 1000) / 1000.f,
		(rand() % 2001 - 1000) / 1000.f,
		0.f
	);
	_vector RotAxis = XMVector3Normalize(XMVector3Cross(vDir, vRandom));

	// 2. 무작위 각도 생성 (0 ~ fMaxAngleRad)
	_float fAngle = ((rand() % 1001) / 1000.f) * fMaxAngleRad;

	// 3. 쿼터니언으로 회전
	_vector qRot = XMQuaternionRotationAxis(RotAxis, fAngle);
	_vector vRotatedDir = XMVector3Rotate(vDir, qRot);

	return XMVector3Normalize(vRotatedDir); // 정규화해서 반환
}

HRESULT CGun::Fire()
{
	if (m_eModuleState == MODULE_STATE::DESTROYED)
		return E_FAIL;

	CShell::SHELL_DESC Desc = {};
	memcpy(&Desc.vInitPosition, m_CombinedWorldMatrix.m[3], sizeof(_float3));
	memcpy(&Desc.vVelocity, m_CombinedWorldMatrix.m[2], sizeof(_float3));
	_vector vVelocity = XMLoadFloat3(&Desc.vVelocity);
	vVelocity = XMVector3Normalize(vVelocity);

	vVelocity = GetRandomSpreadDirection(vVelocity, m_fAngleDegree);

	vVelocity = XMVectorScale(vVelocity, 300.f);

	XMStoreFloat3(&Desc.vVelocity, vVelocity);
	Desc.iID = m_iID;
	Desc.iLevelIndex = m_pGameInstance->Get_NewLevel_Index();
	Desc.strLayerTag = TEXT("Layer_Terrain");
	Desc.iIndex = 0;
	Desc.strComponentTag = TEXT("Com_VIBuffer");
	memcpy(&Desc.vFirePos, m_CombinedWorldMatrix.m[3], sizeof(_float3));

	m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Shell"), m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Shell"), &Desc);

	_vector vPos = XMVectorSet(m_CombinedWorldMatrix._41, m_CombinedWorldMatrix._42, m_CombinedWorldMatrix._43, 1.0f);
	m_pSoundCom->Update3DPosition(vPos);

	m_pSoundCom->Play("wpn_1");

	CreateFireEffect();

	return S_OK;
}

void CGun::CreateFireEffect()
{
	// 1. Look 방향 벡터
	_vector vLook = XMVectorSet(m_CombinedWorldMatrix._31, m_CombinedWorldMatrix._32, m_CombinedWorldMatrix._33, m_CombinedWorldMatrix._34);
	vLook = XMVector3Normalize(vLook); // 정규화

	// 2. 살짝 이동시키기
	_float fOffset = 5.f;
	_vector vLookOffset = XMVectorScale(vLook, fOffset);

	// 3. 현재 위치
	_vector vGunPos = XMVectorSet(m_CombinedWorldMatrix._41, m_CombinedWorldMatrix._42, m_CombinedWorldMatrix._43, m_CombinedWorldMatrix._44);
	_vector vEffectPos = vGunPos + vLookOffset;

	// 4. GAMEOBJECT_DESC 세팅
	GAMEOBJECT_DESC Desc{};
	XMStoreFloat3(&Desc.vInitPosition, vEffectPos);

	m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_FireEffect"), m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_FireEffect"), &Desc);

}

void CGun::Input(_float fTimeDelta)
{

	if (m_pGameInstance->Mouse_Pressing(ENUM_CLASS(DIMK::RBUTTON)))
		return;

	if (m_pGameInstance->Get_ID() == m_iID)
	{
#pragma region 주포 상하 회전

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

		// 방향 벡터 (월드 공간)
		_vector vToPicked = XMLoadFloat3(&vPicked) - XMLoadFloat3(&vMyPos);

		// 내 Up, Look, Right 추출
		_vector vUp = XMVector3Normalize(XMLoadFloat3((_float3*)&m_CombinedWorldMatrix.m[1]));
		_vector vLook = XMVector3Normalize(XMLoadFloat3((_float3*)&m_CombinedWorldMatrix.m[2]));
		_vector vRight = XMVector3Normalize(XMLoadFloat3((_float3*)&m_CombinedWorldMatrix.m[0]));

		// Look-Up 평면에 투영 = Right 축 제거
		_vector vProjected = vToPicked - XMVector3Dot(vToPicked, vRight) * vRight;  // vToPicked의 Right 성분 제거

		vProjected = XMVector3Normalize(vProjected);

		// Up 벡터와 비교해 위/아래 판단
		_float fUpDot = XMVectorGetX(XMVector3Dot(vProjected, -vUp));

		// 회전 축 (x축)
		_vector vAxis = XMVectorSet(1.f, 0.f, 0.f, 0.f);

		// 현재 Look 벡터 가져오기
		_vector vLocalLook = m_pTransformCom->Get_State(STATE::LOOK);
		vLocalLook = XMVector3Normalize(vLocalLook);

		// Pitch = Look 벡터와 세계 기준 (XZ 평면)의 각도
		// 여기선 Y값을 기준으로 판단 가능
		_float fDotY = XMVectorGetY(vLocalLook); // = sin(pitch)

		if (fUpDot > 0.01f) //하로
		{
			if (m_bDown == false)
			{
				m_bDown = true;
				m_bUp = false;

				if (m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::GAMEPLAY))
				{
					BOOL_DESC Desc{};
					Desc.iID = m_iID;
					Desc.bBool = true;
					m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_DOWN), &Desc);
				}
			}
			if (fDotY >= m_fMinPitch)
			{
				Set_AngleDegree_Max();
				m_pTransformCom->Turn(vAxis, fTimeDelta /** m_fRotateSpeed*//** abs(fUpDot)*/);
			}
		}
		else if (fUpDot < -0.01f) //상으로
		{
			if (m_bUp == false)
			{
				m_bUp = true;
				m_bDown = false;

				if (m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::GAMEPLAY))
				{
					BOOL_DESC Desc{};
					Desc.iID = m_iID;
					Desc.bBool = true;
					m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_UP), &Desc);
				}

			}
			if (fDotY <= m_fMaxPitch)
			{
				m_pTransformCom->Turn(vAxis, -fTimeDelta/* * m_fRotateSpeed *//** abs(fUpDot)*/);
				Set_AngleDegree_Max();
			}
		}
		else
		{
			// m_CombinedWorldMatrix에서 Right, Look 벡터 추출
			_vector vUp = XMVector3Normalize(XMLoadFloat3((_float3*)&m_CombinedWorldMatrix.m[1]));
			_vector vLook = XMVector3Normalize(XMLoadFloat3((_float3*)&m_CombinedWorldMatrix.m[2]));

			// 목표 방향 (Up 평면에 투영된 방향)
			vProjected = XMVector3Normalize(vProjected);

			// 두 벡터 사이의 각도 구하기 (Dot)
			_float fDot = XMVectorGetX(XMVector3Dot(vLook, vProjected));
			fDot = clamp(fDot, -1.f, 1.f);
			_float fAngle = asinf(fDot); // 라디안

			// 회전 방향 판단 (Y축 기준, Right 벡터와 목표 방향 Dot)
			_float fSign = XMVectorGetX(XMVector3Dot(vUp, vProjected));

			if (fabsf(fAngle) > 0.001f) // 아주 작은 각도 무시
			{
				_float fRotateAngle = fAngle;
				if (fSign > 0)
					fRotateAngle = -fRotateAngle;

				// 제한된 속도로 회전 (필요시 m_fRotateSpeed 곱할 것)
				_float fRotationSpeed = 0.01f; // 필요시 조절
				_float fDeltaAngle = min(fTimeDelta * fRotationSpeed, fabsf(fRotateAngle));
				if (fRotateAngle < 0)
					fDeltaAngle = -fDeltaAngle;

				_vector vAxis = XMVectorSet(1.f, 0.f, 0.f, 0.f);

				m_pTransformCom->Turn(vAxis, fDeltaAngle);
			}

			if (m_bUp || m_bDown)
			{
				m_bUp = m_bDown = false;

				if (m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::GAMEPLAY))
				{
					BOOL_DESC Desc{};
					Desc.iID = m_iID;
					Desc.bBool = false;
					m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_UP), &Desc);
					Desc.bBool = false;
					m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_DOWN), &Desc);
				}
			}
		}
#pragma endregion
	}
}

void CGun::Picking()
{
	_float fMinDist = { FLT_MAX };
	CGameObject* pHitClosest = { nullptr };

	_float fDist = {};
	_vector vOrigin = XMLoadFloat4x4(&m_CombinedWorldMatrix).r[3];
	_vector vRayDir = XMLoadFloat4x4(&m_CombinedWorldMatrix).r[2];
	vRayDir = XMVector3Normalize(vRayDir);
	CGameObject* pHit = { nullptr };

	pHit = m_pGameInstance->Check_RaycastHit(ENUM_CLASS(COLLISION_GROUP::BUILDING), TEXT("Com_Collider"), vOrigin, vRayDir, fDist);
	if (pHit)
	{
		if (fMinDist > fDist)
		{
			fMinDist = fDist;
			pHitClosest = pHit;
		}
	}

	pHit = m_pGameInstance->Check_RaycastHit(ENUM_CLASS(COLLISION_GROUP::WALL), TEXT("Com_Collider"), vOrigin, vRayDir, fDist);
	if (pHit)
	{
		if (fMinDist > fDist)
		{
			fMinDist = fDist;
			pHitClosest = pHit;
		}
	}

	pHit = m_pGameInstance->Check_RaycastHit(ENUM_CLASS(COLLISION_GROUP::BODY), TEXT("Com_Collider"), vOrigin, vRayDir, fDist, m_pGameInstance->Get_ID());
	if (pHit)
	{
		if (fMinDist > fDist)
		{
			fMinDist = fDist;
			pHitClosest = pHit;
		}
	}

	pHit = m_pGameInstance->Check_RaycastHit(ENUM_CLASS(COLLISION_GROUP::TURRET), TEXT("Com_Collider"), vOrigin, vRayDir, fDist, m_pGameInstance->Get_ID());
	if (pHit)
	{
		if (fMinDist > fDist)
		{
			fMinDist = fDist;
			pHitClosest = pHit;
		}
	}

	if (pHitClosest)
	{
		_float3 vPickedPos = {};
		XMStoreFloat3(&vPickedPos, vOrigin + vRayDir * fMinDist);

		//여기서 픽된 포즈를 계산해서 올리자
		CPickedManager* pPickedManager = static_cast<CPickedManager*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_PickedManager")));
		if (pPickedManager)
			pPickedManager->Add_GunPickedPos(vPickedPos);
	}

	CTerrain* pTerrain = static_cast<CTerrain*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Terrain")));
	if (pTerrain)
	{
		if (pTerrain->Pick(vOrigin, vRayDir, fDist))
		{
			_float3 vPickedPos = {};
			XMStoreFloat3(&vPickedPos, vOrigin + vRayDir * fDist);
			CPickedManager* pPickedManager = static_cast<CPickedManager*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_PickedManager")));
			if (pPickedManager)
				pPickedManager->Add_GunPickedPos(vPickedPos);
		}
	}

}

HRESULT CGun::Bind_ShaderResources()
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

		if (FAILED(m_pShaderCom->Bind_RawValue("g_vBaseColor", &pGameManager->Get_GunColor(), sizeof(_float4))))
			return E_FAIL;
	}

	return S_OK;
}

void CGun::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pSoundCom);
}
