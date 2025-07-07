#include "Missile.h"

#include "GameInstance.h"
#include "VIBuffer_Terrain.h"

CMissile::CMissile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CMissile::CMissile(const CMissile& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CMissile::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMissile::Initialize(void* pArg)
{
	GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
	(*pDesc).fSpeedPerSec = 1.f;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f));

	m_pTargetBuffer = static_cast<CVIBuffer*>(m_pGameInstance->Get_Component(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Terrain"), TEXT("Com_VIBuffer"), 0));
	Safe_AddRef(m_pTargetBuffer);
	if (nullptr == m_pTargetBuffer)
		return E_FAIL;

	return S_OK;
}


void CMissile::Priority_Update(_float fTimeDelta)
{


}

void CMissile::Update(_float fTimeDelta)
{
	m_pTransformCom->Go_Straight(fTimeDelta);


#pragma region ¶¥¿¡ ¶³¾îÁö¸é ÆÄ°í, ÀÌÆåÆ® »ý¼º
	_float vTerrainHeight = XMVectorGetY(m_pTargetBuffer->Compute_HeightPosition(m_pTransformCom->Get_State(STATE::POSITION)));
	_float vShellHeight = XMVectorGetY(m_pTransformCom->Get_State(STATE::POSITION));

	if (vTerrainHeight > vShellHeight)
	{
		_vector vTerrainCenter = m_pTargetBuffer->Compute_HeightPosition(XMVectorSet(TERRAIN_SIZE * TERRAIN_OFFSET_WIDTH * 0.5f,  0.f, TERRAIN_SIZE * TERRAIN_OFFSET_WIDTH * 0.5f, 1.f));
		_float3 vDigCenter = {};
		XMStoreFloat3(&vDigCenter, vTerrainCenter);
		static_cast<CVIBuffer_Terrain*>(m_pTargetBuffer)->DigGround(vDigCenter, TERRAIN_SIZE, 100.f);

		Destroy();
	}
#pragma endregion
}

void CMissile::Late_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(STATE::POSITION), 10.f))
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);

}

HRESULT CMissile::Render()
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

			if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS, 0)))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Begin(0)))
				return E_FAIL;

			if (FAILED(m_pModelCom->Render(i)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CMissile::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Missile"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMissile::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	return S_OK;
}

CMissile* CMissile::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMissile* pInstance = new CMissile(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMissile");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMissile::Clone(void* pArg)
{
	CMissile* pInstance = new CMissile(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMissile");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMissile::Free()
{
	__super::Free();

	Safe_Release(m_pTargetBuffer);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);

}
