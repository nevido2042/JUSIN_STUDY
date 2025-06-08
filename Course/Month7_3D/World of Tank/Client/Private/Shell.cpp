#include "Shell.h"

#include "GameInstance.h"

CShell::CShell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CShell::CShell(const CShell& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CShell::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CShell::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}


void CShell::Priority_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_CollisionGroup(ENUM_CLASS(COLLISION_GROUP::SHELL), this, TEXT("Com_Collider"));
}

void CShell::Update(_float fTimeDelta)
{
	m_pTransformCom->Apply_Gravity(9.8f, fTimeDelta);
	m_pTransformCom->Move_Velocity(fTimeDelta);

	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));
	//m_pGameInstance->Check_Collision(ENUM_CLASS(COLLISION_GROUP::MODULE), this, TEXT("Com_Collider"), TEXT("Com_Collider"));

	//50보다 낮아지면 없애라
	if (XMVectorGetY(m_pTransformCom->Get_State(STATE::POSITION)) < 50.f)
		Destroy();
}

void CShell::Late_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(STATE::POSITION), 0.1f))
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);

}

HRESULT CShell::Render()
{
	/*if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (m_pModelCom)
	{
		_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

		for (_uint i = 0; i < iNumMesh; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Begin(2)))
				return E_FAIL;

			if (FAILED(m_pModelCom->Render(i)))
				return E_FAIL;
		}
	}*/

	m_pColliderCom->Render();

	return S_OK;
}

void CShell::On_Collision_Stay(CGameObject* pOther)
{	
	wcout << "Shell On_Collision_Stay: " << pOther->GetName() << endl;
	Destroy();
}


HRESULT CShell::Ready_Components()
{
	///* For.Com_Shader */
	//if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
	//	TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	//	return E_FAIL;

	///* For.Com_Model */
	//if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Fury"),
	//	TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
	//	return E_FAIL;

	/* For.Com_Collider */
	CBounding_Sphere::SPHERE_DESC	SphereDesc{};
	SphereDesc.fRadius = 0.5f;
	SphereDesc.vCenter = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CShell::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
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

CShell* CShell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CShell* pInstance = new CShell(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CShell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CShell::Clone(void* pArg)
{
	CShell* pInstance = new CShell(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CShell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShell::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	//Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
