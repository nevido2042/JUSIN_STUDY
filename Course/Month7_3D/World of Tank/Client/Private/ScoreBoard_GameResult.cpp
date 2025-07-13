#include "ScoreBoard_GameResult.h"

#include "GameInstance.h"

CScoreBoard_GameResult::CScoreBoard_GameResult(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CScoreBoard_GameResult::CScoreBoard_GameResult(const CScoreBoard_GameResult& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CScoreBoard_GameResult::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CScoreBoard_GameResult::Initialize(void* pArg)
{
	GAMERESULT_DESC* pDesc = static_cast<GAMERESULT_DESC*>(pArg);

	m_bIsVictory = pDesc->bIsVictory;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(0.256f * 3.f, 0.144f * 3.f, 1.f);


	return S_OK;
}

void CScoreBoard_GameResult::Priority_Update(_float fTimeDelta)
{
	if (g_bWindowResizeRequired)
		Resize(g_iWinSizeX, g_iWinSizeY);

}

void CScoreBoard_GameResult::Update(_float fTimeDelta)
{
	//부모의 월드 행렬을 가져와서 자신의 월드 행렬과 곱해준다.
	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixMultiply(m_pTransformCom->Get_WorldMatrix(), XMLoadFloat4x4(m_pParentWorldMatrix)));

}

void CScoreBoard_GameResult::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);

}

HRESULT CScoreBoard_GameResult::Render()
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

	wstring Text = {};

	if (m_bIsVictory)
		Text = TEXT("승리");
	else
		Text = TEXT("패배");

	_vector vScale, vRot, vPos;
	XMMatrixDecompose(&vScale, &vRot, &vPos, XMLoadFloat4x4(&m_CombinedWorldMatrix));

	_float3 vPosF3;
	XMStoreFloat3(&vPosF3, vPos);

	m_pGameInstance->Draw_Font(TEXT("Font_WarheliosKO"), Text.c_str(), _float2(vPosF3.x + g_iWinSizeX * 0.5f - XMVectorGetX(vScale) * 0.25f, g_iWinSizeY * 0.5f - vPosF3.y - XMVectorGetY(vScale) * 0.25f), XMVectorSet(1.0f, 1.0f, 1.0f, 1.f), 0.f, _float2(0.f, 0.f), 1.f * UI_RATIO);


	return S_OK;
}

HRESULT CScoreBoard_GameResult::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (m_bIsVictory)
	{
		/* For.Com_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_Component_Texture_Victory"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
			return E_FAIL;
	}
	else
	{
		/* For.Com_Texture */
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_Component_Texture_Defeat"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
			return E_FAIL;
	}

	return S_OK;
}



CScoreBoard_GameResult* CScoreBoard_GameResult::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CScoreBoard_GameResult* pInstance = new CScoreBoard_GameResult(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CScoreBoard_GameResult");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CScoreBoard_GameResult::Clone(void* pArg)
{
	CScoreBoard_GameResult* pInstance = new CScoreBoard_GameResult(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CScoreBoard_GameResult");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScoreBoard_GameResult::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);

}
