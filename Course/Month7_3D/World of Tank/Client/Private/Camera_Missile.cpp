#include "Camera_TPS.h"
#include "GameInstance.h"
#include "PickedManager.h"
#include "Terrain.h"
#include "Tank.h"
#include "Module.h"
#include "Camera_Missile.h"
#include "Layer.h"

CCamera_Missile::CCamera_Missile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera{ pDevice, pContext }
{

}

CCamera_Missile::CCamera_Missile(const CCamera_Missile& Prototype)
	: CCamera(Prototype)
{

}

HRESULT CCamera_Missile::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Missile::Initialize(void* pArg)
{
	CAMERA_MISSILE_DESC* pDesc = static_cast<CAMERA_MISSILE_DESC*>(pArg);
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

	//ī�޶� ��� ����
	CLayer* pLayer = m_pGameInstance->Find_Layer(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Camera"));
	if (!pLayer) return E_FAIL;
	auto& GameObjects = pLayer->Get_GameObjects();
	for (auto iter = GameObjects.begin(); iter != GameObjects.end(); ++iter)
	{
		(*iter)->Set_Active(false);
	}

	return S_OK;
}

void CCamera_Missile::Priority_Update(_float fTimeDelta)
{
	// �¿� ���� (Yaw)
	m_fYaw += XMConvertToRadians(-2.f) * m_pGameInstance->Get_DIMMoveState(DIMM::X) * m_fSensor;
	// ���� ���� (Pitch)
	m_fPitch += XMConvertToRadians(2.f) * m_pGameInstance->Get_DIMMoveState(DIMM::Y) * m_fSensor;
	// Pitch ���� ����
	m_fPitch = max(XMConvertToRadians(-40.f), min(XMConvertToRadians(85.f), m_fPitch));

#pragma message ("�̵� ������ �������� ������ �ϵ� ��, ���÷� ȿ�� �ֶ�� ��")
	m_pGameInstance->Set_Listener_Position(m_pTransformCom, _float3{});

}

void CCamera_Missile::Update(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fTimeAcc > 5.f)
	{
		m_fTimeAcc = 0.f;

		//ī�޶� �ϳ��� Ų��
		CLayer* pLayer = m_pGameInstance->Find_Layer(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Camera"));
		if (!pLayer) return;

		auto& GameObjects = pLayer->Get_GameObjects();
		//(*GameObjects.begin())->Set_Active(true);
		(*(++GameObjects.begin()))->Set_Active(true);

		Destroy();
	}

#pragma region ī�޶� ������
	_float fDistance = m_fArmLength;

	// Ÿ�� ��ġ
	_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(STATE::POSITION);
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

	// ��/�������� ����
	__super::Bind_Matrices();
#pragma endregion

	// Ŀ�� �����
	if (GetForegroundWindow() == g_hWnd)
	{
		while (ShowCursor(FALSE) >= 0); // ī��Ʈ�� 0���� �۾��� ������ �ݺ�
		Clip_Cursor();
	}
}

void CCamera_Missile::Late_Update(_float fTimeDelta)
{

#pragma message ("��� �Ѵ°� �����ȵ����� �ϴ�")
	CGameObject* pGameObject = m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_PlayerTank"));
	if (nullptr == pGameObject)
		return;

	pGameObject->Set_Visible(true);
}

HRESULT CCamera_Missile::Render()
{

	return S_OK;
}

CCamera_Missile* CCamera_Missile::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Missile* pInstance = new CCamera_Missile(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Missile::Clone(void* pArg)
{
	CCamera_Missile* pInstance = new CCamera_Missile(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Missile");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Missile::Free()
{
	__super::Free();
	Safe_Release(m_pTarget);
}
