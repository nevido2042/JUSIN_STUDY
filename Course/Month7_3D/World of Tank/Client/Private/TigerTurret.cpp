#include "TigerTurret.h"

#include "GameInstance.h"
#include "Gun.h"

CTigerTurret::CTigerTurret(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTurret{ pDevice, pContext }
{

}

CTigerTurret::CTigerTurret(const CTigerTurret& Prototype)
	: CTurret(Prototype)
{

}

HRESULT CTigerTurret::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTigerTurret::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects(static_cast<TURRET_DESC*>(pArg))))
		return E_FAIL;

	return S_OK;
}


void CTigerTurret::Priority_Update(_float fTimeDelta)
{
	CTurret::Priority_Update(fTimeDelta);

}

void CTigerTurret::Update(_float fTimeDelta)
{
	CTurret::Update(fTimeDelta);

}

void CTigerTurret::Late_Update(_float fTimeDelta)
{

	CGameObject::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CTigerTurret::Render()
{
	return CTurret::Render();
}


HRESULT CTigerTurret::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_TigerTurret"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CTigerTurret::Ready_PartObjects(TURRET_DESC* pDesc)
{
	/* 주포을 추가한다. */
	CGun::GUN_DESC GunDesc{};
	GunDesc.vBaseColor = pDesc->vGunColor;
	GunDesc.pParentWorldMatrix = &m_CombinedWorldMatrix;
	GunDesc.fRotationPerSec = 1.f;
	GunDesc.iID = m_iID;
	lstrcpy(GunDesc.szName, TEXT("TigerGun"));

	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_TigerGun"), TEXT("Part_Gun"), &GunDesc)))
		return E_FAIL;

	return S_OK;
}

CTigerTurret* CTigerTurret::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTigerTurret* pInstance = new CTigerTurret(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTigerTurret");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTigerTurret::Clone(void* pArg)
{
	CTigerTurret* pInstance = new CTigerTurret(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTigerTurret");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTigerTurret::Free()
{
	__super::Free();

	for (auto& Pair : m_PartObjects)
		Safe_Release(Pair.second);
	m_PartObjects.clear();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
