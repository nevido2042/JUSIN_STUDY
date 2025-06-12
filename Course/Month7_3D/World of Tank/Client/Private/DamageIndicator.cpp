#include "DamageIndicator.h"

#include "GameInstance.h"

CDamageIndicator::CDamageIndicator(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CDamageIndicator::CDamageIndicator(const CDamageIndicator& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CDamageIndicator::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDamageIndicator::Initialize(void* pArg)
{
	UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CDamageIndicator::Priority_Update(_float fTimeDelta)
{
	if (g_bWindowResizeRequired)
		Resize(g_iWinSizeX, g_iWinSizeY);

	CGameObject::Priority_Update(fTimeDelta);

}

void CDamageIndicator::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Mouse_Down(ENUM_CLASS(DIMK::WHEEL)))
	{
		UIOBJECT_DESC Desc{};
		Desc.fDepth = DEPTH_BACKGROUND - 0.1f;
		Desc.bIsChild = true;
		lstrcpy(Desc.szName, TEXT("DamageBig"));
		Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();

		wstring strPartObjectTag = L"Part_DamageBig_" + to_wstring(m_iNum++);

		if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DamageBig"), strPartObjectTag, &Desc)))
			return;
	}

	CGameObject::Update(fTimeDelta);

}

void CDamageIndicator::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);

	CGameObject::Late_Update(fTimeDelta);

}

HRESULT CDamageIndicator::Render()
{
	CGameObject::Render();

	return S_OK;
}

HRESULT CDamageIndicator::Ready_Components()
{

	return S_OK;
}

CDamageIndicator* CDamageIndicator::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDamageIndicator* pInstance = new CDamageIndicator(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDamageIndicator");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDamageIndicator::Clone(void* pArg)
{
	CDamageIndicator* pInstance = new CDamageIndicator(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDamageIndicator");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDamageIndicator::Free()
{
	__super::Free();

	for (auto& Pair : m_PartObjects)
		Safe_Release(Pair.second);
	m_PartObjects.clear();
}
