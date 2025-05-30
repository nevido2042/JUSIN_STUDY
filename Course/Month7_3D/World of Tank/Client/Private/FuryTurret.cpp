#include "FuryTurret.h"

#include "GameInstance.h"

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
	GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
	m_pParentWorldMatrix = pDesc->pParentWorldMatrix;
	m_iID = pDesc->iID;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
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
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);

	CGameObject::Late_Update(fTimeDelta);

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

	return S_OK;
}

HRESULT CFuryTurret::Ready_PartObjects()
{
	/* 주포을 추가한다. */
	CGameObject::GAMEOBJECT_DESC Desc{};
	Desc.pParentWorldMatrix = &m_CombinedWorldMatrix;
	Desc.fRotationPerSec = 1.f;
	Desc.iID = m_iID;

	lstrcpy(Desc.szName, TEXT("FuryGun"));
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_FuryGun"), TEXT("Part_Gun"), &Desc)))
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

	for (auto& Pair : m_PartObjects)
		Safe_Release(Pair.second);
	m_PartObjects.clear();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
