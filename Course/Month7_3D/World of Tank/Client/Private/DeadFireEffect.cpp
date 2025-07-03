#include "DeadFireEffect.h"

#include "GameInstance.h"

CDeadFireEffect::CDeadFireEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CDeadFireEffect::CDeadFireEffect(const CDeadFireEffect& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CDeadFireEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDeadFireEffect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	//m_pTransformCom->Scaling(10.f, 10.f, 10.f);

	return S_OK;
}


void CDeadFireEffect::Priority_Update(_float fTimeDelta)
{

}

void CDeadFireEffect::Update(_float fTimeDelta)
{
	//점점 커지게
	m_fSize += fTimeDelta;
	if (m_fSize > 10.f)
		m_fSize = 10.f;
	m_pTransformCom->Scaling(m_fSize, m_fSize, m_fSize);

	//부모의 월드 행렬을 가져와서 자신의 월드 행렬과 곱해준다.
	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixMultiply(m_pTransformCom->Get_WorldMatrix(), XMLoadFloat4x4(m_pParentWorldMatrix)));

	m_fFrame += fTimeDelta * iMaxTextureCount;
	if(m_fFrame >= iMaxTextureCount)
		m_fFrame = 0.f;

	//m_fFrame = fmodf(m_fFrame, static_cast<_float>(iMaxTextureCount));

	//if (static_cast<_float>(iMaxTextureCount) < m_fFrame)
	//{
	//	m_fFrame = static_cast<_float>(iMaxTextureCount);
	//	m_fFrame = 0.f;
	//}

	_float3		vScaled = _float3(
		XMVectorGetX(XMVector3Length(XMVectorSet(m_CombinedWorldMatrix._11, m_CombinedWorldMatrix._12, m_CombinedWorldMatrix._13, 0.f))),
		XMVectorGetX(XMVector3Length(XMVectorSet(m_CombinedWorldMatrix._21, m_CombinedWorldMatrix._22, m_CombinedWorldMatrix._23, 0.f))),
		XMVectorGetX(XMVector3Length(XMVectorSet(m_CombinedWorldMatrix._31, m_CombinedWorldMatrix._32, m_CombinedWorldMatrix._33, 0.f))));

	_vector		vLook = XMLoadFloat4(m_pGameInstance->Get_CamPosition()) - XMVectorSet(m_CombinedWorldMatrix._41, m_CombinedWorldMatrix._42, m_CombinedWorldMatrix._43, 1.f);
	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	_vector		vUp = XMVector3Cross(vLook, vRight);

	XMStoreFloat4(reinterpret_cast<_float4*>(&m_CombinedWorldMatrix.m[ENUM_CLASS(STATE::RIGHT)]), XMVector3Normalize(vRight) * vScaled.x);
	XMStoreFloat4(reinterpret_cast<_float4*>(&m_CombinedWorldMatrix.m[ENUM_CLASS(STATE::UP)]), XMVector3Normalize(vUp) * vScaled.y);
	XMStoreFloat4(reinterpret_cast<_float4*>(&m_CombinedWorldMatrix.m[ENUM_CLASS(STATE::LOOK)]), XMVector3Normalize(vLook) * vScaled.z);
}

void CDeadFireEffect::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CDeadFireEffect::Render()
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

HRESULT CDeadFireEffect::Ready_Components()
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_DeadFireEffect"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CDeadFireEffect::Bind_ShaderResources()
{
	//if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
	//	return E_FAIL;

	//파트 오브젝트는 자기 트랜스폼 안써야한다
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Depth"), m_pShaderCom, "g_DepthTexture")))
		return E_FAIL;

	return S_OK;
}

CDeadFireEffect* CDeadFireEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDeadFireEffect* pInstance = new CDeadFireEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDeadFireEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDeadFireEffect::Clone(void* pArg)
{
	CDeadFireEffect* pInstance = new CDeadFireEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDeadFireEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDeadFireEffect::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
