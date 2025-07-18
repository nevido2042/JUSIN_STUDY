#include "DragonHead.h"

CDragonHead::CDragonHead(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:C3DCustom(pDevice, pContext)
{
}

CDragonHead::CDragonHead(const CDragonHead& Prototype)
	:C3DCustom(Prototype)
{
}

HRESULT CDragonHead::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if(FAILED(Ready_PartObjects()))
		return E_FAIL;

	return S_OK;
}

void CDragonHead::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	CGameObject::Priority_Update(fTimeDelta);
}

void CDragonHead::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	CGameObject::Update(fTimeDelta);
}

void CDragonHead::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	CGameObject::Late_Update(fTimeDelta);
}

HRESULT CDragonHead::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CDragonHead::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_DragonHead"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CDragonHead::Ready_PartObjects()
{
	/* DragonHeadSmoke 파티클을 추가한다. */
	GAMEOBJECT_DESC Desc{};
	Desc.pParent = this;
	Desc.vInitPosition = { 0.f, 0.3f, 0.6f };
	Desc.pParentWorldMatrix = &m_CombinedWorldMatrix;
	lstrcpy(Desc.szName, TEXT("DragonHeadSmoke"));
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DragonHeadSmoke"), TEXT("Part_DragonHeadSmoke"), &Desc)))
		return E_FAIL;

	return S_OK;
}

CDragonHead* CDragonHead::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDragonHead* pInstance = new CDragonHead(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDragonHead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDragonHead::Clone(void* pArg)
{
	CDragonHead* pInstance = new CDragonHead(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDragonHead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDragonHead::Free()
{
	__super::Free();

	for (auto& Pair : m_PartObjects)
		Safe_Release(Pair.second);
	m_PartObjects.clear();
}
