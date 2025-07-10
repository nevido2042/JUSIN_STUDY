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

	// �¿� ���� (Yaw)
	m_fYaw += XMConvertToRadians(-2.f) * m_pGameInstance->Get_DIMMoveState(DIMM::X) * m_fSensor;
	// ���� ���� (Pitch)
	m_fPitch += XMConvertToRadians(2.f) * m_pGameInstance->Get_DIMMoveState(DIMM::Y) * m_fSensor;
	// Pitch ���� ����
	m_fPitch = max(XMConvertToRadians(-40.f), min(XMConvertToRadians(85.f), m_fPitch));

#pragma message ("�̵� ������ �������� ������ �ϵ� ��, ���÷� ȿ�� �ֶ�� ��")
	m_pGameInstance->Set_Listener_Position(m_pTransformCom, _float3{});
}

void CCamera_Hanger::Update(_float fTimeDelta)
{
#pragma region ī�޶� ������
	_float fDistance = m_fArmLength;

	// Ÿ�� ��ġ
	_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(STATE::POSITION);
	// Y���� ��¦ �ø�
	vTargetPos.m128_f32[1] += 2.f;

	// ������ǥ�� ��� ������ ���
	_vector vOffset = XMVectorSet(
		fDistance * cosf(m_fPitch) * cosf(m_fYaw),
		fDistance * sinf(m_fPitch),
		fDistance * cosf(m_fPitch) * sinf(m_fYaw),
		0.f
	);

	// ī�޶� ��ġ
	_vector vCamPos = vTargetPos + vOffset;
	m_pTransformCom->Set_State(STATE::POSITION, vCamPos);

	// ī�޶� ���� ���
	_vector vLook = XMVector3Normalize(vTargetPos - vCamPos);
	_vector vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook));
	_vector vUp = XMVector3Cross(vLook, vRight);

	// ���� ����
	m_pTransformCom->Set_State(STATE::RIGHT, vRight);
	m_pTransformCom->Set_State(STATE::UP, vUp);
	m_pTransformCom->Set_State(STATE::LOOK, vLook);

#pragma region ������ ��
	//Ÿ�ٿ��� �ڽ��� ��ġ�� ���̸� ���� ���̽� ��ġ�� ����
	_float fDist = {};
	_vector vRayDir = vCamPos - vTargetPos;
	vRayDir = XMVector3Normalize(vRayDir);
	CGameObject* pHit = m_pGameInstance->Check_RaycastHit(ENUM_CLASS(COLLISION_GROUP::BUILDING), TEXT("Com_Collider"), vTargetPos, vRayDir, fDist);

	if (pHit != nullptr)
	{
		if (fDist < fDistance/*ī�޶� ���� ���̺��� ª����*/)
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

			if (fDist < fDistance/*ī�޶� ���� ���̺��� ª����*/)
			{
				fDistance = fDist;
				m_pTransformCom->Set_State(STATE::POSITION, vTargetPos + vRayDir * fDist * 0.9f);
			}
		}
	}

#pragma endregion

	// ��/�������� ����
	__super::Bind_Matrices();
#pragma endregion


	// Ŀ�� �ٽ� ���̱�
	while (ShowCursor(TRUE) < 0);   // ī��Ʈ�� 0 �̻� �� ������ �ݺ�

	// Ŀ�� �̵� ���� ����
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
