#include "Camera_Hanger.h"
#include "GameInstance.h"
#include "Terrain.h"

CCamera_Hanger::CCamera_Hanger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera{ pDevice, pContext }
{

}

CCamera_Hanger::CCamera_Hanger(const CCamera_Hanger& Prototype)
	: CCamera(Prototype)
{

}

HRESULT CCamera_Hanger::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Hanger::Initialize(void* pArg)
{
	CAMERA_HANGER_DESC* pDesc = static_cast<CAMERA_HANGER_DESC*>(pArg);
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

void CCamera_Hanger::Priority_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Mouse_Pressing(ENUM_CLASS(DIMK::LBUTTON)) == false)
		return;

	// 좌우 공전 (Yaw)
	m_fYaw += XMConvertToRadians(-2.f) * m_pGameInstance->Get_DIMMoveState(DIMM::X) * m_fSensor;
	// 상하 공전 (Pitch)
	m_fPitch += XMConvertToRadians(2.f) * m_pGameInstance->Get_DIMMoveState(DIMM::Y) * m_fSensor;
	// Pitch 각도 제한
	m_fPitch = max(XMConvertToRadians(-40.f), min(XMConvertToRadians(85.f), m_fPitch));

#pragma message ("이동 방향을 넣으란거 같은데 일딴 뺌, 도플러 효과 주라는 듯")
	m_pGameInstance->Set_Listener_Position(m_pTransformCom, _float3{});
}

void CCamera_Hanger::Update(_float fTimeDelta)
{
#pragma region 카메라 움직임
	_float fDistance = m_fArmLength;

	// 타겟 위치
	_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(STATE::POSITION);
	// Y축을 살짝 올림
	vTargetPos.m128_f32[1] += 2.f;

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

	// 뷰/프로젝션 갱신
	__super::Bind_Matrices();
#pragma endregion


	// 커서 다시 보이기
	while (ShowCursor(TRUE) < 0);   // 카운트가 0 이상 될 때까지 반복

	// 커서 이동 제한 해제
	ClipCursor(nullptr);
}

void CCamera_Hanger::Late_Update(_float fTimeDelta)
{
	
}

HRESULT CCamera_Hanger::Render()
{

	return S_OK;
}

CCamera_Hanger* CCamera_Hanger::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Hanger* pInstance = new CCamera_Hanger(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Hanger::Clone(void* pArg)
{
	CCamera_Hanger* pInstance = new CCamera_Hanger(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Hanger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Hanger::Free()
{
	__super::Free();
	Safe_Release(m_pTarget);
}
