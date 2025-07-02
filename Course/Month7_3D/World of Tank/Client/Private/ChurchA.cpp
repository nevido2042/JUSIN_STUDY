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
	m_vLocalPoint[0] = _float3{ 8.f, 0.f, 0.f };
	m_vLocalPoint[1] = _float3{ -8.f, 0.f, 0.f };
	m_vLocalPoint[2] = _float3{ 8.f, 0.f, -28.f };
	m_vLocalPoint[3] = _float3{ -8.f, 0.f, -28.f };

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

#ifdef _DEBUG
	//m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif
}

HRESULT CChurchA::Render()
{
	CBuilding::Render();

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
}
