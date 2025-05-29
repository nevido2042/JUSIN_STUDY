#include "GunMarker.h"

#include "GameInstance.h"
#include "Terrain.h"

CGunMarker::CGunMarker(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CGunMarker::CGunMarker(const CGunMarker& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CGunMarker::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGunMarker::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	// 새 RasterizerState 설정
	D3D11_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthClipEnable = FALSE;
	rasterDesc.FrontCounterClockwise = FALSE;
	m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterState);

	D3D11_DEPTH_STENCIL_DESC depthDesc = {};
	depthDesc.DepthEnable = FALSE;  // 깊이 테스트 완전 비활성화
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;  // 비교는 무조건 통과
	depthDesc.StencilEnable = FALSE;
	m_pDevice->CreateDepthStencilState(&depthDesc, &m_pDSState);

	return S_OK;
}


void CGunMarker::Priority_Update(_float fTimeDelta)
{
}

void CGunMarker::Update(_float fTimeDelta)
{

	//터레인한테 포구 피킹 위치 가져와서 위치시키자
	CTerrain* pTerrain = static_cast<CTerrain*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Terrain")));
	if (nullptr == pTerrain)
		return;

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&pTerrain->Get_PickedPos_Gun()), 1.f));

	m_pTransformCom->LookAt(XMLoadFloat4(m_pGameInstance->Get_CamPosition()));

}

void CGunMarker::Late_Update(_float fTimeDelta)
{
	//거리에 따라 보정해서, 항상 같은 크기 출력하도록
	_vector vToCamera = XMLoadFloat4(m_pGameInstance->Get_CamPosition()) - m_pTransformCom->Get_State(STATE::POSITION);
	_float fDistance = XMVectorGetX(XMVector3Length(vToCamera));
	_float3 vFinalScale = { m_fBaseScale * fDistance, m_fBaseScale * fDistance, m_fBaseScale * fDistance };
	m_pTransformCom->Scaling(vFinalScale.x, vFinalScale.y, vFinalScale.z);


	//if (m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(STATE::POSITION), 10.f))
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);

}

HRESULT CGunMarker::Render()
{
	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGunMarker::SetUp_RenderState()
{
	m_pContext->RSGetState(&m_pOldRasterState);
	m_pContext->RSSetState(m_pRasterState);

	m_pContext->OMGetDepthStencilState(&m_pOldDSState, 0);
	m_pContext->OMSetDepthStencilState(m_pDSState, 0);

	return S_OK;
}

HRESULT CGunMarker::Release_RenderState()
{
	m_pContext->RSSetState(m_pOldRasterState);

	m_pContext->OMSetDepthStencilState(m_pOldDSState, 0);

	return S_OK;
}

HRESULT CGunMarker::Ready_Components()
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_GunMarker"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CGunMarker::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	_float fAlpha = { 0.1f };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &fAlpha, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CGunMarker* CGunMarker::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGunMarker* pInstance = new CGunMarker(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGunMarker");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGunMarker::Clone(void* pArg)
{
	CGunMarker* pInstance = new CGunMarker(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGunMarker");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGunMarker::Free()
{
	__super::Free();

	Safe_Release(m_pDSState);
	Safe_Release(m_pOldDSState);

	Safe_Release(m_pRasterState);
	Safe_Release(m_pOldRasterState);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
