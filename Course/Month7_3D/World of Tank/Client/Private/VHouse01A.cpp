#include "VHouse01A.h"

#include "GameInstance.h"

CVHouse01A::CVHouse01A(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CVHouse01A::CVHouse01A(const CVHouse01A& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CVHouse01A::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVHouse01A::Initialize(void* pArg)
{
	GAMEOBJECT_DESC			Desc{};
	Desc.fRotationPerSec = 0.f;
	Desc.fSpeedPerSec = 0.f;
	lstrcpy(Desc.szName, TEXT("BurntTree"));

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}


void CVHouse01A::Priority_Update(_float fTimeDelta)
{

}

void CVHouse01A::Update(_float fTimeDelta)
{

}

void CVHouse01A::Late_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(STATE::POSITION), 1.f))
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CVHouse01A::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	/* dx9 : 장치에 뷰, 투영행렬을 저장해두면 렌더링시 알아서 정점에 Transform해주었다. */
	/* dx11 : 셰이더에 뷰, 투영행렬을 저장해두고 우리가 직접 변환해주어야한다. */

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

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
