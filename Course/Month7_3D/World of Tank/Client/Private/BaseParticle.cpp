#include "BaseParticle.h"

#include "GameInstance.h"

CBaseParticle::CBaseParticle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CBaseParticle::CBaseParticle(const CBaseParticle& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CBaseParticle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBaseParticle::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	//m_pVIBufferCom->Set_EmissionShape(CVIBuffer_Instance::EMISSION_SHAPE::SPREAD);

	return S_OK;
}

void CBaseParticle::Priority_Update(_float fTimeDelta)
{
	//if (m_pGameInstance->Key_Pressing(DIK_1))
	//{
	//	m_pVIBufferCom->Change_NumInstance(100 + rand() % 50);
	//}
}

void CBaseParticle::Update(_float fTimeDelta)
{
	m_pVIBufferCom->Emission(fTimeDelta);
	//m_pVIBufferCom->Drop(fTimeDelta);
	//m_pVIBufferCom->Spread(fTimeDelta);

}

void CBaseParticle::Late_Update(_float fTimeDelta)
{
	/* WeightBlend */
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CBaseParticle::Render()
{

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
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

HRESULT CBaseParticle::Change_Texture(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag)
{
	Safe_Release(m_pTextureCom);

	Remove_Component(TEXT("Com_Texture"));

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(iPrototypeLevelIndex, strPrototypeTag,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CBaseParticle::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosInstance"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Smoke"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Smoke"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CBaseParticle* CBaseParticle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBaseParticle* pInstance = new CBaseParticle(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBaseParticle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBaseParticle::Clone(void* pArg)
{
	CBaseParticle* pInstance = new CBaseParticle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBaseParticle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBaseParticle::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
