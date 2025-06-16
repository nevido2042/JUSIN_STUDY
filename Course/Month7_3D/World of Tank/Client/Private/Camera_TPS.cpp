#include "Camera_TPS.h"
#include "GameInstance.h"
#include "PickedManager.h"
#include "Terrain.h"

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
	pDesc->fFar = 1000.f;
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
	// �¿� ���� (Yaw)
	m_fYaw += XMConvertToRadians(-2.f) * m_pGameInstance->Get_DIMMoveState(DIMM::X) * m_fSensor;
	// ���� ���� (Pitch)
	m_fPitch += XMConvertToRadians(2.f) * m_pGameInstance->Get_DIMMoveState(DIMM::Y) * m_fSensor;
	// Pitch ���� ����
	m_fPitch = max(XMConvertToRadians(-10.f), min(XMConvertToRadians(85.f), m_fPitch));

#pragma message ("�̵� ������ �������� ������ �ϵ� ��, ���÷� ȿ�� �ֶ�� ��")
	m_pGameInstance->Set_Listener_Position(m_pTransformCom, _float3{});
}

void CCamera_TPS::Update(_float fTimeDelta)
{
	Picking();
}

void CCamera_TPS::Late_Update(_float fTimeDelta)
{
	const _float fDistance = 20.f;

	// Ÿ�� ��ġ
	_vector vTargetPos = m_pTarget->Get_CombinedWorldMatrix().r[3];
	// Y���� ��¦ �ø� (��: �Ӹ� �� 3.0f��ŭ)
	vTargetPos.m128_f32[1] += 5.f;

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

	//Ÿ�ٿ��� �ڽ��� ��ġ�� ���̸� ���� ���̽� ��ġ�� ����
	_float fDist = {};
	_vector vRayDir = vCamPos - vTargetPos;
	vRayDir = XMVector3Normalize(vRayDir);
	CGameObject* pHit = m_pGameInstance->Check_RaycastHit(ENUM_CLASS(COLLISION_GROUP::BUILDING), TEXT("Com_Collider"), vTargetPos, vRayDir, fDist);

	if (pHit != nullptr)
	{
		if (fDist < fDistance/*ī�޶� ���� ���̺��� ª����*/)
			m_pTransformCom->Set_State(STATE::POSITION, vTargetPos + vRayDir * fDist * 0.9f);
	}

	// ��/�������� ����
	__super::Bind_Matrices();

#pragma message ("��� �Ѵ°� �����ȵ����� �ϴ�")
	CGameObject* pGameObject = m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_PlayerTank"));
	if (nullptr == pGameObject)
		return;

	pGameObject->Set_Visible(true);
}

HRESULT CCamera_TPS::Render()
{

	return S_OK;
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
		_float3 vPos = {};
		XMStoreFloat3(&vPos, vOrigin + vRayDir * fMinDist);

		//���⼭ �ȵ� ��� ����ؼ� �ø���
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
