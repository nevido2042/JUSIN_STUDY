#include "CountdownTimer.h"

#include "GameInstance.h"
#include "DamagePanel.h"

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

	//m_bisCounting = true;;
	m_StartTime = chrono::steady_clock::now() + chrono::seconds(2);

	return S_OK;
}

void CCountdownTimer::Priority_Update(_float fTimeDelta)
{

}

void CCountdownTimer::Update(_float fTimeDelta)
{

}

void CCountdownTimer::Late_Update(_float fTimeDelta)
{
	if(m_bActive)
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CCountdownTimer::Render()
{
	// 시간 차이 계산
	chrono::seconds Remaining = chrono::duration_cast<chrono::seconds>(m_StartTime - chrono::steady_clock::now());

	if(Remaining.count() <= 0)
	{
		CDamagePanel* pDamagePanel = static_cast<CDamagePanel*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_DamagePanel")));
		if (pDamagePanel)
			pDamagePanel->Play_Voice_StartBattle();

		m_bActive = false;
	}

	// 음수 방지 (시간이 지났으면 0초로 표시)
	_int iDisplaySeconds = max<_int>(0, static_cast<_int>(Remaining.count()));

	// 숫자를 문자열로 변환
	wstring strText = to_wstring(iDisplaySeconds);

	// 폰트 출력
	m_pGameInstance->Draw_Font(TEXT("Font_WarheliosKO"), strText.c_str(), _float2(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.1f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f), 2.f * UI_RATIO);

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
}
