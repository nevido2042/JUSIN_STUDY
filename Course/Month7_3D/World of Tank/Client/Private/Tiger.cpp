#include "Tiger.h"

#include "GameInstance.h"

CTiger::CTiger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLandObject{ pDevice, pContext }
{

}

CTiger::CTiger(const CTiger& Prototype)
	: CLandObject(Prototype)
{

}

HRESULT CTiger::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTiger::Initialize(void* pArg)
{
	GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

	LANDOBJECT_DESC		Desc{};
	Desc.fRotationPerSec = 5.f;
	Desc.fSpeedPerSec = 10.f;
	lstrcpy(Desc.szName, TEXT("Tiger"));

	Desc.iLevelIndex = m_pGameInstance->Get_NewLevel_Index();
	Desc.strLayerTag = TEXT("Layer_Terrain");
	Desc.strComponentTag = TEXT("Com_VIBuffer");
	Desc.iIndex = 0;
	Desc.vInitPosition = pDesc->vInitPosition;

	if (FAILED(__super::Initialize(&Desc)))
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


void CTiger::Priority_Update(_float fTimeDelta)
{

}

void CTiger::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Pressing(DIK_W))
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}
	if (m_pGameInstance->Key_Pressing(DIK_S))
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}
	if (m_pGameInstance->Key_Pressing(DIK_A))
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * -1.f);
	}
	if (m_pGameInstance->Key_Pressing(DIK_D))
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta);
	}

	if (m_pGameInstance->Key_Down(DIK_F1))
	{
		Destroyed();
	}

	__super::SetUp_Height_Normal(m_pTransformCom, fTimeDelta, 0.5f);
}

void CTiger::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CTiger::Render()
{
	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	/* dx9 : 장치에 뷰, 투영행렬을 저장해두면 렌더링시 알아서 정점에 Transform해주었다. */
	/* dx11 : 셰이더에 뷰, 투영행렬을 저장해두고 우리가 직접 변환해주어야한다. */

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;


	if (m_pModelCom && !m_bDestroyed)
	{
		_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

		for (_uint i = 0; i < iNumMesh; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Begin(0)))
				return E_FAIL;

			if (FAILED(m_pModelCom->Render(i)))
				return E_FAIL;
		}
	}
	else if (m_pModelCom_Destroyed)
	{
		_uint		iNumMesh = m_pModelCom_Destroyed->Get_NumMeshes();

		for (_uint i = 0; i < iNumMesh; i++)
		{
			if (FAILED(m_pModelCom_Destroyed->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Begin(0)))
				return E_FAIL;

			if (FAILED(m_pModelCom_Destroyed->Render(i)))
				return E_FAIL;
		}
	}

	if (FAILED(Release_RenderState()))
		return E_FAIL;


	return S_OK;
}

void CTiger::Destroyed()
{
	m_bDestroyed = true;
}

HRESULT CTiger::SetUp_RenderState()
{
	m_pContext->RSGetState(&m_pOldRasterState);
	m_pContext->RSSetState(m_pRasterState);

	return S_OK;
}

HRESULT CTiger::Release_RenderState()
{
	m_pContext->RSSetState(m_pOldRasterState);

	return S_OK;
}

HRESULT CTiger::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_TigerBody"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Model_Destroyed */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_TigerDestroyed"),
		TEXT("Com_Model_Destroyed"), reinterpret_cast<CComponent**>(&m_pModelCom_Destroyed))))
		return E_FAIL;

	return S_OK;
}

CTiger* CTiger::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTiger* pInstance = new CTiger(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTiger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTiger::Clone(void* pArg)
{
	CTiger* pInstance = new CTiger(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTiger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTiger::Free()
{
	__super::Free();

	Safe_Release(m_pRasterState);
	Safe_Release(m_pOldRasterState);

	Safe_Release(m_pModelCom_Destroyed);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
