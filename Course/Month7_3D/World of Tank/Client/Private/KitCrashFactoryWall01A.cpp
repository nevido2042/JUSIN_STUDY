#include "KitCrashFactoryWall01A.h"
#include "GameInstance.h"

CKitCrashFactoryWall01A::CKitCrashFactoryWall01A(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBuilding{ pDevice, pContext }
{

}

CKitCrashFactoryWall01A::CKitCrashFactoryWall01A(const CKitCrashFactoryWall01A& Prototype)
	: CBuilding(Prototype)
{

}

HRESULT CKitCrashFactoryWall01A::Initialize_Prototype()
{
	m_vLocalPoint[0] = _float3{ 0.25f, 0.f, 6.f };
	m_vLocalPoint[1] = _float3{ -0.25f, 0.f, 6.f };
	m_vLocalPoint[2] = _float3{ 0.25f, 0.f, -6.f };
	m_vLocalPoint[3] = _float3{ -0.25f, 0.f, -6.f };

	return S_OK;
}

HRESULT CKitCrashFactoryWall01A::Initialize(void* pArg)
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


void CKitCrashFactoryWall01A::Priority_Update(_float fTimeDelta)
{
	CBuilding::Priority_Update(fTimeDelta);
	//m_pGameInstance->Add_CollisionGroup(ENUM_CLASS(COLLISION_GROUP::BUILDING), this, TEXT("Com_Collider"));
}

void CKitCrashFactoryWall01A::Update(_float fTimeDelta)
{
	CBuilding::Update(fTimeDelta);

	//m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CKitCrashFactoryWall01A::Late_Update(_float fTimeDelta)
{
	//CBuilding::Late_Update(fTimeDelta);

	if (m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(STATE::POSITION), 20.f))
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CKitCrashFactoryWall01A::Render()
{
	CBuilding::Render();

	return S_OK;
}


HRESULT CKitCrashFactoryWall01A::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_KitCrashFactoryWall"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_OBB::OBB_DESC	OBBDesc{};
	OBBDesc.vExtents = _float3(0.25f, 3.7f, 6.f);
	OBBDesc.vCenter = _float3(0.25f, OBBDesc.vExtents.y * 0.5f, -0.8f);
	OBBDesc.vRotation = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &OBBDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CKitCrashFactoryWall01A::Bind_ShaderResources()
{
	CBuilding::Bind_ShaderResources();

	return S_OK;
}

CKitCrashFactoryWall01A* CKitCrashFactoryWall01A::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CKitCrashFactoryWall01A* pInstance = new CKitCrashFactoryWall01A(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CKitCrashFactoryWall01A");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CKitCrashFactoryWall01A::Clone(void* pArg)
{
	CKitCrashFactoryWall01A* pInstance = new CKitCrashFactoryWall01A(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CKitCrashFactoryWall01A");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKitCrashFactoryWall01A::Free()
{
	__super::Free();

	//Safe_Release(m_pColliderCom);
	//Safe_Release(m_pModelCom);
	//Safe_Release(m_pShaderCom);
}
