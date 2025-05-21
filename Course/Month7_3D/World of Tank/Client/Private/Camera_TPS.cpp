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
	pDesc->fFar = 40000.f;
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

void CCamera_TPS::Priority_Update(_float fTimeDelta)
{
	// 타겟 위치
	_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(STATE::POSITION);

	// 타겟의 Look 방향 (정규화)
	_vector vLookDir = m_pTarget->Get_Transform()->Get_State(STATE::LOOK);
	vLookDir = XMVector3Normalize(vLookDir);

	// 월드 업 벡터
	_vector vWorldUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	// 타겟의 Right 벡터
	_vector vRight = XMVector3Normalize(XMVector3Cross(vWorldUp, vLookDir));

	// 타겟의 Up 벡터
	_vector vUp = XMVector3Cross(vLookDir, vRight);

	// 기본 거리 및 높이
	_float fDistance = 20.f;
	_float fHeight = 10.f;

	// 오프셋 계산 (뒤로 fDistance만큼, 위로 fHeight만큼)
	_vector vOffset = (-vLookDir * fDistance) + (vWorldUp * fHeight);

	// 카메라 위치 = 타겟 위치 + 오프셋
	_vector vCamPos = vTargetPos + vOffset;

	// 위치 설정
	m_pTransformCom->Set_State(STATE::POSITION, vCamPos);

	// 카메라 방향 벡터 계산
	_vector vLook = XMVector3Normalize(vTargetPos - vCamPos);
	vRight = XMVector3Normalize(XMVector3Cross(vWorldUp, vLook));
	vUp = XMVector3Cross(vLook, vRight);

	// 방향 벡터 설정
	m_pTransformCom->Set_State(STATE::RIGHT, vRight);
	m_pTransformCom->Set_State(STATE::UP, vUp);
	m_pTransformCom->Set_State(STATE::LOOK, vLook);

	// 뷰/프로젝션 갱신
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
	Safe_Release(m_pTarget);
}
