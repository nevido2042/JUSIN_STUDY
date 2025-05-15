#include "Camera_TPS.h"
#include "GameInstance.h"

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
	pDesc->fFov = XMConvertToRadians(60.0f);
	pDesc->fNear = 0.1f;
	pDesc->fFar = 800.f;
	pDesc->fRotationPerSec = XMConvertToRadians(180.0f);
	pDesc->fSpeedPerSec = 50.0f;
	lstrcpy(pDesc->szName, TEXT("Camera"));

	m_fSensor = pDesc->fSensor;
	m_pTarget = pDesc->pTarget;

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	return S_OK;
}

void CCamera_TPS::Priority_Update(_float fTimeDelta)
{
#pragma message ("카메라 수정해야함: static")
	// Orbit 회전용 변수 (Yaw: 좌우, Pitch: 상하)
	static _float fYaw = 0.f;
	static _float fPitch = 0.f;

	_int MouseMoveX = m_pGameInstance->Get_DIMMoveState(DIMM::X);
	_int MouseMoveY = m_pGameInstance->Get_DIMMoveState(DIMM::Y);

	fYaw += MouseMoveX * fTimeDelta * m_fSensor;
	fPitch += MouseMoveY * fTimeDelta * m_fSensor;

	// Pitch 제한
	fPitch = max(-XM_PIDIV2 + 0.1f, min(XM_PIDIV2 - 0.1f, fPitch));

	// 타겟 위치
	_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(STATE::POSITION);

	// 기본 거리 (오프셋 크기)
	_float fDistance = 20.f;

	// ----------------------
	// Yaw 회전 (World Up 기준)
	_matrix matRotYaw = XMMatrixRotationY(fYaw);

	// Pitch 회전 축 : Yaw 적용 후의 Local Right 벡터
	_vector vRight = XMVector3TransformNormal(XMVectorSet(1.f, 0.f, 0.f, 0.f), matRotYaw);

	// Pitch 회전 (Local Right 기준)
	_matrix matRotPitch = XMMatrixRotationAxis(vRight, fPitch);

	// ----------------------
	// 오프셋 적용
	_vector vDefaultOffset = XMVectorSet(0.f, 5.f, -fDistance, 0.f);
	_vector vRotatedOffset = XMVector3TransformCoord(vDefaultOffset, matRotPitch * matRotYaw);

	// 카메라 위치 = 타겟 위치 + 오프셋
	_vector vCamPos = vTargetPos + vRotatedOffset;

	// 카메라 위치 설정
	m_pTransformCom->Set_State(STATE::POSITION, vCamPos);

	// Look 벡터 계산 (타겟을 바라봄)
	_vector vLook = XMVector3Normalize(vTargetPos - vCamPos);

	// Right, Up 벡터 계산
	_vector vWorldUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	vRight = XMVector3Normalize(XMVector3Cross(vWorldUp, vLook));
	_vector vUp = XMVector3Cross(vLook, vRight);

	// 방향 벡터 세팅
	m_pTransformCom->Set_State(STATE::RIGHT, vRight);
	m_pTransformCom->Set_State(STATE::UP, vUp);
	m_pTransformCom->Set_State(STATE::LOOK, vLook);

	// 뷰, 프로젝션 갱신
	__super::Bind_Matrices();
}

void CCamera_TPS::Update(_float fTimeDelta)
{
	POSITION_DESC Desc;
	XMStoreFloat3(&Desc.vPos, m_pTransformCom->Get_State(STATE::POSITION));

	m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_POSITION), &Desc);
}

void CCamera_TPS::Late_Update(_float fTimeDelta)
{

}

HRESULT CCamera_TPS::Render()
{

	return S_OK;
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


}
