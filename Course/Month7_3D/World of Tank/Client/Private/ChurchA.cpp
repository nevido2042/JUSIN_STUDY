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
	LANDOBJECT_DESC			Desc{};
	Desc.iLevelIndex = m_pGameInstance->Get_NewLevel_Index();
	Desc.strLayerTag = TEXT("Layer_Terrain");
	Desc.strComponentTag = TEXT("Com_VIBuffer");
	Desc.iIndex = 0;
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
}

void CChurchA::Update(_float fTimeDelta)
{
	CBuilding::Update(fTimeDelta);

}

void CChurchA::Late_Update(_float fTimeDelta)
{

	if (m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(STATE::POSITION), 50.f))
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CChurchA::Render()
{
	CBuilding::Render();

	return S_OK;
}

void CChurchA::OnGround(_float fTimeDelta)
{
	// 현재 위치에서 지형 높이 계산
	_vector vPosition = m_pTransformCom->Get_State(STATE::POSITION);
	_vector vLook = m_pTransformCom->Get_State(STATE::LOOK);
	vLook = XMVector3Normalize(vLook);
	vPosition = m_pTargetBuffer->Compute_HeightPosition(vPosition - vLook * 11.f);
	_float fHeight = XMVectorGetY(vPosition);

	// 최종 위치 설정
	vPosition = m_pTransformCom->Get_State(STATE::POSITION);
	vPosition = XMVectorSetY(vPosition, fHeight);

	m_pTransformCom->Set_State(STATE::POSITION, vPosition);

	_vector vLocalPoint[4] =
	{
		XMVectorSet(8.f, 0.f, 0.f, 1.f),
		XMVectorSet(-8.f, 0.f, 0.f, 1.f),
		XMVectorSet(8.f, 0.f, -28.f, 1.f),
		XMVectorSet(-8.f, 0.f, -28.f, 1.f),
	};

	_vector vNormalSum = XMVectorZero();
	for (_int i = 0; i < 4; ++i)
	{
		vLocalPoint[i] = XMVector3TransformCoord(vLocalPoint[i], m_pTransformCom->Get_WorldMatrix());
		vNormalSum += m_pTargetBuffer->Compute_NormalPosition(vLocalPoint[i]);
	}
	_vector vTargetUp = XMVector3Normalize(vNormalSum / 4.f);

	// 이전 Up 벡터 가져오기
	_vector vCurrentUp = m_pTransformCom->Get_State(STATE::UP);

	// 선형 보간으로 부드럽게 변화
	const _float fLerpSpeed = 5.f; // 클수록 빠르게 보간됨
	vTargetUp = XMVector3Normalize(XMVectorLerp(vCurrentUp, vTargetUp, fLerpSpeed * fTimeDelta));

	// Look 벡터는 이전 유지
	vLook = XMVector3Normalize(m_pTransformCom->Get_State(STATE::LOOK));

	// Up과 Look으로부터 Right 재계산
	_vector vRight = XMVector3Normalize(XMVector3Cross(vTargetUp, vLook));

	// Right와 Up으로 Look 재계산
	vLook = XMVector3Normalize(XMVector3Cross(vRight, vTargetUp));

	// 방향 벡터 적용
	m_pTransformCom->Set_State(STATE::RIGHT, vRight);
	m_pTransformCom->Set_State(STATE::UP, vTargetUp);
	m_pTransformCom->Set_State(STATE::LOOK, vLook);
		


	//_vector vOffsets[4] = {
	//XMVectorSet(8.f, 0.f, 0.f, 0.f),
	//XMVectorSet(-8.f, 0.f, 0.f, 0.f),
	//XMVectorSet(8.f, 0.f, -28.f, 0.f),
	//XMVectorSet(-8.f, 0.f, -28.f, 0.f),
	//};

	//////x,z임
	//_float2 vCenter = { 0.f, -11.f };


	//// 4개 위치 노말 평균
	//_vector vNormalSum = XMVectorZero();
	//for (_int i = 0; i < 4; ++i)
	//{
	//	_vector vSamplePos = vPosition + vOffsets[i];
	//	//vSamplePos = m_pTargetBuffer->Compute_HeightPosition(vSamplePos);
	//	vNormalSum += m_pTargetBuffer->Compute_NormalPosition(vSamplePos);
	//}
	
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
