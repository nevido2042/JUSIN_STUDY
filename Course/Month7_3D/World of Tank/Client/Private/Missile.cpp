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
	//(*pDesc).fSpeedPerSec = 20.f;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pSoundCom->Set3DState(0.f, 500.f);

	m_pSoundCom->Play("rocket_accelerator_10");

	XMStoreFloat3(&vStartPos, m_pTransformCom->Get_State(STATE::POSITION));

	m_pTransformCom->Scaling(10.f, 10.f, 10.f);

	m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-90.f));

	m_pTargetBuffer = static_cast<CVIBuffer*>(m_pGameInstance->Get_Component(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Terrain"), TEXT("Com_VIBuffer"), 0));
	Safe_AddRef(m_pTargetBuffer);
	if (nullptr == m_pTargetBuffer)
		return E_FAIL;

	//카메라 가져옴
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
	m_pSoundCom->Update3DPosition(m_pTransformCom->Get_State(STATE::POSITION));

	//m_pTransformCom->Go_Straight(fTimeDelta);

	// t: 현재 시간, dt: 한 프레임 시간 (ex. 0.01)

	_float3 vControlPos = { TERRAIN_SIZE * TERRAIN_OFFSET_WIDTH * 0.25f, 500.f, TERRAIN_SIZE * TERRAIN_OFFSET_WIDTH * 0.25f };
	_float3 vEndPos = { TERRAIN_SIZE * TERRAIN_OFFSET_WIDTH * 0.5f, 0.f, TERRAIN_SIZE * TERRAIN_OFFSET_WIDTH * 0.5f };

	_vector vCurPos = Bezier2(vStartPos, vControlPos, vEndPos, fProgress);
	_vector vNextPos = Bezier2(vStartPos, vControlPos, vEndPos, fProgress + fTimeDelta * 0.05f);

	//_vector vDir = XMVector3Normalize(vNextPos - vCurPos);

	// 회전 행렬로 변환하거나, LookAt 방식으로 방향 적용
	//Matrix rotation = LookAtMatrix(dir); // 또는 QuaternionLookRotation(dir)

	m_pTransformCom->Set_State(STATE::POSITION, vCurPos);

	m_pTransformCom->LookAt(vNextPos);

	fProgress += fTimeDelta * 0.05f;

#pragma region 떨어지는 동안 카메라 쉐이크
	//카메라 셰이크
	if (m_pCameraTPS && m_pCameraTPS->Get_isActive())
	{
		m_pCameraTPS->Start_CameraShake(0.1f, 0.2f);
	}
#pragma endregion

#pragma region 땅에 떨어지면 파고, 이펙트 생성, 카메라 크게 쉐이크
	_float vTerrainHeight = XMVectorGetY(m_pTargetBuffer->Compute_HeightPosition(m_pTransformCom->Get_State(STATE::POSITION)));
	_float vShellHeight = XMVectorGetY(m_pTransformCom->Get_State(STATE::POSITION));

	if (vTerrainHeight > vShellHeight)
	{
		m_pSoundCom->Play("explosive_objects_35");

		//_vector vTerrainCenter = m_pTargetBuffer->Compute_HeightPosition(XMVectorSet(TERRAIN_SIZE * TERRAIN_OFFSET_WIDTH * 0.5f,  0.f, TERRAIN_SIZE * TERRAIN_OFFSET_WIDTH * 0.5f, 1.f));
		_vector vColl = m_pTargetBuffer->Compute_HeightPosition(m_pTransformCom->Get_State(STATE::POSITION));
		_float3 vDigCenter = {};
		XMStoreFloat3(&vDigCenter, vColl);
		static_cast<CVIBuffer_Terrain*>(m_pTargetBuffer)->DigGround(vDigCenter, TERRAIN_SIZE, 50.f);

		//폭발 파티클
		GAMEOBJECT_DESC Desc{};
		XMStoreFloat3(&Desc.vInitPosition, m_pTransformCom->Get_State(STATE::POSITION));
		m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_MissileExplosionEffect"), m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_MissileExplosionEffect"), &Desc);

		//모든 탱크 파괴
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

		//모든 나무 파괴
		for (CGameObject* pBurntTree : m_pGameInstance->Find_Layer(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_BurntTree"))->Get_GameObjects())
		{
			pBurntTree->Destroy();
		}

		//모든 풀 파괴
		CGameObject* pMapVegetation = m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_MapVegetation"));
		if(pMapVegetation)
			pMapVegetation->Destroy();

		//카메라 셰이크
		if (m_pCameraTPS && m_pCameraTPS->Get_isActive())
		{
			m_pCameraTPS->Start_CameraShake(1.f, 0.5f);
		}

		m_bActive = false;
		m_pSoundCom->Stop("rocket_accelerator_10");
		//Destroy();
	}

#pragma endregion

	CGameObject::Update(fTimeDelta);
}

void CMissile::Late_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(STATE::POSITION), 50.f))
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

_vector CMissile::Bezier2(_float3 vStart, _float3 vControl, _float3 vEnd, _float fT)
{
	_vector vA = XMVectorSetW(XMLoadFloat3(&vStart), 1.f);
	_vector vB = XMVectorSetW(XMLoadFloat3(&vControl), 1.f);
	_vector vC = XMVectorSetW(XMLoadFloat3(&vEnd), 1.f);

	_vector vAB = XMVectorLerp(vA, vB, fT);
	_vector vBC = XMVectorLerp(vB, vC, fT);
	return XMVectorLerp(vAB, vBC, fT);
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

	/* For.Com_Sound */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController_Missile"),
		TEXT("Com_Sound"), reinterpret_cast<CComponent**>(&m_pSoundCom))))
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
	Safe_Release(m_pSoundCom);

}
