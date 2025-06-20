#include "Tiger_Chassis.h"

#include "GameInstance.h"

CTiger_Chassis::CTiger_Chassis(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CChassis{ pDevice, pContext }
{

}

CTiger_Chassis::CTiger_Chassis(const CTiger_Chassis& Prototype)
	: CChassis(Prototype)
{

}

HRESULT CTiger_Chassis::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTiger_Chassis::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObject()))
		return E_FAIL;

	return S_OK;
}


void CTiger_Chassis::Priority_Update(_float fTimeDelta)
{
	CChassis::Priority_Update(fTimeDelta);
}

void CTiger_Chassis::Update(_float fTimeDelta)
{
	CChassis::Update(fTimeDelta);

}

void CTiger_Chassis::Late_Update(_float fTimeDelta)
{
	CChassis::Late_Update(fTimeDelta);

}

HRESULT CTiger_Chassis::Render()
{
	CChassis::Render();
	return S_OK;
}

void CTiger_Chassis::On_RaycastHit(CGameObject* pOther)
{
	CChassis::On_RaycastHit(pOther);
}

HRESULT CTiger_Chassis::Ready_PartObject()
{
	CGameObject::GAMEOBJECT_DESC Desc{};
	Desc.pParentWorldMatrix = &m_CombinedWorldMatrix;
	Desc.iID = m_iID;
	Desc.pParent = this;

	/* 왼쪽 궤도를 추가한다. */
	lstrcpy(Desc.szName, TEXT("TrackLeft"));
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_TigerTrackLeft"), TEXT("Part_TrackLeft"), &Desc)))
		return E_FAIL;

	/* 오른쪽 궤도를 추가한다. */
	lstrcpy(Desc.szName, TEXT("TrackRight"));
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_TigerTrackRight"), TEXT("Part_TrackRight"), &Desc)))
		return E_FAIL;

	return S_OK;
}

CTiger_Chassis* CTiger_Chassis::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTiger_Chassis* pInstance = new CTiger_Chassis(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTiger_Chassis");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTiger_Chassis::Clone(void* pArg)
{
	CTiger_Chassis* pInstance = new CTiger_Chassis(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTiger_Chassis");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTiger_Chassis::Free()
{
	__super::Free();

}
