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

void CCamera_FPS::Priority_Update(_float fTimeDelta)
{

}

void CCamera_FPS::Update(_float fTimeDelta)
{

}

void CCamera_FPS::Late_Update(_float fTimeDelta)
{
	// Å¸°ÙÀÇ Right
	_vector vTargetRight = m_pTarget->Get_CombinedWorldMatrix().r[0];
	vTargetRight = XMVector3Normalize(vTargetRight);

	// Å¸°ÙÀÇ UP
	_vector vTargetUp = m_pTarget->Get_CombinedWorldMatrix().r[1];
	vTargetUp = XMVector3Normalize(vTargetUp);

	// Å¸°ÙÀÇ Look
	_vector vTargetLook = m_pTarget->Get_CombinedWorldMatrix().r[2];
	vTargetLook = XMVector3Normalize(vTargetLook);

	// Å¸°ÙÀÇ Pos
	_vector vTargetPos = m_pTarget->Get_CombinedWorldMatrix().r[3];
	//vTargetPos = XMVectorAdd(vTargetPos, XMVectorSet(0.f, 2.2863f, 0.f, 0.f));

	// À§Ä¡ ¼³Á¤
	m_pTransformCom->Set_State(STATE::POSITION, vTargetPos);

	// ¹æÇâ º¤ÅÍ ¼³Á¤
	m_pTransformCom->Set_State(STATE::RIGHT, vTargetRight);
	m_pTransformCom->Set_State(STATE::UP, vTargetUp);
	m_pTransformCom->Set_State(STATE::LOOK, vTargetLook);

	m_pGameInstance->Set_Listener_Position(m_pTransformCom, _float3{});

	// ºä/ÇÁ·ÎÁ§¼Ç °»½Å
	__super::Bind_Matrices();

#pragma message ("°è¼Ó ²ô´Â°Å ¸¾¿¡¾ÈµéÁö¸¸ ÀÏ´Ü, Å¸°Ù °Ç")
	m_pTarget->Set_Visible(false);
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
