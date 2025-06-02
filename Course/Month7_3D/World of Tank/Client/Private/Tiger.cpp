#include "Tiger.h"

#include "GameInstance.h"

#include "TigerTurret.h"
#include "TigerGun.h"


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

	/* For.Com_Sound */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController_Voice"),
		TEXT("Com_Sound"), reinterpret_cast<CComponent**>(&m_pSoundCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CTiger::Ready_PartObjects(TANK_DESC* pDesc)
{
	/* 포탑을 추가한다. */
	CTurret::TURRET_DESC TurretDesc{};
	TurretDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	TurretDesc.fRotationPerSec = 1.f;
	TurretDesc.iID = m_iID;
	lstrcpy(TurretDesc.szName, TEXT("Turret"));
	TurretDesc.vBaseColor = pDesc->vTurretColor;
	TurretDesc.vGunColor = pDesc->vGunColor;

	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_TigerTurret"), TEXT("Part_Turret"), &TurretDesc)))
		return E_FAIL;

	CGameObject::GAMEOBJECT_DESC Desc{};
	Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Desc.fRotationPerSec = 1.f;
	Desc.iID = m_iID;

	/* 엔진을 추가한다. */
	lstrcpy(Desc.szName, TEXT("Engine"));
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Engine"), TEXT("Part_Engine"), &Desc)))
		return E_FAIL;

	/* 왼쪽 궤도를 추가한다. */
	lstrcpy(Desc.szName, TEXT("TrackLeft"));
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_TigerTrackLeft"), TEXT("Part_TrackLeft"), &Desc)))
		return E_FAIL;

	/* 오른쪽 궤도를 추가한다. */
	lstrcpy(Desc.szName, TEXT("TrackRight"));
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_TigerTrackRight"), TEXT("Part_TrackRight"), &Desc)))
		return E_FAIL;

	/* 데미지바_월드를 추가한다. */
	lstrcpy(Desc.szName, TEXT("DamageBar"));
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DamageBar_World"), TEXT("Part_DamageBar"), &Desc)))
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
