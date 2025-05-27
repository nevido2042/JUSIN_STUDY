#include "Camera_FPS.h"
#include "GameInstance.h"

CCamera_FPS::CCamera_FPS(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera{ pDevice, pContext }
{

}

CCamera_FPS::CCamera_FPS(const CCamera_FPS& Prototype)
	: CCamera(Prototype)
{

}

HRESULT CCamera_FPS::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_FPS::Initialize(void* pArg)
{
	CAMERA_FPS_DESC* pDesc = static_cast<CAMERA_FPS_DESC*>(pArg);
	pDesc->vEye = _float3(0.f, 20.f, -15.f);
	pDesc->vAt = _float3(0.f, 0.f, 0.f);
	pDesc->fFov = XMConvertToRadians(60.0f);
	pDesc->fNear = 0.1f;
	pDesc->fFar = 400.f;
	pDesc->fRotationPerSec = XMConvertToRadians(0.0f);
	pDesc->fSpeedPerSec = 0.0f;
	lstrcpy(pDesc->szName, TEXT("Camera"));

	m_fSensor = pDesc->fSensor;
	m_pTarget = pDesc->pTarget;
	Safe_AddRef(m_pTarget);

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	return S_OK;
}

void CCamera_FPS::Priority_Update(_float fTimeDelta)
{
	if (GetForegroundWindow() == g_hWnd)
	{
		// 좌우 공전 (Yaw)
		m_fYaw += XMConvertToRadians(2.f) * m_pGameInstance->Get_DIMMoveState(DIMM::X) * m_fSensor;
		// 상하 공전 (Pitch)
		m_fPitch += XMConvertToRadians(-2.f) * m_pGameInstance->Get_DIMMoveState(DIMM::Y) * m_fSensor;
	}

#pragma message ("이동 방향을 넣으란거 같은데 일딴 뺌, 도플러 효과 주라는 듯")
	m_pGameInstance->Set_Listener_Position(m_pTransformCom, _float3{});
}

void CCamera_FPS::Update(_float fTimeDelta)
{

}

void CCamera_FPS::Late_Update(_float fTimeDelta)
{
	// Pitch 각도 제한
	m_fPitch = max(XMConvertToRadians(-20.f), min(XMConvertToRadians(85.f), m_fPitch));

	// 구면 좌표계 기반 카메라 방향 계산 (카메라 회전 적용)
	_vector vLook = XMVector3Normalize(XMVectorSet(
		cosf(m_fPitch) * sinf(m_fYaw),
		sinf(m_fPitch),
		cosf(m_fPitch) * cosf(m_fYaw),
		0.f
	));

	_vector vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook));
	_vector vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));


	_vector vTargetPos = m_pTarget->Get_CombinedWorldMatrix().r[3];
	vTargetPos = XMVectorAdd(vTargetPos, XMVectorSet(0.f, 2.f, 0.f, 0.f)); // 머리 위치 정도

	// 카메라 위치 설정
	m_pTransformCom->Set_State(STATE::POSITION, vTargetPos);

	// 회전 방향 설정
	m_pTransformCom->Set_State(STATE::LOOK, vLook);
	m_pTransformCom->Set_State(STATE::RIGHT, vRight);
	m_pTransformCom->Set_State(STATE::UP, vUp);

	m_pGameInstance->Set_Listener_Position(m_pTransformCom, _float3{});

	// 뷰/프로젝션 갱신
	__super::Bind_Matrices();

#pragma message ("계속 끄는거 맘에안들지만 일단")
	CGameObject* pGameObject = m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_PlayerTank"));
	if( nullptr == pGameObject )
		return;

	pGameObject->Set_Visible(false);
}

HRESULT CCamera_FPS::Render()
{

	return S_OK;
}

CCamera_FPS* CCamera_FPS::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_FPS* pInstance = new CCamera_FPS(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_FPS::Clone(void* pArg)
{
	CCamera_FPS* pInstance = new CCamera_FPS(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_FPS");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_FPS::Free()
{
	__super::Free();
	Safe_Release(m_pTarget);

}
