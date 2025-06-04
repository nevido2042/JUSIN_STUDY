#include "Icon_Module.h"

#include "GameInstance.h"

CIcon_Module::CIcon_Module(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CIcon_Module::CIcon_Module(const CIcon_Module& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CIcon_Module::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CIcon_Module::Initialize(void* pArg)
{
	ICON_MODULE_DESC* pDesc = static_cast<ICON_MODULE_DESC*>(pArg);
	m_strTextureName = pDesc->strTextureName;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(0.2f, 0.2f, 0.2f);
	//m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(-0.38f, 0.23f, 0.f, 1.f));

	return S_OK;
}

void CIcon_Module::Priority_Update(_float fTimeDelta)
{
	if (g_bWindowResizeRequired)
		Resize(g_iWinSizeX, g_iWinSizeY);
}

void CIcon_Module::Update(_float fTimeDelta)
{
	//부모의 월드 행렬을 가져와서 자신의 월드 행렬과 곱해준다.
	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixMultiply(m_pTransformCom->Get_WorldMatrix(), XMLoadFloat4x4(m_pParentWorldMatrix)));
}

void CIcon_Module::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CIcon_Module::Render()
{
	//파트 오브젝트는 자기 트랜스폼 안써야한다
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	_float fAlpha = { 0.3f };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", ENUM_CLASS(m_eModuleState))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CIcon_Module::Ready_Components()
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), m_strTextureName,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CIcon_Module* CIcon_Module::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CIcon_Module* pInstance = new CIcon_Module(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CIcon_Module");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CIcon_Module::Clone(void* pArg)
{
	CIcon_Module* pInstance = new CIcon_Module(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CIcon_Module");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIcon_Module::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
