#include "MissileSmoke.h"

#include "GameInstance.h"

CMissileSmoke::CMissileSmoke(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CMissileSmoke::CMissileSmoke(const CMissileSmoke& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CMissileSmoke::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMissileSmoke::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CMissileSmoke::Priority_Update(_float fTimeDelta)
{


}

void CMissileSmoke::Update(_float fTimeDelta)
{
	//부모의 월드 행렬을 가져와서 자신의 월드 행렬과 곱해준다.
	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixMultiply(m_pTransformCom->Get_WorldMatrix(), XMLoadFloat4x4(m_pParentWorldMatrix)));

	m_pVIBufferCom->Emission(fTimeDelta);
}

void CMissileSmoke::Late_Update(_float fTimeDelta)
{
	// 위치만 추출
	_vector vPos = XMVectorSet(
		m_CombinedWorldMatrix._41,
		m_CombinedWorldMatrix._42,
		m_CombinedWorldMatrix._43,
		1.f
	);

	// 프러스텀 안에 있으면 렌더링 추가
	if (m_pGameInstance->Is_In_Frustum(vPos, 1.f))
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CMissileSmoke::Render()
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
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpah", &m_pVIBufferCom->Get_Desc().fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(1)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMissileSmoke::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosInstance"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_MissileSmoke"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Smoke"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CMissileSmoke* CMissileSmoke::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMissileSmoke* pInstance = new CMissileSmoke(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMissileSmoke");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMissileSmoke::Clone(void* pArg)
{
	CMissileSmoke* pInstance = new CMissileSmoke(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMissileSmoke");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMissileSmoke::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
