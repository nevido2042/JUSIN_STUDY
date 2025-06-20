#include "Chassis.h"

#include "GameInstance.h"
#include "DamagePanel.h"
#include "Icon_Module.h"
#include "Tank.h"

CChassis::CChassis(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CModule{ pDevice, pContext }
{

}

CChassis::CChassis(const CChassis& Prototype)
	: CModule(Prototype)
{

}

HRESULT CChassis::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CChassis::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}


void CChassis::Priority_Update(_float fTimeDelta)
{
	//m_pGameInstance->Add_CollisionGroup(ENUM_CLASS(COLLISION_GROUP::MODULE), this, TEXT("Com_Collider"));

	CGameObject::Priority_Update(fTimeDelta);

}

void CChassis::Update(_float fTimeDelta)
{
	//부모의 월드 행렬을 가져와서 자신의 월드 행렬과 곱해준다.
	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixMultiply(m_pTransformCom->Get_WorldMatrix(), XMLoadFloat4x4(m_pParentWorldMatrix)));

	CGameObject::Update(fTimeDelta);

}

void CChassis::Late_Update(_float fTimeDelta)
{

	//m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);

	CGameObject::Late_Update(fTimeDelta);

}

HRESULT CChassis::Render()
{
	return S_OK;
}

void CChassis::On_RaycastHit(CGameObject* pOther)
{
	CModule::On_RaycastHit(pOther);
}

void CChassis::Set_ModuleState(MODULE_STATE eState)
{
	m_eModuleState = eState;

	if (m_pGameInstance->Get_ID() == m_iID)
	{
		CDamagePanel* pDamagePanel = static_cast<CDamagePanel*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_DamagePanel")));
		if (pDamagePanel == nullptr)
			return;

		if (m_pOwner)
		{
			if (!m_pOwner->Get_isTankDestroyed())
				pDamagePanel->Play_Voice_ModuleState(m_eModuleType, m_eModuleState);
		}

		CIcon_Module* pIcon = static_cast<CIcon_Module*>(pDamagePanel->Find_PartObject(TEXT("Part_Track")));
		if (pIcon == nullptr)
			return;

		pIcon->Set_ModuleState(m_eModuleState);
	}
}

HRESULT CChassis::Bind_ShaderResources()
{
	return S_OK;
}

void CChassis::Free()
{
	__super::Free();

	for (auto& Pair : m_PartObjects)
		Safe_Release(Pair.second);
	m_PartObjects.clear();
}
