#include "CountdownTimer.h"

#include "GameInstance.h"
#include "DamagePanel.h"
#include "GameManager.h"

CCountdownTimer::CCountdownTimer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CCountdownTimer::CCountdownTimer(const CCountdownTimer& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CCountdownTimer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCountdownTimer::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	
	m_pSoundCom->SetVolume(0.5f);

	//m_bisCounting = true;;
	CGameManager* pGameManager = GET_GAMEMANAGER;
	if (pGameManager == nullptr)
		return E_FAIL;

	m_StartTime = pGameManager->Get_GameStartTime();

	// �ð� ���� ���
	m_Remaining = chrono::duration_cast<chrono::seconds>(m_StartTime - chrono::steady_clock::now());

	// �ð� ���� ���
	m_PrevRemaining = chrono::duration_cast<chrono::seconds>(m_StartTime - chrono::steady_clock::now());


	return S_OK;
}

void CCountdownTimer::Priority_Update(_float fTimeDelta)
{

}

void CCountdownTimer::Update(_float fTimeDelta)
{
	// �ð� ���� ���
	m_Remaining = chrono::duration_cast<chrono::seconds>(m_StartTime - chrono::steady_clock::now());

	if (m_Remaining.count() <= 0)
	{
		CDamagePanel* pDamagePanel = static_cast<CDamagePanel*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_DamagePanel")));
		if (pDamagePanel)
			pDamagePanel->Play_Voice_StartBattle();

		m_bActive = false;
	}

	//�ð� ������ 1���̻����̳��� �Ҹ� ���
	if (m_PrevRemaining.count() - m_Remaining.count() >= 1)
	{
		m_PrevRemaining = m_Remaining;
		m_pSoundCom->Play("ClockTick");
	}

}

void CCountdownTimer::Late_Update(_float fTimeDelta)
{
	if(m_bActive)
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CCountdownTimer::Render()
{
	// ���� ���� (�ð��� �������� 0�ʷ� ǥ��)
	_int iDisplaySeconds = max<_int>(0, static_cast<_int>(m_Remaining.count()));

	// ���ڸ� ���ڿ��� ��ȯ
	wstring strText = to_wstring(iDisplaySeconds);

	// ��Ʈ ���
	m_pGameInstance->Draw_Font(TEXT("Font_WarheliosKO"), strText.c_str(), _float2(g_iWinSizeX * 0.465f, g_iWinSizeY * 0.1f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f), 2.f * UI_RATIO);

	wstring strMission = L"�ӹ�: ���� �ð� ���� ��� �� ������ �����Ͻʽÿ�.";
	m_pGameInstance->Draw_Font(TEXT("Font_WarheliosKO"), strMission.c_str(), _float2(g_iWinSizeX * 0.355f, g_iWinSizeY * 0.25f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f), 0.3f * UI_RATIO);

	return S_OK;
}



HRESULT CCountdownTimer::Ready_Components()
{
	/* For.Com_Sound */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController_Clock"),
		TEXT("Com_Sound"), reinterpret_cast<CComponent**>(&m_pSoundCom))))
		return E_FAIL;

	return S_OK;
}


CCountdownTimer* CCountdownTimer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCountdownTimer* pInstance = new CCountdownTimer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCountdownTimer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCountdownTimer::Clone(void* pArg)
{
	CCountdownTimer* pInstance = new CCountdownTimer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCountdownTimer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCountdownTimer::Free()
{
	__super::Free();

	Safe_Release(m_pSoundCom);
}
