#include "Missile.h"

#include "GameInstance.h"
#include "VIBuffer_Terrain.h"

#include "Layer.h"
#include "Tank.h"

#include "Camera_TPS.h"

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
	(*pDesc).fSpeedPerSec = 20.f;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(10.f, 10.f, 10.f);

	m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f));

	m_pTargetBuffer = static_cast<CVIBuffer*>(m_pGameInstance->Get_Component(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Terrain"), TEXT("Com_VIBuffer"), 0));
	Safe_AddRef(m_pTargetBuffer);
	if (nullptr == m_pTargetBuffer)
		return E_FAIL;

	//Ä«¸Þ¶ó °¡Á®¿È
	CLayer* pLayer = nullptr;
	pLayer = m_pGameInstance->Find_Layer(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Camera"));
	if (pLayer)
	{
		CGameObject* pObject = pLayer->Find_GameObject_By_Name(TEXT("Camera_TPS"));

		if (pObject)
		{
			m_pCameraTPS = static_cast<CCamera_TPS*>(pObject);
			Safe_AddRef(m_pCameraTPS);
		}

	}


	if(FAILED(Ready_PartObjects()))
		return E_FAIL;

	return S_OK;
}


void CMissile::Priority_Update(_float fTimeDelta)
{

	CGameObject::Priority_Update(fTimeDelta);
}

void CMissile::Update(_float fTimeDelta)
{
	m_pTransformCom->Go_Straight(fTimeDelta);

#pragma region ¶³¾îÁö´Â µ¿¾È Ä«¸Þ¶ó ½¦ÀÌÅ©
	//Ä«¸Þ¶ó ¼ÎÀÌÅ©
	if (m_pCameraTPS && m_pCameraTPS->Get_isActive())
	{
		m_pCameraTPS->Start_CameraShake(0.1f, 0.2f);
	}
#pragma endregion

#pragma region ¶¥¿¡ ¶³¾îÁö¸é ÆÄ°í, ÀÌÆåÆ® »ý¼º, Ä«¸Þ¶ó Å©°Ô ½¦ÀÌÅ©
	_float vTerrainHeight = XMVectorGetY(m_pTargetBuffer->Compute_HeightPosition(m_pTransformCom->Get_State(STATE::POSITION)));
	_float vShellHeight = XMVectorGetY(m_pTransformCom->Get_State(STATE::POSITION));

	if (vTerrainHeight > vShellHeight)
	{
		_vector vTerrainCenter = m_pTargetBuffer->Compute_HeightPosition(XMVectorSet(TERRAIN_SIZE * TERRAIN_OFFSET_WIDTH * 0.5f,  0.f, TERRAIN_SIZE * TERRAIN_OFFSET_WIDTH * 0.5f, 1.f));
		_float3 vDigCenter = {};
		XMStoreFloat3(&vDigCenter, vTerrainCenter);
		static_cast<CVIBuffer_Terrain*>(m_pTargetBuffer)->DigGround(vDigCenter, TERRAIN_SIZE, 30.f);

		//Æø¹ß ÆÄÆ¼Å¬
		GAMEOBJECT_DESC Desc{};
		XMStoreFloat3(&Desc.vInitPosition, m_pTransformCom->Get_State(STATE::POSITION));
		m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_MissileExplosionEffect"), m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_MissileExplosionEffect"), &Desc);

		//¸ðµç ÅÊÅ© ÆÄ±«
		CGameObject* pGameObject = m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_PlayerTank"));
		if (pGameObject)
		{
			static_cast<CTank*>(pGameObject)->Take_Damage(999.f, nullptr, vDigCenter);
		}

		CLayer* pLayer = m_pGameInstance->Find_Layer(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Tank"));
		if (pLayer)
		{
			for (CGameObject* pGameObject : pLayer->Get_GameObjects())
			{
				static_cast<CTank*>(pGameObject)->Take_Damage(999.f, nullptr, vDigCenter);
			}
		}

		//¸ðµç ³ª¹« ÆÄ±«
		for (CGameObject* pBurntTree : m_pGameInstance->Find_Layer(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_BurntTree"))->Get_GameObjects())
		{
			pBurntTree->Destroy();
		}

		//¸ðµç Ç® ÆÄ±«
		m_pGameInstance->Get_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_MapVegetation"), 0)->Destroy();

		//Ä«¸Þ¶ó ¼ÎÀÌÅ©
		if (m_pCameraTPS && m_pCameraTPS->Get_isActive())
		{
			m_pCameraTPS->Start_CameraShake(1.f, 0.5f);
		}

		Destroy();
	}

#pragma endregion

	CGameObject::Update(fTimeDelta);
}

void CMissile::Late_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(STATE::POSITION), 10.f))
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);

	CGameObject::Late_Update(fTimeDelta);
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

HRESULT CMissile::Ready_PartObjects()
{
	GAMEOBJECT_DESC SmokeDesc = {};
	SmokeDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	SmokeDesc.vInitPosition = _float3(0.f, 0.f, -2.0f);
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_MissileSmoke"), TEXT("Part_Smoke"), &SmokeDesc)))
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

	for (auto& Pair : m_PartObjects)
		Safe_Release(Pair.second);
	m_PartObjects.clear();

	Safe_Release(m_pTargetBuffer);
	Safe_Release(m_pCameraTPS);

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);

}
