#include "Fury.h"

#include "GameInstance.h"

CFury::CFury(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLandObject{ pDevice, pContext }
{

}

CFury::CFury(const CFury& Prototype)
	: CLandObject(Prototype)
{

}

HRESULT CFury::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFury::Initialize(void* pArg)
{
	LANDOBJECT_DESC		Desc{};
	Desc.fRotationPerSec = 5.f;
	Desc.fSpeedPerSec = 10.f;
	lstrcpy(Desc.szName, TEXT("Fury"));

	Desc.iLevelIndex = ENUM_CLASS(LEVEL::PRACTICE);
	Desc.strLayerTag = TEXT("Layer_Terrain");
	Desc.strComponentTag = TEXT("Com_VIBuffer");
	Desc.iIndex = 0;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}


void CFury::Priority_Update(_float fTimeDelta)
{

}

void CFury::Update(_float fTimeDelta)
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
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f,0.f), fTimeDelta);
	}

	__super::SetUp_HeightPosition(m_pTransformCom, 0.5f);
}

void CFury::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CFury::Render()
{
	// ���� RasterizerState ����
	ID3D11RasterizerState* pOldRasterState = nullptr;
	m_pContext->RSGetState(&pOldRasterState);

	// �� RasterizerState ����
	D3D11_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_FRONT;
	rasterDesc.FrontCounterClockwise = FALSE;

	ID3D11RasterizerState* pRasterState = nullptr;
	m_pDevice->CreateRasterizerState(&rasterDesc, &pRasterState);
	m_pContext->RSSetState(pRasterState);



	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	/* dx9 : ��ġ�� ��, ��������� �����صθ� �������� �˾Ƽ� ������ Transform���־���. */
	/* dx11 : ���̴��� ��, ��������� �����صΰ� �츮�� ���� ��ȯ���־���Ѵ�. */

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;


	if (m_pModelCom)
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

	// �ٽ� ���󺹱�
	m_pContext->RSSetState(pOldRasterState);

	// ����
	Safe_Release(pRasterState);
	Safe_Release(pOldRasterState);


	return S_OK;
}

HRESULT CFury::SetUp_RenderState()
{
	return E_NOTIMPL;
}

HRESULT CFury::Release_RenderState()
{
	return E_NOTIMPL;
}

HRESULT CFury::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Fury"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

CFury* CFury::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFury* pInstance = new CFury(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFury");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFury::Clone(void* pArg)
{
	CFury* pInstance = new CFury(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CFury");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFury::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
