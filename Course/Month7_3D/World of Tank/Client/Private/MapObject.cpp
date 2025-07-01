#include "MapObject.h"

#include "GameInstance.h"

CMapObject::CMapObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CMapObject::CMapObject(const CMapObject& Prototype)
	: CGameObject(Prototype)
{
	
}

HRESULT CMapObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMapObject::Initialize(void* pArg)
{
	MAPOBJECT_DESC*			pDesc{};
	pDesc = static_cast<MAPOBJECT_DESC*>(pArg);
	pDesc->fRotationPerSec = 0.f;
	pDesc->fSpeedPerSec = 0.f;
	lstrcpy(pDesc->szName, TEXT("MapObject"));
	m_wstrComModel = pDesc->wstrModelCom;

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components(pDesc->wstrModelCom)))
		return E_FAIL;

	return S_OK;
}


void CMapObject::Priority_Update(_float fTimeDelta)
{

}

void CMapObject::Update(_float fTimeDelta)
{

}

void CMapObject::Late_Update(_float fTimeDelta)
{
	if(m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(STATE::POSITION), 50.f))
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);

}

HRESULT CMapObject::Render()
{
	if (m_eCullMode == D3D11_CULL_FRONT)
	{
#pragma message("컬모드 프론트 어떻게 구분해야할지 고민")
		Render_Cull_Front();
	}
	else
	{
		if (FAILED(Bind_ShaderResources()))
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
	}

	return S_OK;
}

HRESULT CMapObject::Render_Cull_Front()
{
	// 기존 RasterizerState 저장
	ID3D11RasterizerState* pOldRasterState = nullptr;
	m_pContext->RSGetState(&pOldRasterState);

	// 새 RasterizerState 설정 (컬링 안 함)
	D3D11_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_FRONT;
	rasterDesc.FrontCounterClockwise = FALSE;

	ID3D11RasterizerState* pRasterState = nullptr;
	m_pDevice->CreateRasterizerState(&rasterDesc, &pRasterState);
	m_pContext->RSSetState(pRasterState);

	if (FAILED(Bind_ShaderResources()))
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

	// 다시 원상복구
	m_pContext->RSSetState(pOldRasterState);

	// 해제
	Safe_Release(pRasterState);
	Safe_Release(pOldRasterState);


	return S_OK;
}

HRESULT CMapObject::Ready_Components(wstring wstrComModel)
{

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), wstrComModel,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMapObject::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;
#pragma message ("여기만 일단 했고, 나머지 건물이나 배치할 오브젝트는 나중에 신경써 주자")
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

CMapObject* CMapObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMapObject* pInstance = new CMapObject(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMapObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMapObject::Clone(void* pArg)
{
	CMapObject* pInstance = new CMapObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMapObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMapObject::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
