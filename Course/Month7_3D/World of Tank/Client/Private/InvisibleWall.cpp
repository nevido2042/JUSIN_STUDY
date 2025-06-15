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
