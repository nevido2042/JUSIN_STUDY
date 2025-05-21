#include "Tiger.h"
#include "FuryTrackLeft.h"

#include "GameInstance.h"

CFuryTrackLeft::CFuryTrackLeft(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CFuryTrackLeft::CFuryTrackLeft(const CFuryTrackLeft& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CFuryTrackLeft::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFuryTrackLeft::Initialize(void* pArg)
{
	GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
	m_pParentWorldMatrix = pDesc->pParentWorldMatrix;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	// 새 RasterizerState 설정
	D3D11_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_FRONT;
	rasterDesc.FrontCounterClockwise = FALSE;

	m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterState);

	return S_OK;
}


void CFuryTrackLeft::Priority_Update(_float fTimeDelta)
{

}

void CFuryTrackLeft::Update(_float fTimeDelta)
{
	m_fUVScrollY += m_fSpeed * 0.05f;
	m_fUVScrollY = fmodf(m_fUVScrollY, 1.0f); // 0~1 사이 유지
}

void CFuryTrackLeft::Late_Update(_float fTimeDelta)
{
	//부모의 월드 행렬을 가져와서 자신의 월드 행렬과 곱해준다.
	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixMultiply(m_pTransformCom->Get_WorldMatrix(), XMLoadFloat4x4(m_pParentWorldMatrix)));

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CFuryTrackLeft::Render()
{
	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	//파트 오브젝트는 자기 트랜스폼 안써야한다
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	if (m_pModelCom)
	{
		_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

		for (_uint i = 0; i < 2; i++)
		{
			if (i == 1)
			{
				_float2 Offset{ 0.f, m_fUVScrollY };
				m_pShaderCom->Bind_Float2("g_UVOffset", &Offset);
			}

			if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Begin(0)))
				return E_FAIL;

			if (FAILED(m_pModelCom->Render(i)))
				return E_FAIL;

			if (i == 1)
			{
				_float2 zeroOffset = { 0.f, 0.f };
				m_pShaderCom->Bind_Float2("g_UVOffset", &zeroOffset);
			}
		}
	}

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFuryTrackLeft::SetUp_RenderState()
{
	m_pContext->RSGetState(&m_pOldRasterState);
	m_pContext->RSSetState(m_pRasterState);

	return S_OK;
}

HRESULT CFuryTrackLeft::Release_RenderState()
{
	m_pContext->RSSetState(m_pOldRasterState);

	return S_OK;
}

HRESULT CFuryTrackLeft::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_FuryTrackLeft"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

CFuryTrackLeft* CFuryTrackLeft::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFuryTrackLeft* pInstance = new CFuryTrackLeft(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFuryTrackLeft");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFuryTrackLeft::Clone(void* pArg)
{
	CFuryTrackLeft* pInstance = new CFuryTrackLeft(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CFuryTrackLeft");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFuryTrackLeft::Free()
{
	__super::Free();

	Safe_Release(m_pRasterState);
	Safe_Release(m_pOldRasterState);

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
