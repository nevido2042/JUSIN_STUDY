#include "DamagePanel.h"

#include "GameInstance.h"
#include "Icon_Module.h" 

CDamagePanel::CDamagePanel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CDamagePanel::CDamagePanel(const CDamagePanel& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CDamagePanel::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDamagePanel::Initialize(void* pArg)
{
	UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	m_pSoundCom_Voice->SetVolume(0.3f);

	return S_OK;
}

void CDamagePanel::Priority_Update(_float fTimeDelta)
{
	if (g_bWindowResizeRequired)
		Resize(g_iWinSizeX, g_iWinSizeY);

	CGameObject::Priority_Update(fTimeDelta);

}

void CDamagePanel::Update(_float fTimeDelta)
{
	CGameObject::Update(fTimeDelta);

}

void CDamagePanel::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);

	CGameObject::Late_Update(fTimeDelta);

}

HRESULT CDamagePanel::Render()
{

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	_float fAlpha = { 0.0f };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &fAlpha, sizeof(_float))))
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

void CDamagePanel::Play_Voice_StartBattle()
{
	m_pSoundCom_Voice->Play("start_battle_2");
}

void CDamagePanel::Play_Voice_EngineState(MODULE_STATE eState)
{
	switch (eState)
	{
	case MODULE_STATE::DESTROYED:
		m_pSoundCom_Voice->Play("engine_destroyed_4");
		break;
	case MODULE_STATE::DAMAGED:
		m_pSoundCom_Voice->Play("engine_damaged_6");
		break;
	case MODULE_STATE::FUNCTIONAL:
		//m_pSoundCom_Voice->Play("engine_functional_1");
		break;
	case MODULE_STATE::END:
		break;
	default:
		break;
	}

}

void CDamagePanel::Repair_All()
{
	static_cast<CIcon_Module*>(Find_PartObject(TEXT("Part_Engine")))->Set_ModuleState(MODULE_STATE::FUNCTIONAL);
	static_cast<CIcon_Module*>(Find_PartObject(TEXT("Part_AmmoBay")))->Set_ModuleState(MODULE_STATE::FUNCTIONAL);

	if(m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::GAMEPLAY))
	{
		MODULE_STATE_DESC Desc{};
		Desc.iID = m_pGameInstance->Get_ID();
		Desc.eModule = MODULE::ENGINE;
		Desc.eState = MODULE_STATE::FUNCTIONAL;
		m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_MODULE_STATE), &Desc);
	}
}

HRESULT CDamagePanel::Ready_Components()
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_DamagePanel"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
	/* For.Com_Sound_Voice */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController_Voice"),
		TEXT("Com_Sound_Voice"), reinterpret_cast<CComponent**>(&m_pSoundCom_Voice))))
		return E_FAIL;

	return S_OK;
}

HRESULT CDamagePanel::Ready_PartObjects()
{
	CIcon_Module::ICON_MODULE_DESC Desc{};
	Desc.fDepth = DEPTH_BACKGROUND - 0.1f;
	Desc.bIsChild = true;
	Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();

	lstrcpy(Desc.szName, TEXT("Icon_Engine"));
	Desc.strTextureName = TEXT("Prototype_Component_Texture_Icon_Engine");
	Desc.fX = -0.38f;
	Desc.fY = 0.23f;
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Icon_Module"), TEXT("Part_Engine"), &Desc)))
		return E_FAIL;

	lstrcpy(Desc.szName, TEXT("Icon_AmmoBay"));
	Desc.strTextureName = TEXT("Prototype_Component_Texture_Icon_AmmoBay");
	Desc.fX = -0.38f;
	Desc.fY = 0.08f;
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Icon_Module"), TEXT("Part_AmmoBay"), &Desc)))
		return E_FAIL;

	CUIObject::UIOBJECT_DESC				BodyDir_Desc{};

	BodyDir_Desc.bIsChild = true;
	BodyDir_Desc.fX = 0.f;
	BodyDir_Desc.fY = 0.f;
	BodyDir_Desc.fRotationPerSec = 1.f;
	BodyDir_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();

	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DirectionBody"), TEXT("Part_DirectionBody"), &BodyDir_Desc)))
		return E_FAIL;

	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DirectionTurret"), TEXT("Part_DirectionTurret"), &BodyDir_Desc)))
		return E_FAIL;

	return S_OK;
}

CDamagePanel* CDamagePanel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDamagePanel* pInstance = new CDamagePanel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDamagePanel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDamagePanel::Clone(void* pArg)
{
	CDamagePanel* pInstance = new CDamagePanel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDamagePanel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDamagePanel::Free()
{
	__super::Free();

	for (auto& Pair : m_PartObjects)
		Safe_Release(Pair.second);
	m_PartObjects.clear();

	Safe_Release(m_pSoundCom_Voice);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
