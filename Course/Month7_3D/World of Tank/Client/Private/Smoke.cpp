#include "Smoke.h"

#include "GameInstance.h"

CSmoke::CSmoke(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CSmoke::CSmoke(const CSmoke& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CSmoke::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSmoke::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	//m_pVIBufferCom->Load_Particle_Desc

	//m_pVIBufferCom->Set_EmissionShape(CVIBuffer_Instance::EMISSION_SHAPE::SPREAD);

	m_InitPivot = m_pVIBufferCom->Get_Desc().vPivot;

	return S_OK;
}

void CSmoke::Priority_Update(_float fTimeDelta)
{
	//if (m_pGameInstance->Key_Pressing(DIK_1))
	//{
	//	m_pVIBufferCom->Change_NumInstance(100 + rand() % 50);
	//}

}

void CSmoke::Update(_float fTimeDelta)
{
	if(m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::HANGER))
		return;

	//부모의 월드 행렬을 가져와서 자신의 월드 행렬과 곱해준다.
	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixMultiply(m_pTransformCom->Get_WorldMatrix(), XMLoadFloat4x4(m_pParentWorldMatrix)));


	m_pVIBufferCom->Emission(fTimeDelta);


}

void CSmoke::Late_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::HANGER))
		return;

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


	/* WeightBlend */
	//m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CSmoke::Render()
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

void CSmoke::Reset_Smoke_Pivot()
{
	m_OffsetPivot = {};
	m_pVIBufferCom->Change_Pivot(m_InitPivot);
}

void CSmoke::Add_Smoke_Pivot(_float vSpeed)
{
	m_OffsetPivot.z += vSpeed;
	m_OffsetPivot.z = clamp(m_OffsetPivot.z, -1.f, 1.f);

	m_OffsetPivot.y += abs(vSpeed);
	m_OffsetPivot.y = clamp(m_OffsetPivot.z, -1.f, 0.f);

	_float3 vResult = {};

	XMStoreFloat3(&vResult, XMLoadFloat3(&m_InitPivot) + XMLoadFloat3(&m_OffsetPivot));

	m_pVIBufferCom->Change_Pivot(vResult);
}

void CSmoke::Set_Loop(_bool bisLoop)
{
	m_pVIBufferCom->Change_isLoop(bisLoop);
}

HRESULT CSmoke::Ready_Components()
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

CSmoke* CSmoke::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSmoke* pInstance = new CSmoke(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSmoke");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSmoke::Clone(void* pArg)
{
	CSmoke* pInstance = new CSmoke(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSmoke");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSmoke::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
