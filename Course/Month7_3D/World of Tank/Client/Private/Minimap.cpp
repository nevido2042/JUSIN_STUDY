#include "Minimap.h"

#include "GameInstance.h"
#include "PersonalArrowEntry.h"
#include "Minimap_Tank.h"
#include "Layer.h"
#include "Tank.h"

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

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

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
	CGameObject* pPlayerTank = m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_PlayerTank"));

	if (pPlayerTank == nullptr)
		return E_FAIL;

	CPersonalArrowEntry::PERSONAL_ARROW_ENTRY_DESC PersonalArrowEntry_Desc{};
	PersonalArrowEntry_Desc.fDepth = DEPTH_BACKGROUND - 0.1f;
	PersonalArrowEntry_Desc.bIsChild = true;
	lstrcpy(PersonalArrowEntry_Desc.szName, TEXT("PersonalArrowEntry"));
	PersonalArrowEntry_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	PersonalArrowEntry_Desc.pTarget = pPlayerTank;

	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_PersonalArrowEntry"), TEXT("Part_PersonalArrowEntry"), &PersonalArrowEntry_Desc)))
		return E_FAIL;

	CLayer* pLayer = m_pGameInstance->Find_Layer(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Tank"));
	if (pLayer == nullptr)
		return S_OK;

	CMinimap_Tank::MINIMAP_TANK_DESC MinimapTankDesc{};
	MinimapTankDesc.fDepth = DEPTH_BACKGROUND - 0.1f;
	MinimapTankDesc.bIsChild = true;
	lstrcpy(MinimapTankDesc.szName, TEXT("Minimap_Tank"));
	MinimapTankDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();

	_uint iCount = 0;
	for (CGameObject* pTank : pLayer->Get_GameObjects())
	{

		if (static_cast<CTank*>(pPlayerTank)->Get_Team() == static_cast<CTank*>(pTank)->Get_Team())
			lstrcpy(MinimapTankDesc.szTextureName, TEXT("Prototype_Component_Texture_MediumTank_Ally_Green"));
		else
			lstrcpy(MinimapTankDesc.szTextureName, TEXT("Prototype_Component_Texture_MediumTank_Enemy_Red"));

		MinimapTankDesc.pTarget = pTank;
		wstring PartName = TEXT("Part_Minimap_Tank_") + to_wstring(iCount++);

		if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Minimap_Tank"), PartName, &MinimapTankDesc)))
			return E_FAIL;
	}

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
