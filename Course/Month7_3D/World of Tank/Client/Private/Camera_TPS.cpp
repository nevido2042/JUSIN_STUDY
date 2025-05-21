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
	// Ÿ�� ��ġ
	_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(STATE::POSITION);

	// Ÿ���� Look ���� (����ȭ)
	_vector vLookDir = m_pTarget->Get_Transform()->Get_State(STATE::LOOK);
	vLookDir = XMVector3Normalize(vLookDir);

	// ���� �� ����
	_vector vWorldUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	// Ÿ���� Right ����
	_vector vRight = XMVector3Normalize(XMVector3Cross(vWorldUp, vLookDir));

	// Ÿ���� Up ����
	_vector vUp = XMVector3Cross(vLookDir, vRight);

	// �⺻ �Ÿ� �� ����
	_float fDistance = 20.f;
	_float fHeight = 10.f;

	// ������ ��� (�ڷ� fDistance��ŭ, ���� fHeight��ŭ)
	_vector vOffset = (-vLookDir * fDistance) + (vWorldUp * fHeight);

	// ī�޶� ��ġ = Ÿ�� ��ġ + ������
	_vector vCamPos = vTargetPos + vOffset;

	// ��ġ ����
	m_pTransformCom->Set_State(STATE::POSITION, vCamPos);

	// ī�޶� ���� ���� ���
	_vector vLook = XMVector3Normalize(vTargetPos - vCamPos);
	vRight = XMVector3Normalize(XMVector3Cross(vWorldUp, vLook));
	vUp = XMVector3Cross(vLook, vRight);

	// ���� ���� ����
	m_pTransformCom->Set_State(STATE::RIGHT, vRight);
	m_pTransformCom->Set_State(STATE::UP, vUp);
	m_pTransformCom->Set_State(STATE::LOOK, vLook);

	// ��/�������� ����
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
