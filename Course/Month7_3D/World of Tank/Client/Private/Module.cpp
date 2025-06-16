#include "Module.h"

#include "GameInstance.h"
#include "Tank.h"
#include "Shell.h"

CModule::CModule(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CModule::CModule(const CModule& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CModule::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CModule::Initialize(void* pArg)
{
	MODULE_DESC* pModuleDesc = reinterpret_cast<MODULE_DESC*>(pArg);
	m_pOwner = pModuleDesc->pOwner;
	m_eModuleType = pModuleDesc->eModuleType;
	if (nullptr == m_pOwner)
		return E_FAIL;
	//Safe_AddRef(m_pOwner);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}



void CModule::Priority_Update(_float fTimeDelta)
{


}

void CModule::Update(_float fTimeDelta)
{

}

void CModule::Late_Update(_float fTimeDelta)
{

}

HRESULT CModule::Render()
{

	return S_OK;
}

void CModule::On_Collision_Stay(CGameObject* pOther, _fvector vNormal)
{
	// 게임플레이 일때는 자신의 엔진충돌은 메시지로부터만 받는다.
	if (m_pGameInstance->Get_NewLevel_Index() != ENUM_CLASS(LEVEL::GAMEPLAY) || m_pGameInstance->Get_ID() != m_iID)
	{
		TakeDamage(30.f);
	}

	// 게임플레이면서 플레이어의 탱크가 아닌애들은 충돌 메시지를 전달한다.
	if (m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::GAMEPLAY) && m_pGameInstance->Get_ID() != m_iID)
	{
		HIT_MODULE_DESC Desc = {};
		Desc.iID = m_pGameInstance->Get_ID();
		Desc.iTargetID = m_iID;
		Desc.eModule = m_eModuleType;
		Desc.vFirePos = static_cast<CShell*>(pOther)->Get_FirePos();

		m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_HIT_MODULE), &Desc);
	}
}

void CModule::Set_ModuleState(MODULE_STATE eState)
{
	m_eModuleState = eState;
}

void CModule::TakeDamage(_float fDamage)
{
	m_pOwner->Take_Damage(fDamage);

	Set_ModuleState(static_cast<MODULE_STATE>(max(0, _int(ENUM_CLASS(m_eModuleState) - 1))));
}

HRESULT CModule::Ready_Components()
{

	return S_OK;
}





void CModule::Free()
{
	__super::Free();

	//Safe_Release(m_pOwner);

}
