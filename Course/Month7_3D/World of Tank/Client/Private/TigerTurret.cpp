#include "TigerTurret.h"

#include "GameInstance.h"

CTigerTurret::CTigerTurret(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTurret{ pDevice, pContext }
{

}

CTigerTurret::CTigerTurret(const CTigerTurret& Prototype)
	: CTurret(Prototype)
{

}

HRESULT CTigerTurret::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTigerTurret::Initialize(void* pArg)
{
	GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
	m_pParentWorldMatrix = pDesc->pParentWorldMatrix;
	m_iID = pDesc->iID;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	// 새 RasterizerState 설정
	D3D11_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_FRONT;
	rasterDesc.FrontCounterClockwise = FALSE;

	m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterState);

	return S_OK;
}


void CTigerTurret::Priority_Update(_float fTimeDelta)
{
	CTurret::Priority_Update(fTimeDelta);

}

void CTigerTurret::Update(_float fTimeDelta)
{
	CTurret::Update(fTimeDelta);

}

void CTigerTurret::Late_Update(_float fTimeDelta)
{

	CGameObject::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CTigerTurret::Render()
{
	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

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

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTigerTurret::SetUp_RenderState()
{
	m_pContext->RSGetState(&m_pOldRasterState);
	m_pContext->RSSetState(m_pRasterState);

	return S_OK;
}

HRESULT CTigerTurret::Release_RenderState()
{
	m_pContext->RSSetState(m_pOldRasterState);

	return S_OK;
}

HRESULT CTigerTurret::Bind_ShaderResources()
{
	//파트 오브젝트는 자기 트랜스폼 안써야한다
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_Light(0);

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

HRESULT CTigerTurret::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_TigerTurret"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CTigerTurret::Ready_PartObjects()
{
	/* 주포을 추가한다. */
	CGameObject::GAMEOBJECT_DESC Desc{};
	Desc.pParentWorldMatrix = &m_CombinedWorldMatrix;
	Desc.fRotationPerSec = 1.f;
	Desc.iID = m_iID;

	lstrcpy(Desc.szName, TEXT("TigerGun"));
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_TigerGun"), TEXT("Part_Gun"), &Desc)))
		return E_FAIL;

	return S_OK;
}

CTigerTurret* CTigerTurret::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTigerTurret* pInstance = new CTigerTurret(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTigerTurret");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTigerTurret::Clone(void* pArg)
{
	CTigerTurret* pInstance = new CTigerTurret(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTigerTurret");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTigerTurret::Free()
{
	__super::Free();

	for (auto& Pair : m_PartObjects)
		Safe_Release(Pair.second);
	m_PartObjects.clear();

	Safe_Release(m_pRasterState);
	Safe_Release(m_pOldRasterState);

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
