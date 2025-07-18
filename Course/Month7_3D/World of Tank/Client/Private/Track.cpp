#include "Track.h"

#include "GameInstance.h"
#include "Module.h"
#include "Chassis.h"

CTrack::CTrack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CTrack::CTrack(const CTrack& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CTrack::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTrack::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}


void CTrack::Priority_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_CollisionGroup(ENUM_CLASS(COLLISION_GROUP::MODULE), this, TEXT("Com_Collider"));

}

void CTrack::Update(_float fTimeDelta)
{
	m_fUVScrollY += m_fSpeed * 0.075f;
	m_fUVScrollY = fmodf(m_fUVScrollY, 1.0f); // 0~1 사이 유지

	//부모의 월드 행렬을 가져와서 자신의 월드 행렬과 곱해준다.
	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixMultiply(m_pTransformCom->Get_WorldMatrix(), XMLoadFloat4x4(m_pParentWorldMatrix)));

	m_pColliderCom->Update(XMLoadFloat4x4(&m_CombinedWorldMatrix));

}

void CTrack::Late_Update(_float fTimeDelta)
{

	// 위치만 추출
	_vector vPos = XMVectorSet(
		m_CombinedWorldMatrix._41,
		m_CombinedWorldMatrix._42,
		m_CombinedWorldMatrix._43,
		1.f
	);


	if (static_cast<CChassis*>(m_pParent)->Get_isPicked() && m_pGameInstance->Is_In_Frustum(vPos, 2.f))
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_OUTLINE, this);

	if (m_pGameInstance->Is_In_Frustum(vPos, 2.f) && m_pParent->Get_isVisible())
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_SHADOW, this);

	// 프러스텀 안에 있으면 렌더링 추가
	if (m_pGameInstance->Is_In_Frustum(vPos, 2.f))
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);


#ifdef _DEBUG
	//m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif
	//m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CTrack::Render()
{

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

#pragma region 외곽선
	//if (m_pModelCom && static_cast<CChassis*>(m_pParent)->Get_isPicked())
	//{
	//	_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

	//	for (_uint i = 0; i < iNumMesh; i++)
	//	{
	//		if (FAILED(m_pShaderCom->Begin(3)))
	//			return E_FAIL;

	//		if (FAILED(m_pModelCom->Render(i)))
	//			return E_FAIL;
	//	}
	//}
#pragma endregion

	if (m_pModelCom)
	{
		_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

		for (_uint i = 0; i < 2; i++)
		{
			if (i == 1)
			{
				_float2 Offset{ 0.f, m_fUVScrollY };
				m_pShaderCom->Bind_RawValue("g_UVOffset", &Offset, sizeof(_float2));
			}

			if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
				return E_FAIL;

			if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS, 0)))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Begin(2)))
				return E_FAIL;

			if (FAILED(m_pModelCom->Render(i)))
				return E_FAIL;

			if (i == 1)
			{
				_float2 zeroOffset = { 0.f, 0.f };
				m_pShaderCom->Bind_RawValue("g_UVOffset", &zeroOffset, sizeof(_float2));
			}
		}
	}

#ifdef _DEBUG
	m_pColliderCom->Render();
#endif
	return S_OK;
}

HRESULT CTrack::Render_Outline()
{
	//파트 오브젝트는 자기 트랜스폼 안써야한다
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMesh; i++)
	{
		if (FAILED(m_pShaderCom->Begin(3)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CTrack::Render_Shadow()
{
	//파트 오브젝트는 자기 트랜스폼 안써야한다
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
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

void CTrack::On_RaycastHit(CGameObject* pOther)
{
	m_pParent->On_RaycastHit(pOther);
}

HRESULT CTrack::Bind_ShaderResources()
{
	//파트 오브젝트는 자기 트랜스폼 안써야한다
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
	//	return E_FAIL;

	_float4 vBaseColor = { 1.f, 1.f, 1.f, 1.f };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vBaseColor", &vBaseColor, sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

void CTrack::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
