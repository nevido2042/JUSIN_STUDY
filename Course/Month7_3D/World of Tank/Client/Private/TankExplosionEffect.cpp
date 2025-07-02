#include "TankExplosionEffect.h"

#include "GameInstance.h"

CTankExplosionEffect::CTankExplosionEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CTankExplosionEffect::CTankExplosionEffect(const CTankExplosionEffect& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CTankExplosionEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTankExplosionEffect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(30.f, 30.f, 30.f);

	return S_OK;
}


void CTankExplosionEffect::Priority_Update(_float fTimeDelta)
{

}

void CTankExplosionEffect::Update(_float fTimeDelta)
{
	fCurTexture += fTimeDelta * 20.f;

	if (static_cast<_float>(iMaxTextureCount) < fCurTexture)
	{
		fCurTexture = static_cast<_float>(iMaxTextureCount);
		Destroy();
	}

	m_pTransformCom->LookAt(XMLoadFloat4(m_pGameInstance->Get_CamPosition()));
}

void CTankExplosionEffect::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONLIGHT, this);
}

HRESULT CTankExplosionEffect::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", static_cast<_uint>(fCurTexture))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(2)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	return S_OK;
}

HRESULT CTankExplosionEffect::Ready_Components()
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_TankExplosionEffect"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CTankExplosionEffect::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	return S_OK;
}

CTankExplosionEffect* CTankExplosionEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTankExplosionEffect* pInstance = new CTankExplosionEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTankExplosionEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTankExplosionEffect::Clone(void* pArg)
{
	CTankExplosionEffect* pInstance = new CTankExplosionEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTankExplosionEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTankExplosionEffect::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
