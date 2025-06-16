#include "Camera_FPS.h"
#include "GameInstance.h"

#include "PickedManager.h"
#include "Terrain.h"

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

void CCamera_FPS::Priority_Update(_float fTimeDelta)
{
	//if (GetForegroundWindow() == g_hWnd)
	//{
		// �¿� ���� (Yaw)
		m_fYaw += XMConvertToRadians(2.f) * m_pGameInstance->Get_DIMMoveState(DIMM::X) * m_fSensor;
		// ���� ���� (Pitch)
		m_fPitch += XMConvertToRadians(-2.f) * m_pGameInstance->Get_DIMMoveState(DIMM::Y) * m_fSensor;
	//}

		if (m_pGameInstance->Get_DIMMoveState(DIMM::WHEEL) < 0.f)
		{
			m_fFov += XMConvertToRadians(10.0f);
			m_fFov = min(m_fFov, m_fMaxFov);
		}
		else if (m_pGameInstance->Get_DIMMoveState(DIMM::WHEEL) > 0.f)
		{
			m_fFov -= XMConvertToRadians(10.0f);
			m_fFov = max(m_fFov, m_fMinFov);
		}

#pragma message ("�̵� ������ �������� ������ �ϵ� ��, ���÷� ȿ�� �ֶ�� ��")
	m_pGameInstance->Set_Listener_Position(m_pTransformCom, _float3{});
}

void CCamera_FPS::Update(_float fTimeDelta)
{
	Picking();
}

void CCamera_FPS::Late_Update(_float fTimeDelta)
{
	// Pitch ���� ����
	m_fPitch = max(XMConvertToRadians(-20.f), min(XMConvertToRadians(70.f), m_fPitch));

	// �θ� ��ü�� World Matrix���� Up ���� �������� (Yaw ȸ�� ������)
	_vector vParentUp = XMVector3Normalize(m_pTarget->Get_CombinedWorldMatrix().r[1]);

	// ���� ��ǥ�� ��� ȸ�� ���� ��� (Pitch & Yaw ����)
	// ���� �ӽ÷� ���� ���� Z+�� ���¿��� Look ���� ���� (Yaw�� �θ� Up ���� ȸ��)
	_matrix matYaw = XMMatrixRotationAxis(vParentUp, m_fYaw);

	// Pitch�� Look ���� ���� Right ���� ȸ�������� ����ϱ� ���� Look ���� ���� ����
#pragma message ("XMVectorSet(0.f, 0.f, 1.f, 0.f) ������ ���������� �ݵ��� ���ϰ� ���� �ذ���� �𸣰���. �ݵ��� �ƿ� ���ִ� ���(�� ���� �ݵ� ����)")
	_vector vTempLook = XMVector3TransformNormal(XMVectorSet(0.f, 0.f, 1.f, 0.f), matYaw);
	_vector vRight = XMVector3Normalize(XMVector3Cross(vParentUp, vTempLook));

	// Pitch ȸ�� ����
	_matrix matPitch = XMMatrixRotationAxis(vRight, -m_fPitch);
	_vector vLook = XMVector3Normalize(XMVector3TransformNormal(vTempLook, matPitch));

	// ���� ī�޶� Up/Right ���� �����ϰ� ����ȭ
	vRight = XMVector3Normalize(XMVector3Cross(vParentUp, vLook)); // �ٽ� ��� (Pitch ���� �ݿ�)
	_vector vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));

	// Ÿ�� ��ġ �������� (�Ӹ� ��ġ ������ ��¦ ����)
	_vector vTargetPos = m_pTarget->Get_CombinedWorldMatrix().r[3];
	vTargetPos = XMVectorAdd(vTargetPos, XMVectorSet(0.f, 2.f, 0.f, 0.f));

	// ī�޶� ��ġ ���� (Ÿ�� ��ġ�� �̵�)
	m_pTransformCom->Set_State(STATE::POSITION, vTargetPos);

	// ȸ�� ���� ����
	m_pTransformCom->Set_State(STATE::LOOK, vLook);
	m_pTransformCom->Set_State(STATE::RIGHT, vRight);
	m_pTransformCom->Set_State(STATE::UP, vUp);


	m_pGameInstance->Set_Listener_Position(m_pTransformCom, _float3{});

	// ��/�������� ����
	__super::Bind_Matrices();

#pragma message ("��� ���°� �����ȵ����� �ϴ�")
	CGameObject* pGameObject = m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_PlayerTank"));
	if( nullptr == pGameObject )
		return;

	pGameObject->Set_Visible(false);
}

HRESULT CCamera_FPS::Render()
{

	return S_OK;
}

void CCamera_FPS::Picking()
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
