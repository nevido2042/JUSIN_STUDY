#include "PersonalArrowEntry.h"

#include "GameInstance.h"

CPersonalArrowEntry::CPersonalArrowEntry(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CPersonalArrowEntry::CPersonalArrowEntry(const CPersonalArrowEntry& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CPersonalArrowEntry::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPersonalArrowEntry::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(0.2f, 0.2f, 0.2f);

	return S_OK;
}

void CPersonalArrowEntry::Priority_Update(_float fTimeDelta)
{
	if (g_bWindowResizeRequired)
		Resize(g_iWinSizeX, g_iWinSizeY);
}

void CPersonalArrowEntry::Update(_float fTimeDelta)
{
	CGameObject* pPlayerTank = m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_PlayerTank"));
	if (pPlayerTank)
	{
		_vector vPlayerPos = pPlayerTank->Get_Transform()->Get_State(STATE::POSITION);
		_vector vIconPos = XMVectorSet(-0.5f + (XMVectorGetX(vPlayerPos) / (128.f * 5.f * 0.9f)), 0.5f - (XMVectorGetZ(vPlayerPos) / (128.f * 5.f * 0.9f)), m_fDepth, 1.f);//XMVectorSet(XMVectorGetX(vPlayerPos) / 128.f * 5.f, -XMVectorGetZ(vPlayerPos) / 128.f * 5.f, m_fDepth, 1.f);
		m_pTransformCom->Set_State(STATE::POSITION, vIconPos);
	}
	
	//부모의 월드 행렬을 가져와서 자신의 월드 행렬과 곱해준다.
	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixMultiply(m_pTransformCom->Get_WorldMatrix(), XMLoadFloat4x4(m_pParentWorldMatrix)));
}

void CPersonalArrowEntry::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CPersonalArrowEntry::Render()
{
	//파트 오브젝트는 자기 트랜스폼 안써야한다
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	_float fAlpha = { 0.9f };
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

HRESULT CPersonalArrowEntry::Ready_Components()
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_PersonalArrowEntry"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CPersonalArrowEntry* CPersonalArrowEntry::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPersonalArrowEntry* pInstance = new CPersonalArrowEntry(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPersonalArrowEntry");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPersonalArrowEntry::Clone(void* pArg)
{
	CPersonalArrowEntry* pInstance = new CPersonalArrowEntry(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPersonalArrowEntry");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPersonalArrowEntry::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
