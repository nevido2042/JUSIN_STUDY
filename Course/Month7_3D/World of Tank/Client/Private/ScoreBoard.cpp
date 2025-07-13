#include "ScoreBoard.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "ScoreBoard_TotalDamage.h"
#include "ScoreBoard_GameResult.h"
#include "ScoreBoard_CountDamageModule.h"

CScoreBoard::CScoreBoard(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CScoreBoard::CScoreBoard(const CScoreBoard& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CScoreBoard::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CScoreBoard::Initialize(void* pArg)
{
	UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	CGameManager* pGameManager = GET_GAMEMANAGER;
	if (pGameManager == nullptr)
		return E_FAIL;

	m_tGameResult = pGameManager->Get_GameResult();

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;



	return S_OK;
}

void CScoreBoard::Priority_Update(_float fTimeDelta)
{
	if (g_bWindowResizeRequired)
		Resize(g_iWinSizeX, g_iWinSizeY);

	CGameObject::Priority_Update(fTimeDelta);
}

void CScoreBoard::Update(_float fTimeDelta)
{

	CGameObject::Update(fTimeDelta);
}

void CScoreBoard::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);

	CGameObject::Late_Update(fTimeDelta);
}

HRESULT CScoreBoard::Render()
{

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
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

HRESULT CScoreBoard::Ready_Components()
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_Component_Texture_ScoreBoard"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CScoreBoard::Ready_PartObjects()
{
	CScoreBoard_GameResult::GAMERESULT_DESC				GameResultDesc{};

	GameResultDesc.bIsChild = true;
	GameResultDesc.fX = 0.f;
	GameResultDesc.fY = 0.2f;
	GameResultDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	GameResultDesc.bIsVictory = m_tGameResult.bIsVictory;
	GameResultDesc.fDepth = DEPTH_BACKGROUND;

	if (FAILED(Add_PartObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_ScoreBoard_GameResult"), TEXT("Part_GameResult"), &GameResultDesc)))
		return E_FAIL;

	CScoreBoard_TotalDamage::TOTALDAMAGE_DESC				TotalDamageDesc{};

	TotalDamageDesc.bIsChild = true;
	TotalDamageDesc.fX = -0.3f;
	TotalDamageDesc.fY = -0.1f;
	TotalDamageDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	TotalDamageDesc.fTotalDamage = m_tGameResult.fTotalDamage;

	if (FAILED(Add_PartObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_ScoreBoard_TotalDamage"), TEXT("Part_TotalDamage"), &TotalDamageDesc)))
		return E_FAIL;

	CScoreBoard_CountDamageModule::COUNTDAMAGEMODULE_DESC				CountDamageModuleDesc{};

	CountDamageModuleDesc.bIsChild = true;
	CountDamageModuleDesc.fX = -0.3f;
	CountDamageModuleDesc.fY = -0.25f;
	CountDamageModuleDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	CountDamageModuleDesc.iCountDamageModule = m_tGameResult.iNumDamageModule;

	if (FAILED(Add_PartObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_ScoreBoard_CountDamageModule"), TEXT("Part_CountDamageModule"), &CountDamageModuleDesc)))
		return E_FAIL;

	CUIObject::UIOBJECT_DESC				Desc{};

	Desc.bIsChild = true;
	Desc.fX = 0.425f;
	Desc.fY = 0.45f;
	Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Desc.pParent = this;
	Desc.fDepth = DEPTH_BACKGROUND - 0.01f;
	if (FAILED(Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_CloseButton"), TEXT("Part_CloseButton"), &Desc)))
		return E_FAIL;

	return S_OK;
}

CScoreBoard* CScoreBoard::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CScoreBoard* pInstance = new CScoreBoard(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CScoreBoard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CScoreBoard::Clone(void* pArg)
{
	CScoreBoard* pInstance = new CScoreBoard(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CScoreBoard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScoreBoard::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);

	for (auto Pair : m_PartObjects)
	{
		Safe_Release(Pair.second);
	}
	m_PartObjects.clear();
}
