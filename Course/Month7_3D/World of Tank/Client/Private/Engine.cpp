#include "Engine.h"

#include "GameInstance.h"
#include "SoundController.h"

CEngine::CEngine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CEngine::CEngine(const CEngine& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CEngine::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEngine::Initialize(void* pArg)
{
	GAMEOBJECT_DESC			Desc{};

	Desc.fRotationPerSec = 0.f;
	Desc.fSpeedPerSec = 0.f;
	lstrcpy(Desc.szName, TEXT("Engine"));

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

void CEngine::Priority_Update(_float fTimeDelta)
{

}

void CEngine::Update(_float fTimeDelta)
{
	if (false == m_bIsEngineOn && m_pGameInstance->Key_Pressing(DIK_W))
		Start_Engine();

	if (m_pGameInstance->Key_Pressing(DIK_W))
		Press_Accelerator(fTimeDelta);
	else
		Set_RPM(m_fRPM -= fTimeDelta);
}

void CEngine::Late_Update(_float fTimeDelta)
{
	m_pSoundCom->SetVolume("engines_650", m_fRPM);
	m_pSoundCom->Set_Pitch("engines_650", m_fRPM);
}

HRESULT CEngine::Render()
{

	return S_OK;
}

void CEngine::Start_Engine()
{
	Set_RPM(30.f);
	m_pSoundCom->Play("engines_650");
	m_pSoundCom->Set_Loop("engines_650");
}

void CEngine::Press_Accelerator(_float fTimeDelta)
{
	Set_RPM(m_fRPM += fTimeDelta);
}

void CEngine::Set_RPM(_float _fValue)
{
	if (m_fRPM > RPM_MAX)
		m_fRPM = RPM_MAX;
	else if (m_fRPM < RPM_MIN)
		m_fRPM = RPM_MIN;
}

HRESULT CEngine::Ready_Components()
{
	/* For.Com_Sound */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController"),
		TEXT("Com_Sound"), reinterpret_cast<CComponent**>(&m_pSoundCom))))
		return E_FAIL;

	return S_OK;
}

CEngine* CEngine::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEngine* pInstance = new CEngine(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEngine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEngine::Clone(void* pArg)
{
	CEngine* pInstance = new CEngine(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEngine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEngine::Free()
{
	__super::Free();
	Safe_Release(m_pSoundCom);
}
