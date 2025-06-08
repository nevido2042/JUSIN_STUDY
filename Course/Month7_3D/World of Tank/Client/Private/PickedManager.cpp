#include "PickedManager.h"

#include "GameInstance.h"

CPickedManager::CPickedManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CPickedManager::CPickedManager(const CPickedManager& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CPickedManager::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPickedManager::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}


void CPickedManager::Priority_Update(_float fTimeDelta)
{

}

void CPickedManager::Update(_float fTimeDelta)
{

}

void CPickedManager::Late_Update(_float fTimeDelta)
{
	Update_ScreenCenterPickedPos();
	Update_GunPickedPos();
	m_ScreenCenterPickedPoses.clear();
	m_GunPickedPoses.clear();
}

HRESULT CPickedManager::Render()
{

	return S_OK;
}

void CPickedManager::Add_ScreenCenterPickedPos(_float3 vPickedPos)
{
	m_ScreenCenterPickedPoses.push_back(vPickedPos);
}

void CPickedManager::Add_GunPickedPos(_float3 vPickedPos)
{
	m_GunPickedPoses.push_back(vPickedPos);
}

void CPickedManager::Update_ScreenCenterPickedPos()
{
	//카메라 위치르 가져와서, 현재 저장된 포즈의 거리와
	_vector vCamPos = XMVectorSetW(XMLoadFloat4(m_pGameInstance->Get_CamPosition()), 1.f);

	_float	fMinLength = FLT_MAX;
	_float3  vPickedPos = {};
	for (_float3& vPos : m_ScreenCenterPickedPoses)
	{
		//픽된 것들 중 가장 앞선 것을 픽포즈로 담는다.
		_float fNewLength = XMVectorGetX(XMVector3Length(vCamPos - XMLoadFloat3(&vPos)));

		if (fNewLength < fMinLength)
		{
			fMinLength = fNewLength;
			vPickedPos = vPos;
		}
	}

	m_vScreenCenterPickedPos = vPickedPos;
}

void CPickedManager::Update_GunPickedPos()
{
	//포구 피킹
	CGameObject* pPlayerTank = m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_PlayerTank"));
	if (nullptr == pPlayerTank)
		return;

	CGameObject* pGun = pPlayerTank->Find_PartObject(TEXT("Part_Turret"))->Find_PartObject(TEXT("Part_Gun"));
	if (nullptr == pGun)
		return;

	//카메라 위치르 가져와서, 현재 저장된 포즈의 거리와
	_vector vGunPos = pGun->Get_CombinedWorldMatrix().r[3];

	_float	fMinLength = FLT_MAX;
	_float3  vPickedPos = {};
	for (_float3& vPos : m_GunPickedPoses)
	{
		//픽된 것들 중 가장 앞선 것을 픽포즈로 담는다.
		_float fNewLength = XMVectorGetX(XMVector3Length(vGunPos - XMLoadFloat3(&vPos)));

		if (fNewLength < fMinLength)
		{
			fMinLength = fNewLength;
			vPickedPos = vPos;
		}
	}

	m_vGunPickedPos = vPickedPos;
}


CPickedManager* CPickedManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPickedManager* pInstance = new CPickedManager(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPickedManager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPickedManager::Clone(void* pArg)
{
	CPickedManager* pInstance = new CPickedManager(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPickedManager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPickedManager::Free()
{
	__super::Free();

}
