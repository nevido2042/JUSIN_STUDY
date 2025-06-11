#include "DamageCollider.h"

#include "GameInstance.h"

CDamageCollider::CDamageCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CDamageCollider::CDamageCollider(const CDamageCollider& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CDamageCollider::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDamageCollider::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}


void CDamageCollider::Priority_Update(_float fTimeDelta)
{
	if(m_bVisible)
		m_pGameInstance->Add_CollisionGroup(ENUM_CLASS(COLLISION_GROUP::SHELL), this, TEXT("Com_Collider"));
}

void CDamageCollider::Update(_float fTimeDelta)
{
	
}

void CDamageCollider::Late_Update(_float fTimeDelta)
{
	if (m_bVisible)
	{
		m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));
		m_pGameInstance->Check_Collision(ENUM_CLASS(COLLISION_GROUP::MODULE), this, TEXT("Com_Collider"), TEXT("Com_Collider"));

		if (m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(STATE::POSITION), 0.1f))
			m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
	}
	else if (!m_bVisible)
	{
		m_fAccTime += fTimeDelta;

		if (m_fAccTime >= 2.f)
			m_bVisible = true;
	}
}

HRESULT CDamageCollider::Render()
{
#ifdef _DEBUG
	m_pColliderCom->Render();
#endif
	return S_OK;
}

void CDamageCollider::On_Collision_Stay(CGameObject* pOther, _fvector vNormal)
{
	wcout << "Shell On_Collision_Stay: " << pOther->GetName() << endl;
	m_bVisible = false;

	m_fAccTime = 0.f;
}


HRESULT CDamageCollider::Ready_Components()
{

	/* For.Com_Collider */
	CBounding_Sphere::SPHERE_DESC	SphereDesc{};
	SphereDesc.fRadius = 5.f;
	SphereDesc.vCenter = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDamageCollider::Bind_ShaderResources()
{

	return S_OK;
}

CDamageCollider* CDamageCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDamageCollider* pInstance = new CDamageCollider(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDamageCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDamageCollider::Clone(void* pArg)
{
	CDamageCollider* pInstance = new CDamageCollider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDamageCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDamageCollider::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
}
