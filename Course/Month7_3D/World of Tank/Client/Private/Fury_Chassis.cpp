#include "Fury_Chassis.h"

#include "GameInstance.h"

CFury_Chassis::CFury_Chassis(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CChassis{ pDevice, pContext }
{

}

CFury_Chassis::CFury_Chassis(const CFury_Chassis& Prototype)
	: CChassis(Prototype)
{

}

HRESULT CFury_Chassis::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFury_Chassis::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObject()))
		return E_FAIL;

	return S_OK;
}


void CFury_Chassis::Priority_Update(_float fTimeDelta)
{
	CChassis::Priority_Update(fTimeDelta);
}

void CFury_Chassis::Update(_float fTimeDelta)
{
	CChassis::Update(fTimeDelta);

}

void CFury_Chassis::Late_Update(_float fTimeDelta)
{
	CChassis::Late_Update(fTimeDelta);

}

HRESULT CFury_Chassis::Render()
{
	CChassis::Render();
	return S_OK;
}

void CFury_Chassis::On_RaycastHit(CGameObject* pOther)
{
	CChassis::On_RaycastHit(pOther);
}

HRESULT CFury_Chassis::Ready_PartObject()
{
	CGameObject::GAMEOBJECT_DESC Desc{};
	Desc.pParentWorldMatrix = &m_CombinedWorldMatrix;
	Desc.iID = m_iID;
	Desc.pParent = this;

	/* 왼쪽 궤도를 추가한다. */
	lstrcpy(Desc.szName, TEXT("TrackLeft"));
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_FuryTrackLeft"), TEXT("Part_TrackLeft"), &Desc)))
		return E_FAIL;

	/* 오른쪽 궤도를 추가한다. */
	lstrcpy(Desc.szName, TEXT("TrackRight"));
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_FuryTrackRight"), TEXT("Part_TrackRight"), &Desc)))
		return E_FAIL;

	return S_OK;
}

CFury_Chassis* CFury_Chassis::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFury_Chassis* pInstance = new CFury_Chassis(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFury_Chassis");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFury_Chassis::Clone(void* pArg)
{
	CFury_Chassis* pInstance = new CFury_Chassis(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CFury_Chassis");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFury_Chassis::Free()
{
	__super::Free();

}
