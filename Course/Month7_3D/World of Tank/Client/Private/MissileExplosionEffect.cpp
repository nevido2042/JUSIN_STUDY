#include "MissileExplosionEffect.h"
#include "GameInstance.h"

CMissileExplosionEffect::CMissileExplosionEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CMissileExplosionEffect::CMissileExplosionEffect(const CMissileExplosionEffect& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CMissileExplosionEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMissileExplosionEffect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(300.f, 300.f, 300.f);

	return S_OK;
}


void CMissileExplosionEffect::Priority_Update(_float fTimeDelta)
{

}

void CMissileExplosionEffect::Update(_float fTimeDelta)
{
	m_fFrame += fTimeDelta * 10.f;

	if (static_cast<_float>(iMaxTextureCount) < m_fFrame)
	{
		m_fFrame = static_cast<_float>(iMaxTextureCount);
		Destroy();
	}

	m_pTransformCom->LookAt(XMLoadFloat4(m_pGameInstance->Get_CamPosition()));
}

void CMissileExplosionEffect::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CMissileExplosionEffect::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", static_cast<_uint>(m_fFrame))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(2)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	return S_OK;
}

HRESULT CMissileExplosionEffect::Ready_Components()
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

HRESULT CMissileExplosionEffect::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Depth"), m_pShaderCom, "g_DepthTexture")))
		return E_FAIL;

	return S_OK;
}

CMissileExplosionEffect* CMissileExplosionEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMissileExplosionEffect* pInstance = new CMissileExplosionEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMissileExplosionEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMissileExplosionEffect::Clone(void* pArg)
{
	CMissileExplosionEffect* pInstance = new CMissileExplosionEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMissileExplosionEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMissileExplosionEffect::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
