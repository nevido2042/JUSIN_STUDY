#include "TigerTurretSkin.h"

#include "GameInstance.h"

CTigerTurretSkin::CTigerTurretSkin(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTurret{ pDevice, pContext }
{

}

CTigerTurretSkin::CTigerTurretSkin(const CTigerTurretSkin& Prototype)
	: CTurret(Prototype)
{

}

HRESULT CTigerTurretSkin::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTigerTurretSkin::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	return S_OK;
}


void CTigerTurretSkin::Priority_Update(_float fTimeDelta)
{
	CTurret::Priority_Update(fTimeDelta);

}

void CTigerTurretSkin::Update(_float fTimeDelta)
{
	CTurret::Update(fTimeDelta);

}

void CTigerTurretSkin::Late_Update(_float fTimeDelta)
{

	CGameObject::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CTigerTurretSkin::Render()
{
	return CTurret::Render();
}


HRESULT CTigerTurretSkin::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_TigerTurretSkin"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CTigerTurretSkin::Ready_PartObjects()
{
	/* 주포을 추가한다. */
	CGameObject::GAMEOBJECT_DESC Desc{};
	Desc.pParentWorldMatrix = &m_CombinedWorldMatrix;
	Desc.fRotationPerSec = 1.f;
	Desc.iID = m_iID;

	lstrcpy(Desc.szName, TEXT("TigerGun"));
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_TigerGunSkin"), TEXT("Part_Gun"), &Desc)))
		return E_FAIL;

	return S_OK;
}

CTigerTurretSkin* CTigerTurretSkin::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTigerTurretSkin* pInstance = new CTigerTurretSkin(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTigerTurretSkin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTigerTurretSkin::Clone(void* pArg)
{
	CTigerTurretSkin* pInstance = new CTigerTurretSkin(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTigerTurretSkin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTigerTurretSkin::Free()
{
	__super::Free();

	for (auto& Pair : m_PartObjects)
		Safe_Release(Pair.second);
	m_PartObjects.clear();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
