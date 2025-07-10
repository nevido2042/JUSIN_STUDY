#include "Building.h"

#include "GameInstance.h"
#include "PickedManager.h"

CBuilding::CBuilding(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLandObject{ pDevice, pContext }
{

}

CBuilding::CBuilding(const CBuilding& Prototype)
	: CLandObject(Prototype)
{
	for (_uint i = 0; i < 4; ++i)
	{
		m_vLocalPoint[i] = Prototype.m_vLocalPoint[i];
	}
}

HRESULT CBuilding::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBuilding::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}


void CBuilding::Priority_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_CollisionGroup(ENUM_CLASS(COLLISION_GROUP::BUILDING), this, TEXT("Com_Collider"));
}

void CBuilding::Update(_float fTimeDelta)
{
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

	//CLandObject::SetUp_Height_Normal(m_pTransformCom, fTimeDelta, 0.5f);
	OnGround(fTimeDelta);
}

void CBuilding::Late_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(STATE::POSITION), 50.f))
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_SHADOW, this);

	if (m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(STATE::POSITION), 50.f))
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);

#ifdef _DEBUG
	//m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif
}

HRESULT CBuilding::Render()
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
#ifdef _DEBUG
	m_pColliderCom->Render();
#endif
	return S_OK;
}

HRESULT CBuilding::Render_Shadow()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Light_ViewMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Light_ProjMatrix())))
		return E_FAIL;

	_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMesh; i++)
	{
		if (FAILED(m_pShaderCom->Begin(4)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

void CBuilding::OnGround(_float fTimeDelta)
{
#pragma region 노말 평균

	_vector vWorldPoint[4] = {};

	_vector vNormalSum = XMVectorZero();
	for (_int i = 0; i < 4; ++i)
	{
		vWorldPoint[i] = XMVector3TransformCoord(XMVectorSetW(XMLoadFloat3(&m_vLocalPoint[i]), 1.f), m_pTransformCom->Get_WorldMatrix());
		vNormalSum += m_pTargetBuffer->Compute_NormalPosition(vWorldPoint[i]);
	}
	_vector vTargetUp = XMVector3Normalize(vNormalSum / 4.f);

	// 이전 Up 벡터 가져오기
	_vector vCurrentUp = m_pTransformCom->Get_State(STATE::UP);

	// 선형 보간으로 부드럽게 변화
	const _float fLerpSpeed = 20.f; // 클수록 빠르게 보간됨
	vTargetUp = XMVector3Normalize(XMVectorLerp(vCurrentUp, vTargetUp, fLerpSpeed * fTimeDelta));

	// Look 벡터는 이전 유지
	_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(STATE::LOOK));

	// Up과 Look으로부터 Right 재계산
	_vector vRight = XMVector3Normalize(XMVector3Cross(vTargetUp, vLook));

	// Right와 Up으로 Look 재계산
	vLook = XMVector3Normalize(XMVector3Cross(vRight, vTargetUp));

	// 방향 벡터 적용
	m_pTransformCom->Set_State(STATE::RIGHT, vRight);
	m_pTransformCom->Set_State(STATE::UP, vTargetUp);
	m_pTransformCom->Set_State(STATE::LOOK, vLook);

#pragma endregion

#pragma region 높이
	_float  fHeightSum = {};
	for (_int i = 0; i < 4; ++i)
	{
		fHeightSum += XMVectorGetY(m_pTargetBuffer->Compute_HeightPosition(vWorldPoint[i]));
	}
	_float fHeight = fHeightSum / 4.f;

	// 최종 위치 설정
	_vector vPosition = m_pTransformCom->Get_State(STATE::POSITION);
	vPosition = XMVectorSetY(vPosition, fHeight);

	m_pTransformCom->Set_State(STATE::POSITION, vPosition);
#pragma endregion
}

HRESULT CBuilding::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	return S_OK;
}

void CBuilding::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
