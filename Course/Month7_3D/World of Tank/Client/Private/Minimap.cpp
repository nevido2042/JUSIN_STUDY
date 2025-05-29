#include "Minimap.h"

#include "GameInstance.h"

CMinimap::CMinimap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CMinimap::CMinimap(const CMinimap& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CMinimap::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMinimap::Initialize(void* pArg)
{
	UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

#pragma message ("UI 부모 자식 트랜스폼 해보려다 실패")
	//if (FAILED(Ready_PartObjects()))
	//	return E_FAIL;

	return S_OK;
}

void CMinimap::Priority_Update(_float fTimeDelta)
{
	if (g_bWindowResizeRequired)
		Resize(g_iWinSizeX, g_iWinSizeY);

	CGameObject::Priority_Update(fTimeDelta);
}

void CMinimap::Update(_float fTimeDelta)
{
	CGameObject::Update(fTimeDelta);

}

void CMinimap::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);

	CGameObject::Late_Update(fTimeDelta);

}

HRESULT CMinimap::Render()
{

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	_float fAlpha = { 0.0f };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMinimap::Ready_Components()
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Minimap"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMinimap::Ready_PartObjects()
{
	UIOBJECT_DESC UIObject_Desc{};
	UIObject_Desc.fSizeX = 188.0f * UI_RATIO;
	UIObject_Desc.fSizeY = 226.0f * UI_RATIO;
	UIObject_Desc.fX = 0.f;
	UIObject_Desc.fY = 0.f;
	UIObject_Desc.fDepth = DEPTH_BACKGROUND - 0.1f;
	lstrcpy(UIObject_Desc.szName, TEXT("PersonalArrowEntry"));

	UIObject_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();

	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_PersonalArrowEntry"), TEXT("Part_PersonalArrowEntry"), &UIObject_Desc)))
		return E_FAIL;

	return S_OK;
}

CMinimap* CMinimap::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMinimap* pInstance = new CMinimap(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMinimap");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMinimap::Clone(void* pArg)
{
	CMinimap* pInstance = new CMinimap(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMinimap");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMinimap::Free()
{
	__super::Free();

	for (auto& Pair : m_PartObjects)
		Safe_Release(Pair.second);
	m_PartObjects.clear();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
