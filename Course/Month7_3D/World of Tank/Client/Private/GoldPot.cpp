#include "GoldPot.h"

CGoldPot::CGoldPot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:C3DCustom(pDevice, pContext)
{
}

CGoldPot::CGoldPot(const CGoldPot& Prototype)
	:C3DCustom(Prototype)
{
}

HRESULT CGoldPot::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	return S_OK;
}

void CGoldPot::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
	
	CGameObject::Priority_Update(fTimeDelta);
}

void CGoldPot::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	CGameObject::Update(fTimeDelta);
}

void CGoldPot::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	CGameObject::Late_Update(fTimeDelta);
}

HRESULT CGoldPot::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CGoldPot::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_GoldPot"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CGoldPot::Ready_PartObjects()
{
	/* DragonHeadSmoke 파티클을 추가한다. */
	GAMEOBJECT_DESC Desc{};
	Desc.pParent = this;
	Desc.vInitPosition = { 0.f, 1.5f, 0.0f };
	Desc.pParentWorldMatrix = &m_CombinedWorldMatrix;
	lstrcpy(Desc.szName, TEXT("DragonHeadSmoke"));
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_GoldPotShining"), TEXT("Part_GoldPotShining"), &Desc)))
		return E_FAIL;

	return S_OK;
}

CGoldPot* CGoldPot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGoldPot* pInstance = new CGoldPot(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGoldPot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGoldPot::Clone(void* pArg)
{
	CGoldPot* pInstance = new CGoldPot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGoldPot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGoldPot::Free()
{
	__super::Free();

	for (auto& Pair : m_PartObjects)
		Safe_Release(Pair.second);
	m_PartObjects.clear();
}
