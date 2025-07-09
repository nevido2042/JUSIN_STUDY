#include "Tiger.h"

#include "GameInstance.h"

#include "TigerTurret.h"
#include "TigerGun.h"

#include "DamageBar_World.h"


CTiger::CTiger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTank(pDevice, pContext)
{

}

CTiger::CTiger(const CTiger& Prototype)
	: CTank(Prototype)
{

}

HRESULT CTiger::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTiger::Initialize(void* pArg)
{
	GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
	lstrcpy((*pDesc).szName, TEXT("Tiger"));
	(*pDesc).fRotationPerSec = 0.5f;
	(*pDesc).fSpeedPerSec = 0.5f;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects(static_cast<TANK_DESC*>(pArg))))
		return E_FAIL;

	return S_OK;
}


void CTiger::Priority_Update(_float fTimeDelta)
{
	CTank::Priority_Update(fTimeDelta);
}

void CTiger::Update(_float fTimeDelta)
{
	CTank::Update(fTimeDelta);
}

void CTiger::Late_Update(_float fTimeDelta)
{
	CTank::Late_Update(fTimeDelta);

}


HRESULT CTiger::Render()
{
	CTank::Render();

	return S_OK;
}



HRESULT CTiger::Ready_Components()
{
	CTank::Ready_Components();

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_TigerBody"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Model_Destroyed */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_TigerDestroyed"),
		TEXT("Com_Model_Destroyed"), reinterpret_cast<CComponent**>(&m_pModelCom_Destroyed))))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_OBB::OBB_DESC	OBBDesc{};
	OBBDesc.vExtents = _float3(1.5f, 1.f, 3.0f);
	OBBDesc.vCenter = _float3(0.0f, OBBDesc.vExtents.y, -0.1f);
	OBBDesc.vRotation = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &OBBDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTiger::Ready_PartObjects(TANK_DESC* pDesc)
{
	CTank::Ready_PartObjects(pDesc);

	/* 포탑을 추가한다. */
	CTurret::TURRET_DESC TurretDesc{};
	TurretDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	TurretDesc.fRotationPerSec = 0.5f;
	TurretDesc.iID = m_iID;
	lstrcpy(TurretDesc.szName, TEXT("Turret"));
	TurretDesc.vBaseColor = pDesc->vTurretColor;
	TurretDesc.e3DCustom = pDesc->e3DCustom;
	TurretDesc.vGunColor = pDesc->vGunColor;
	TurretDesc.pOwner = this;

	TurretDesc.eModuleType = MODULE::TURRET;
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_TigerTurret"), TEXT("Part_Turret"), &TurretDesc)))
		return E_FAIL;

	CModule::MODULE_DESC Desc{};
	Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Desc.fRotationPerSec = 1.f;
	Desc.iID = m_iID;
	Desc.pOwner = this;

	Desc.eModuleType = MODULE::ENGINE;
	/* 엔진을 추가한다. */
	lstrcpy(Desc.szName, TEXT("Tiger_Engine"));
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Engine"), TEXT("Part_Engine"), &Desc)))
		return E_FAIL;

	Desc.eModuleType = MODULE::CHASSIS;
	Desc.fRotationPerSec = 0.f;
	/* 궤도를 추가한다. */
	lstrcpy(Desc.szName, TEXT("Chassis"));
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Tiger_Chassis"), TEXT("Part_Chassis"), &Desc)))
		return E_FAIL;

	Desc.eModuleType = MODULE::AMMO_BAY;
	/* 탄약적재함을 추가한다 */
	lstrcpy(Desc.szName, TEXT("AmmoBay"));
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_AmmoBay"), TEXT("Part_AmmoBay"), &Desc)))
		return E_FAIL;

	Store_Modules();

	GAMEOBJECT_DESC EngineSmokeParticleDesc = {};
	EngineSmokeParticleDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();

	EngineSmokeParticleDesc.vInitPosition = _float3(0.5f, 1.7f, -3.0f);
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Smoke"), TEXT("Part_EngineSmokeParticle_Right"), &EngineSmokeParticleDesc)))
		return E_FAIL;
	EngineSmokeParticleDesc.vInitPosition = _float3(-0.5f, 1.7f, -3.0f);
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Smoke"), TEXT("Part_EngineSmokeParticle_Left"), &EngineSmokeParticleDesc)))
		return E_FAIL;

	return S_OK;
}

CTiger* CTiger::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTiger* pInstance = new CTiger(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTiger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTiger::Clone(void* pArg)
{
	CTiger* pInstance = new CTiger(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTiger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTiger::Free()
{
	__super::Free();

	for (auto& Pair : m_PartObjects)
		Safe_Release(Pair.second);
	m_PartObjects.clear();

	Safe_Release(m_pModelCom_Destroyed);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}


