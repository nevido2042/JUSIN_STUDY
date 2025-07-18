#include "Camera_TPS.h"
#include "GameInstance.h"
#include "PickedManager.h"
#include "Terrain.h"
#include "Tank.h"
#include "Module.h"

CCamera_TPS::CCamera_TPS(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera{ pDevice, pContext }
{

}

CCamera_TPS::CCamera_TPS(const CCamera_TPS& Prototype)
	: CCamera(Prototype)
{

}

HRESULT CCamera_TPS::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_TPS::Initialize(void* pArg)
{
	CAMERA_TPS_DESC* pDesc = static_cast<CAMERA_TPS_DESC*>(pArg);
	pDesc->vEye = _float3(0.f, 20.f, -15.f);
	pDesc->vAt = _float3(0.f, 0.f, 0.f);
	pDesc->fFov = XMConvertToRadians(BASE_FOV);
	pDesc->fNear = 0.1f;
	pDesc->fFar = CAMERA_FAR;
	pDesc->fRotationPerSec = XMConvertToRadians(0.0f);
	pDesc->fSpeedPerSec = 0.0f;
	//lstrcpy(pDesc->szName, TEXT("Camera"));

	m_fSensor = pDesc->fSensor;
	m_pTarget = pDesc->pTarget;
	Safe_AddRef(m_pTarget);

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	return S_OK;
}

void CCamera_TPS::Priority_Update(_float fTimeDelta)
{
	// 좌우 공전 (Yaw)
	m_fYaw += XMConvertToRadians(-2.f) * m_pGameInstance->Get_DIMMoveState(DIMM::X) * m_fSensor;
	// 상하 공전 (Pitch)
	m_fPitch += XMConvertToRadians(2.f) * m_pGameInstance->Get_DIMMoveState(DIMM::Y) * m_fSensor;
	// Pitch 각도 제한
	m_fPitch = max(XMConvertToRadians(-40.f), min(XMConvertToRadians(85.f), m_fPitch));

#pragma message ("이동 방향을 넣으란거 같은데 일딴 뺌, 도플러 효과 주라는 듯")
	m_pGameInstance->Set_Listener_Position(m_pTransformCom, _float3{});




	
}

void CCamera_TPS::Update(_float fTimeDelta)
{
#pragma region 카메라 움직임
	_float fDistance = m_fArmLength;

	// 타겟 위치
	_vector vTargetPos = m_pTarget->Get_CombinedWorldMatrix().r[3];
	// Y축을 살짝 올림 (예: 머리 위 3.0f만큼)
	vTargetPos.m128_f32[1] += 5.f;

	// 구면좌표계 기반 오프셋 계산
	_vector vOffset = XMVectorSet(
		fDistance * cosf(m_fPitch) * cosf(m_fYaw),
		fDistance * sinf(m_fPitch),
		fDistance * cosf(m_fPitch) * sinf(m_fYaw),
		0.f
	);

	// 카메라 위치
	_vector vCamPos = vTargetPos + vOffset;
	m_pTransformCom->Set_State(STATE::POSITION, vCamPos);

	// 카메라 방향 계산
	_vector vLook = XMVector3Normalize(vTargetPos - vCamPos);
	_vector vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook));
	_vector vUp = XMVector3Cross(vLook, vRight);

	// 방향 설정
	m_pTransformCom->Set_State(STATE::RIGHT, vRight);
	m_pTransformCom->Set_State(STATE::UP, vUp);
	m_pTransformCom->Set_State(STATE::LOOK, vLook);

#pragma region 스프링 암
	//타겟에서 자신의 위치로 레이를 쏴서 레이쏜 위치로 변경
	_float fDist = {};
	_vector vRayDir = vCamPos - vTargetPos;
	vRayDir = XMVector3Normalize(vRayDir);
	CGameObject* pHit = m_pGameInstance->Check_RaycastHit(ENUM_CLASS(COLLISION_GROUP::BUILDING), TEXT("Com_Collider"), vTargetPos, vRayDir, fDist);

	if (pHit != nullptr)
	{
		if (fDist < fDistance/*카메라 암의 길이보다 짧으면*/)
		{
			fDistance = fDist;
			m_pTransformCom->Set_State(STATE::POSITION, vTargetPos + vRayDir * fDist * 0.9f);
		}
	}

	CTerrain* pTerrain = static_cast<CTerrain*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Terrain")));
	if (pTerrain)
	{
		if (pTerrain->Pick(vTargetPos, vRayDir, fDist))
		{
			_float3 vPickedPos = {};
			XMStoreFloat3(&vPickedPos, vTargetPos + vRayDir * fDist);

			if (fDist < fDistance/*카메라 암의 길이보다 짧으면*/)
			{
				fDistance = fDist;
				m_pTransformCom->Set_State(STATE::POSITION, vTargetPos + vRayDir * fDist * 0.9f);
			}
		}
	}

#pragma endregion

#pragma region 카메라 쉐이크 
	if (m_fShakeTime > 0.f)
	{
		m_fShakeTime -= fTimeDelta;

		_vector vPos = m_pTransformCom->Get_State(STATE::POSITION);

		// 감쇠 (시간에 따라 줄어드는 진폭)
		_float fAttenuation = m_fShakeTime / m_fShakeTime;
		_float fCurrentPower = m_fShakePower * fAttenuation;

		// 랜덤 오프셋
		_float fX = m_pGameInstance->Compute_Random(-1.f, 1.f);
		_float fY = m_pGameInstance->Compute_Random(-1.f, 1.f);
		_float fZ = m_pGameInstance->Compute_Random(-1.f, 1.f);

		_vector vOffset = XMVectorSet(fX, fY, fZ, 0.f) * fCurrentPower;
		//XMVectorSetW(XMLoadFloat3(&m_vShakeOriginalPos), 1.f)
		// 흔들린 위치 적용
		m_pTransformCom->Set_State(STATE::POSITION, m_pTransformCom->Get_State(STATE::POSITION) + vOffset);
	}
#pragma endregion


	// 뷰/프로젝션 갱신
	__super::Bind_Matrices();
#pragma endregion

	Picking();

	// 커서 숨기기
	if (GetForegroundWindow() == g_hWnd)
	{
		while (ShowCursor(FALSE) >= 0); // 카운트가 0보다 작아질 때까지 반복
		Clip_Cursor();
	}
}

void CCamera_TPS::Late_Update(_float fTimeDelta)
{

#pragma message ("계속 켜는거 맘에안들지만 일단")
	CGameObject* pGameObject = m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_PlayerTank"));
	if (nullptr == pGameObject)
		return;

	pGameObject->Set_Visible(true);
}

HRESULT CCamera_TPS::Render()
{

	return S_OK;
}

void CCamera_TPS::Start_CameraShake(_float fDuration, _float fPower)
{
	m_fShakeTime = fDuration;
	m_fShakePower = fPower;
}

void CCamera_TPS::Picking()
{
	_float fMinDist = { FLT_MAX };
	CGameObject* pHitClosest = { nullptr };

	_float fDist = { };
	_vector vOrigin = m_pTransformCom->Get_State(STATE::POSITION);
	_vector vRayDir = m_pTransformCom->Get_State(STATE::LOOK);

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

			static_cast<CTank*>(pHit)->Set_IsPicked(true);
		}
	}

	pHit = m_pGameInstance->Check_RaycastHit(ENUM_CLASS(COLLISION_GROUP::TURRET), TEXT("Com_Collider"), vOrigin, vRayDir, fDist, m_pGameInstance->Get_ID());
	if (pHit)
	{
		if (fMinDist > fDist)
		{
			fMinDist = fDist;
			pHitClosest = pHit;

			static_cast<CModule*>(pHit)->Get_Owner()->Set_IsPicked(true);
		}
	}

	if (pHitClosest)
	{
		_float3 vPos = {};
		XMStoreFloat3(&vPos, vOrigin + vRayDir * fMinDist);

		//여기서 픽된 포즈를 계산해서 올리자
		CPickedManager* pPickedManager = static_cast<CPickedManager*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_PickedManager")));
		if (pPickedManager)
			pPickedManager->Add_ScreenCenterPickedPos(vPos);
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
				pPickedManager->Add_ScreenCenterPickedPos(vPickedPos);
		}
	}
}

CCamera_TPS* CCamera_TPS::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_TPS* pInstance = new CCamera_TPS(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_TPS::Clone(void* pArg)
{
	CCamera_TPS* pInstance = new CCamera_TPS(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_TPS");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_TPS::Free()
{
	__super::Free();
	Safe_Release(m_pTarget);
}
