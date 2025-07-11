#include "TopBar.h"

#include "GameInstance.h"

CTopBar::CTopBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CTopBar::CTopBar(const CTopBar& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CTopBar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTopBar::Initialize(void* pArg)
{
	UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	//if (FAILED(Ready_PartObjects()))
	//	return E_FAIL;

	return S_OK;
}

void CTopBar::Priority_Update(_float fTimeDelta)
{
	if (g_bWindowResizeRequired)
		Resize(g_iWinSizeX, g_iWinSizeY);

	CGameObject::Priority_Update(fTimeDelta);
}

void CTopBar::Update(_float fTimeDelta)
{
	CGameObject::Update(fTimeDelta);
}

void CTopBar::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI_NONBLEND, this);

	CGameObject::Late_Update(fTimeDelta);
}

HRESULT CTopBar::Render()
{

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	//_float fAlpha = { 0.0f };
	//if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &fAlpha, sizeof(_float))))
	//	return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(7)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTopBar::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_Component_Texture_TopBar"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

//HRESULT CTopBar::Ready_PartObjects()
//{
//	CUIObject::UIOBJECT_DESC				UIObject_Desc{};
//
//	UIObject_Desc.bIsChild = true;
//	UIObject_Desc.fX = 0.f;
//	UIObject_Desc.fY = 0.f;
//	UIObject_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
//
//	if (FAILED(Add_PartObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Button_Start"), TEXT("Part_StartButton"), &UIObject_Desc)))
//		return E_FAIL;
//
//	return S_OK;
//}

CTopBar* CTopBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTopBar* pInstance = new CTopBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTopBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTopBar::Clone(void* pArg)
{
	CTopBar* pInstance = new CTopBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTopBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTopBar::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);

	for (auto& Pair : m_PartObjects)
		Safe_Release(Pair.second);
	m_PartObjects.clear();
}
