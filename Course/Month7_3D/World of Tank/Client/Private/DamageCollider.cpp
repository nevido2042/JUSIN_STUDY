#include "DamageCollider.h"

#include "GameInstance.h"
#include "Tank.h"

CDamageCollider::CDamageCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLandObject{ pDevice, pContext }
{

}

CDamageCollider::CDamageCollider(const CDamageCollider& Prototype)
	: CLandObject(Prototype)
{

}

HRESULT CDamageCollider::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDamageCollider::Initialize(void* pArg)
{
	DAMAGECOLLIDER_DESC* pDesc = static_cast<DAMAGECOLLIDER_DESC*>(pArg);
	m_eDamageModule = pDesc->eDamageModule;

	switch (m_eDamageModule)
	{
	case MODULE::ENGINE:
		m_strText = L"Damage Engine";
		break;
	case MODULE::AMMO_BAY:
		m_strText = L"Damage AmmoBay";
		break;
	case MODULE::TURRET:
		m_strText = L"Damage Turret";
		break;
	}



	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}


void CDamageCollider::Priority_Update(_float fTimeDelta)
{
	if(m_bVisible)
		m_pGameInstance->Add_CollisionGroup(ENUM_CLASS(COLLISION_GROUP::DAMAGE_COLL), this, TEXT("Com_Collider"));
}

void CDamageCollider::Update(_float fTimeDelta)
{
	SetUp_Height(m_pTransformCom);
}

void CDamageCollider::Late_Update(_float fTimeDelta)
{
	if (m_bVisible)
	{
		m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));
		m_pGameInstance->Check_Collision(ENUM_CLASS(COLLISION_GROUP::BODY), this, TEXT("Com_Collider"), TEXT("Com_Collider"));

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
	_vector vWorldPos = m_pTransformCom->Get_State(STATE::POSITION);
	_matrix matView = m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW);
	_matrix matProj = m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ);

	_vector vClipPos = XMVector3TransformCoord(vWorldPos, matView * matProj);

	_float3 vClip;
	XMStoreFloat3(&vClip, vClipPos);

	_float2 vScreenPos;
	vScreenPos.x = (vClip.x * 0.5f + 0.5f) * g_iWinSizeX;
	vScreenPos.y = (-vClip.y * 0.5f + 0.5f) * g_iWinSizeY;

	m_pGameInstance->Draw_Font(
		TEXT("Font_WarheliosKO"),
		m_strText.c_str(),
		vScreenPos,
		XMVectorSet(1.f, 0.f, 0.f, 1.f),
		0.f,
		_float2(0.f, 0.f),
		0.3f * UI_RATIO
	);

	m_pColliderCom->Render();

	return S_OK;
}

void CDamageCollider::On_Collision_Stay(CGameObject* pOther, _fvector vNormal)
{
	wcout << "Shell On_Collision_Stay: " << pOther->GetName() << endl;
	m_bVisible = false;

	m_fAccTime = 0.f;

	CTank* pTank = static_cast<CTank*>(pOther);
	if (pTank)
	{
		pTank->Damage_Module(m_eDamageModule, 0.f);
	}
}


HRESULT CDamageCollider::Ready_Components()
{

	/* For.Com_Collider */
	CBounding_Sphere::SPHERE_DESC	SphereDesc{};
	SphereDesc.fRadius = 2.f;
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
