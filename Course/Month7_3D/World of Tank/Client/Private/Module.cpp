#include "Module.h"

#include "GameInstance.h"
#include "Tank.h"
#include "Shell.h"
#include "DamageIndicator.h"

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

void CModule::On_RaycastHit(CGameObject* pOther)
{
	// �����÷��� �϶��� �ڽ��� �����浹�� �޽����κ��͸� �޴´�.
	if (m_pGameInstance->Get_NewLevel_Index() != ENUM_CLASS(LEVEL::GAMEPLAY) || m_pGameInstance->Get_ID() != m_iID)
	{
		TakeDamage(30.f);
	}

	// �����÷��̸鼭 �÷��̾��� ��ũ�� �ƴѾֵ��� �浹 �޽����� �����Ѵ�.
	if (m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::GAMEPLAY) && m_pGameInstance->Get_ID() != m_iID)
	{
		HIT_MODULE_DESC Desc = {};
		Desc.iID = m_pGameInstance->Get_ID();
		Desc.iTargetID = m_iID;
		Desc.eModule = m_eModuleType;
		Desc.vFirePos = static_cast<CShell*>(pOther)->Get_FirePos();

		m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_HIT_MODULE), &Desc);
	}

	if (m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::PRACTICE) && m_iID == m_pGameInstance->Get_ID())
	{
		//������ �ε������� ����� ���⼭
		CDamageIndicator::DAMAGE_INDICATOR_DESC		Desc{};

		Desc.fSizeX = 248.f * UI_RATIO;
		Desc.fSizeY = 716.f * UI_RATIO;
		Desc.fX = g_iWinSizeX * 0.5f;
		Desc.fY = g_iWinSizeY * 0.5f;
		Desc.fDepth = DEPTH_BACKGROUND - 0.01f;
		Desc.fRotationPerSec = 1.f;
		Desc.vFirePos = static_cast<CShell*>(pOther)->Get_FirePos();

		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DamageIndicator"),
			ENUM_CLASS(LEVEL::PRACTICE), TEXT("Layer_DamageIndicator"), &Desc)))
			return;
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
