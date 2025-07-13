#include "ScoreBoard_CountDamageModule.h"

#include "GameInstance.h"

CScoreBoard_CountDamageModule::CScoreBoard_CountDamageModule(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CScoreBoard_CountDamageModule::CScoreBoard_CountDamageModule(const CScoreBoard_CountDamageModule& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CScoreBoard_CountDamageModule::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CScoreBoard_CountDamageModule::Initialize(void* pArg)
{
	COUNTDAMAGEMODULE_DESC* pDesc = static_cast<COUNTDAMAGEMODULE_DESC*>(pArg);

	m_iCountDamageModule = pDesc->iCountDamageModule;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(0.2f, 0.2f, 0.2f);


	return S_OK;
}

void CScoreBoard_CountDamageModule::Priority_Update(_float fTimeDelta)
{
	if (g_bWindowResizeRequired)
		Resize(g_iWinSizeX, g_iWinSizeY);

}

void CScoreBoard_CountDamageModule::Update(_float fTimeDelta)
{
	//부모의 월드 행렬을 가져와서 자신의 월드 행렬과 곱해준다.
	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixMultiply(m_pTransformCom->Get_WorldMatrix(), XMLoadFloat4x4(m_pParentWorldMatrix)));

}

void CScoreBoard_CountDamageModule::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);

}

HRESULT CScoreBoard_CountDamageModule::Render()
{

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;

	//if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
	//	return E_FAIL;

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

	wstring Text = to_wstring(m_iCountDamageModule);

	_vector vScale, vRot, vPos;
	XMMatrixDecompose(&vScale, &vRot, &vPos, XMLoadFloat4x4(&m_CombinedWorldMatrix));

	_float3 vPosF3;
	XMStoreFloat3(&vPosF3, vPos);

	m_pGameInstance->Draw_Font(TEXT("Font_WarheliosKO"), Text.c_str(), _float2(vPosF3.x + g_iWinSizeX * 0.5f + XMVectorGetX(vScale) * 0.5f, g_iWinSizeY * 0.5f - vPosF3.y - XMVectorGetY(vScale) * 0.25f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f), 0.5f * UI_RATIO);


	return S_OK;
}

HRESULT CScoreBoard_CountDamageModule::Ready_Components()
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_CountDamageModule"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}



CScoreBoard_CountDamageModule* CScoreBoard_CountDamageModule::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CScoreBoard_CountDamageModule* pInstance = new CScoreBoard_CountDamageModule(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CScoreBoard_CountDamageModule");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CScoreBoard_CountDamageModule::Clone(void* pArg)
{
	CScoreBoard_CountDamageModule* pInstance = new CScoreBoard_CountDamageModule(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CScoreBoard_CountDamageModule");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScoreBoard_CountDamageModule::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);

}
