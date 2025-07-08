#include "FuryTurret.h"

#include "GameInstance.h"
#include "Gun.h"

CFuryTurret::CFuryTurret(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTurret{ pDevice, pContext }
{

}

CFuryTurret::CFuryTurret(const CFuryTurret& Prototype)
	: CTurret(Prototype)
{

}

HRESULT CFuryTurret::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFuryTurret::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects(static_cast<TURRET_DESC*>(pArg))))
		return E_FAIL;

	return S_OK;
}


void CFuryTurret::Priority_Update(_float fTimeDelta)
{
	CTurret::Priority_Update(fTimeDelta);
}

void CFuryTurret::Update(_float fTimeDelta)
{
	CTurret::Update(fTimeDelta);
}

void CFuryTurret::Late_Update(_float fTimeDelta)
{
	CTurret::Late_Update(fTimeDelta);

	//m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);

	//CGameObject::Late_Update(fTimeDelta);

}

HRESULT CFuryTurret::Render()
{
	return CTurret::Render();
}

HRESULT CFuryTurret::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_FuryTurret"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_OBB::OBB_DESC	OBBDesc{};
	OBBDesc.vExtents = _float3(1.f, 0.5f, 1.2f);
	OBBDesc.vCenter = _float3(0.0f, 2.3f, -0.4f);
	OBBDesc.vRotation = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &OBBDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFuryTurret::Ready_PartObjects(TURRET_DESC* pDesc)
{
	if(FAILED(__super::Ready_PartObjects()))
		return E_FAIL;
	
	/* 주포을 추가한다. */
	CGun::GUN_DESC GunDesc{};
	GunDesc.vBaseColor = pDesc->vGunColor;
	GunDesc.pParentWorldMatrix = &m_CombinedWorldMatrix;
	GunDesc.fRotationPerSec = 0.25f;
	GunDesc.iID = m_iID;
	GunDesc.pOwner = pDesc->pOwner;
	lstrcpy(GunDesc.szName, TEXT("FuryGun"));
	GunDesc.eModuleType = MODULE::GUN;

	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_FuryGun"), TEXT("Part_Gun"), &GunDesc)))
		return E_FAIL;

	return S_OK;
}

CFuryTurret* CFuryTurret::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFuryTurret* pInstance = new CFuryTurret(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFuryTurret");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFuryTurret::Clone(void* pArg)
{
	CFuryTurret* pInstance = new CFuryTurret(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CFuryTurret");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFuryTurret::Free()
{
	__super::Free();

}
