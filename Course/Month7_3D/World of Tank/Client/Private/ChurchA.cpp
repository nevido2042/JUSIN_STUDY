#include "ChurchA.h"

#include "GameInstance.h"
#include "PickedManager.h"

CChurchA::CChurchA(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBuilding{ pDevice, pContext }
{

}

CChurchA::CChurchA(const CChurchA& Prototype)
	: CBuilding(Prototype)
{

}

HRESULT CChurchA::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CChurchA::Initialize(void* pArg)
{
	GAMEOBJECT_DESC			Desc{};
	Desc.fRotationPerSec = 0.f;
	Desc.fSpeedPerSec = 0.f;
	lstrcpy(Desc.szName, TEXT("ChurchA"));

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	return S_OK;
}


void CChurchA::Priority_Update(_float fTimeDelta)
{
	CBuilding::Priority_Update(fTimeDelta);
	//m_pGameInstance->Add_CollisionGroup(ENUM_CLASS(COLLISION_GROUP::BUILDING), this, TEXT("Com_Collider"));
}

void CChurchA::Update(_float fTimeDelta)
{
	CBuilding::Update(fTimeDelta);

	//m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CChurchA::Late_Update(_float fTimeDelta)
{
	//CBuilding::Late_Update(fTimeDelta);

	if (m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(STATE::POSITION), 50.f))
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CChurchA::Render()
{
	CBuilding::Render();

//	if (FAILED(Bind_ShaderResources()))
//		return E_FAIL;
//
//	if (m_pModelCom)
//	{
//		_uint		iNumMesh = m_pModelCom->Get_NumMeshes();
//
//		for (_uint i = 0; i < iNumMesh; i++)
//		{
//			if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
//				return E_FAIL;
//
//			if (FAILED(m_pShaderCom->Begin(0)))
//				return E_FAIL;
//
//			if (FAILED(m_pModelCom->Render(i)))
//				return E_FAIL;
//		}
//	}
//#ifdef _DEBUG
//	m_pColliderCom->Render();
//#endif
	return S_OK;
}

HRESULT CChurchA::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_ChurchA"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_OBB::OBB_DESC	OBBDesc{};
	OBBDesc.vExtents = _float3(8.f, 30.f, 14.f);
	OBBDesc.vCenter = _float3(0.0f, OBBDesc.vExtents.y, -11.f);
	OBBDesc.vRotation = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &OBBDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CChurchA::Bind_ShaderResources()
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

CChurchA* CChurchA::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CChurchA* pInstance = new CChurchA(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CChurchA");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CChurchA::Clone(void* pArg)
{
	CChurchA* pInstance = new CChurchA(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CChurchA");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChurchA::Free()
{
	__super::Free();

	//Safe_Release(m_pColliderCom);
	//Safe_Release(m_pModelCom);
	//Safe_Release(m_pShaderCom);
}
