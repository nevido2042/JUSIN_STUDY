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
	//m_pGameInstance->Add_CollisionGroup(ENUM_CLASS(COLLISION_GROUP::BUILDING), this, TEXT("Com_Collider"));
}

void CVHouse01A::Update(_float fTimeDelta)
{
	CBuilding::Update(fTimeDelta);

	//m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CVHouse01A::Late_Update(_float fTimeDelta)
{
	//CBuilding::Late_Update(fTimeDelta);

	if (m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(STATE::POSITION), 20.f))
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CVHouse01A::Render()
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
//
//#ifdef _DEBUG
//	m_pColliderCom->Render();
//#endif

	return S_OK;
}

void CVHouse01A::OnGround(_float fTimeDelta)
{// 현재 위치에서 지형 높이 계산
	_vector vPosition = m_pTransformCom->Get_State(STATE::POSITION);
	vPosition = m_pTargetBuffer->Compute_HeightPosition(vPosition);

	//x,z임
	_float2 vCenter = { 1.5f, 0.f };

	_float2 Pos[4] =
	{
		{vCenter.x + 8.f, vCenter.y + 4.5f },
		{vCenter.x + 8.f, vCenter.y - 4.5f },
		{vCenter.x - 8.f, vCenter.y - 4.5f },
		{vCenter.x - 8.f, vCenter.y + 4.5f },
	};

	_vector vOffsets[4] =
	{
	XMVectorSet(Pos[0].x, 0.f, Pos[0].y, 0.f),
	XMVectorSet(Pos[1].x, 0.f, Pos[1].y, 0.f),
	XMVectorSet(Pos[2].x, 0.f, Pos[2].y, 0.f),
	XMVectorSet(Pos[3].x, 0.f, Pos[3].y, 0.f)
	};

	// 4개 위치 노말 평균
	_vector vNormalSum = XMVectorZero();
	for (_int i = 0; i < 4; ++i)
	{
		_vector vSamplePos = vPosition + vOffsets[i];
		vSamplePos = m_pTargetBuffer->Compute_HeightPosition(vSamplePos);
		vNormalSum += m_pTargetBuffer->Compute_NormalPosition(vSamplePos);
	}
	_vector vTargetUp = XMVector3Normalize(vNormalSum / 4.f);

	// 이전 Up 벡터 가져오기
	_vector vCurrentUp = m_pTransformCom->Get_State(STATE::UP);

	// 선형 보간으로 부드럽게 변화
	const _float fLerpSpeed = 5.f; // 클수록 빠르게 보간됨
	vTargetUp = XMVector3Normalize(XMVectorLerp(vCurrentUp, vTargetUp, fLerpSpeed * fTimeDelta));

	// Look 벡터는 이전 유지
	_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(STATE::LOOK));

	// Up과 Look으로부터 Right 재계산
	_vector vRight = XMVector3Normalize(XMVector3Cross(vTargetUp, vLook));

	// Right와 Up으로 Look 재계산
	vLook = XMVector3Normalize(XMVector3Cross(vRight, vTargetUp));

	// 최종 위치 설정
	vPosition = XMVectorSetY(vPosition, XMVectorGetY(vPosition) + 0.5f);
	m_pTransformCom->Set_State(STATE::POSITION, vPosition);

	// 방향 벡터 적용
	m_pTransformCom->Set_State(STATE::RIGHT, vRight);
	m_pTransformCom->Set_State(STATE::UP, vTargetUp);
	m_pTransformCom->Set_State(STATE::LOOK, vLook);
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
