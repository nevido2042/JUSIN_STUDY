#include "VHouse01A.h"

#include "GameInstance.h"

CVHouse01A::CVHouse01A(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBuilding{ pDevice, pContext }
{

}

CVHouse01A::CVHouse01A(const CVHouse01A& Prototype)
	: CBuilding(Prototype)
{

}

HRESULT CVHouse01A::Initialize_Prototype()
{
	m_vLocalPoint[0] = _float3{ 8.f, 0.f, 4.5f };
	m_vLocalPoint[1] = _float3{ -8.f, 0.f, 4.5f };
	m_vLocalPoint[2] = _float3{ 8.f, 0.f, -4.5f };
	m_vLocalPoint[3] = _float3{ -8.f, 0.f, -4.5f };

	return S_OK;
}

HRESULT CVHouse01A::Initialize(void* pArg)
{
	LANDOBJECT_DESC			Desc{};
	Desc.iLevelIndex = m_pGameInstance->Get_NewLevel_Index();
	Desc.strLayerTag = TEXT("Layer_Terrain");
	Desc.strComponentTag = TEXT("Com_VIBuffer");
	Desc.iIndex = 0;
	lstrcpy(Desc.szName, TEXT("VHouse01A"));

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;



	return S_OK;
}


void CVHouse01A::Priority_Update(_float fTimeDelta)
{
	CBuilding::Priority_Update(fTimeDelta);

}

void CVHouse01A::Update(_float fTimeDelta)
{
	CBuilding::Update(fTimeDelta);


}

void CVHouse01A::Late_Update(_float fTimeDelta)
{

	if (m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(STATE::POSITION), 20.f))
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);

#ifdef _DEBUG
	m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif
}

HRESULT CVHouse01A::Render()
{
	CBuilding::Render();


	return S_OK;
}

HRESULT CVHouse01A::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_VHouse01A"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_OBB::OBB_DESC	OBBDesc{};
	OBBDesc.vExtents = _float3(8.f, 4.5f, 4.5f);
	OBBDesc.vCenter = _float3(1.5f, OBBDesc.vExtents.y, 0.f);
	OBBDesc.vRotation = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &OBBDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVHouse01A::Bind_ShaderResources()
{
	CBuilding::Bind_ShaderResources();

	//if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
	//	return E_FAIL;

	//const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_Light(0);

	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
	//	return E_FAIL;

	return S_OK;
}

CVHouse01A* CVHouse01A::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVHouse01A* pInstance = new CVHouse01A(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVHouse01A");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVHouse01A::Clone(void* pArg)
{
	CVHouse01A* pInstance = new CVHouse01A(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVHouse01A");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVHouse01A::Free()
{
	__super::Free();

	//Safe_Release(m_pColliderCom);
	//Safe_Release(m_pModelCom);
	//Safe_Release(m_pShaderCom);
}
