#include "InvisibleWall.h"

#include "GameInstance.h"
#include "PickedManager.h"

CInvisibleWall::CInvisibleWall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CInvisibleWall::CInvisibleWall(const CInvisibleWall& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CInvisibleWall::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CInvisibleWall::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}


void CInvisibleWall::Priority_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_CollisionGroup(ENUM_CLASS(COLLISION_GROUP::WALL), this, TEXT("Com_Collider"));
}

void CInvisibleWall::Update(_float fTimeDelta)
{
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

	Picked_Ray_ScreenCenter();
	Picked_Ray_Gun();
}

void CInvisibleWall::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CInvisibleWall::Render()
{
#ifdef _DEBUG
	m_pColliderCom->Render();
#endif
	return S_OK;
}

void CInvisibleWall::Picked_Ray_ScreenCenter()
{
	_float fDist = { 0 };

	_bool bisColl = false;
	bisColl = m_pColliderCom->Intersect_Ray(XMVectorSetW(XMLoadFloat3(&m_pGameInstance->Get_ScreenCenterPos()), 1.f),
		XMVectorSetW(XMLoadFloat3(&m_pGameInstance->Get_ScreenCenterRay()), 1.f),
		fDist);

	if (bisColl)
	{
		_float3 vPickedPos = {};
		_vector vOrigin = { XMLoadFloat3(&m_pGameInstance->Get_ScreenCenterPos()) };
		_vector vDir = { XMLoadFloat3(&m_pGameInstance->Get_ScreenCenterRay()) };

		XMStoreFloat3(&vPickedPos, vOrigin + vDir * fDist);

		//여기서 픽된 포즈를 계산해서 올리자
		CPickedManager* pPickedManager = static_cast<CPickedManager*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_PickedManager")));
		if (pPickedManager)
			pPickedManager->Add_ScreenCenterPickedPos(vPickedPos);
	}

}

void CInvisibleWall::Picked_Ray_Gun()
{
	_float fDist = { 0 };

	_bool bisColl = false;

	//포구 피킹
	CGameObject* pPlayerTank = m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_PlayerTank"));
	if (nullptr == pPlayerTank)
		return;

	CGameObject* pGun = pPlayerTank->Find_PartObject(TEXT("Part_Turret"))->Find_PartObject(TEXT("Part_Gun"));
	if (nullptr == pGun)
		return;

	//카메라 위치르 가져와서, 현재 저장된 포즈의 거리와
	_vector vGunPos = pGun->Get_CombinedWorldMatrix().r[3];
	_vector vGunLook = pGun->Get_CombinedWorldMatrix().r[2];
	vGunLook = XMVector3Normalize(vGunLook);

	bisColl = m_pColliderCom->Intersect_Ray(vGunPos, vGunLook, fDist);

	if (bisColl)
	{
		_float3 vPickedPos = {};
		XMStoreFloat3(&vPickedPos, vGunPos + vGunLook * fDist);

		//여기서 픽된 포즈를 계산해서 올리자
		CPickedManager* pPickedManager = static_cast<CPickedManager*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_PickedManager")));
		if (pPickedManager)
			pPickedManager->Add_GunPickedPos(vPickedPos);
	}
}

HRESULT CInvisibleWall::Ready_Components(void* pArg)
{
	INVISIBLE_WALL_DESC* pDesc = static_cast<INVISIBLE_WALL_DESC*>(pArg);

	/* For.Com_Collider */
	CBounding_AABB::AABB_DESC	AABB{};
	AABB.eType = COLLIDER::AABB;
	AABB.vExtents = pDesc->vExtents;// _float3(320.f, 500.f, 1.f);
	AABB.vCenter = _float3(0.0f, AABB.vExtents.y, 0.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &AABB)))
		return E_FAIL;

	return S_OK;
}


CInvisibleWall* CInvisibleWall::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CInvisibleWall* pInstance = new CInvisibleWall(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CInvisibleWall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CInvisibleWall::Clone(void* pArg)
{
	CInvisibleWall* pInstance = new CInvisibleWall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CInvisibleWall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInvisibleWall::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
}
