#include "Fury.h"

#include "GameInstance.h"

#include "FuryTurret.h"
#include "FuryGun.h"


CFury::CFury(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTank(pDevice, pContext)
{

}

CFury::CFury(const CFury& Prototype)
	: CTank(Prototype)
{

}

HRESULT CFury::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFury::Initialize(void* pArg)
{
	GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
	lstrcpy((*pDesc).szName, TEXT("Fury"));

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects(static_cast<TANK_DESC*>(pArg))))
		return E_FAIL;

	return S_OK;
}


void CFury::Priority_Update(_float fTimeDelta)
{
	CTank::Priority_Update(fTimeDelta);
}

void CFury::Update(_float fTimeDelta)
{
	CTank::Update(fTimeDelta);
}

void CFury::Late_Update(_float fTimeDelta)
{
	CTank::Late_Update(fTimeDelta);

}


HRESULT CFury::Render()
{	
	CTank::Render();

	return S_OK;
}



HRESULT CFury::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_FuryBody"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Model_Destroyed */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_FuryDestroyed"),
		TEXT("Com_Model_Destroyed"), reinterpret_cast<CComponent**>(&m_pModelCom_Destroyed))))
		return E_FAIL;

	/* For.Com_Sound_Voice */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController_Voice"),
		TEXT("Com_Sound_Voice"), reinterpret_cast<CComponent**>(&m_pSoundCom_Voice))))
		return E_FAIL;

	/* For.Com_Sound_TankSound2D */
	//if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController_TankSound2D"),
	//	TEXT("Com_Sound_TankSound2D"), reinterpret_cast<CComponent**>(&m_pSoundCom_TankSound2D))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CFury::Ready_PartObjects(TANK_DESC* pDesc)
{
	/* 포탑을 추가한다. */
	CTurret::TURRET_DESC TurretDesc{};
	TurretDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	TurretDesc.fRotationPerSec = 1.f;
	TurretDesc.iID = m_iID;
	lstrcpy(TurretDesc.szName, TEXT("Turret"));
	TurretDesc.vBaseColor = pDesc->vTurretColor;
	TurretDesc.vGunColor = pDesc->vGunColor;

	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_FuryTurret"), TEXT("Part_Turret"), &TurretDesc)))
		return E_FAIL;

	GAMEOBJECT_DESC Desc{};
	Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Desc.fRotationPerSec = 1.f;
	Desc.iID = m_iID;

	/* 엔진을 추가한다. */
	lstrcpy(Desc.szName, TEXT("Engine"));
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Engine"), TEXT("Part_Engine"), &Desc)))
		return E_FAIL;

	/* 왼쪽 궤도를 추가한다. */
	lstrcpy(Desc.szName, TEXT("TrackLeft"));
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_FuryTrackLeft"), TEXT("Part_TrackLeft"), &Desc)))
		return E_FAIL;

	/* 오른쪽 궤도를 추가한다. */
	lstrcpy(Desc.szName, TEXT("TrackRight"));
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_FuryTrackRight"), TEXT("Part_TrackRight"), &Desc)))
		return E_FAIL;


	CModule* pEngine = static_cast<CModule*>(Find_PartObject(TEXT("Part_Engine")));
	CModule* pTurret = static_cast<CModule*>(Find_PartObject(TEXT("Part_Turret")));
	CModule* pGun = static_cast<CModule*>(Find_PartObject(TEXT("Part_Turret"))->Find_PartObject(TEXT("Part_Gun")));
	CModule* pTrackLeft = static_cast<CModule*>(Find_PartObject(TEXT("Part_TrackLeft")));
	CModule* pTrackRight = static_cast<CModule*>(Find_PartObject(TEXT("Part_TrackRight")));

	m_Modules.resize(ENUM_CLASS(MODULE::END));
	m_Modules.at(ENUM_CLASS(MODULE::ENGINE)) = pEngine;
	m_Modules.at(ENUM_CLASS(MODULE::TURRET)) = pTurret;
	m_Modules.at(ENUM_CLASS(MODULE::GUN)) = pGun;
	m_Modules.at(ENUM_CLASS(MODULE::TRACK_LEFT)) = pTrackLeft;
	m_Modules.at(ENUM_CLASS(MODULE::TRACK_RIGHT)) = pTrackRight;

	Safe_AddRef(pEngine);
	Safe_AddRef(pTurret);
	Safe_AddRef(pGun);
	Safe_AddRef(pTrackLeft);
	Safe_AddRef(pTrackRight);


	/* 데미지바_월드를 추가한다. */
	lstrcpy(Desc.szName, TEXT("DamageBar"));
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DamageBar_World"), TEXT("Part_DamageBar"), &Desc)))
		return E_FAIL;

	return S_OK;
}

CFury* CFury::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFury* pInstance = new CFury(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFury");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFury::Clone(void* pArg)
{
	CFury* pInstance = new CFury(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CFury");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFury::Free()
{
	__super::Free();

	for (auto& Pair : m_PartObjects)
		Safe_Release(Pair.second);
	m_PartObjects.clear();

	Safe_Release(m_pModelCom_Destroyed);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
