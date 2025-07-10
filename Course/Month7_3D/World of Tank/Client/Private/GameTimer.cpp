#include "GameTimer.h"

#include "GameInstance.h"
#include "DamagePanel.h"

CGameTimer::CGameTimer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CGameTimer::CGameTimer(const CGameTimer& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CGameTimer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameTimer::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	//m_bisCounting = true;;
	m_StartTime = chrono::steady_clock::now() + chrono::seconds(2);

	return S_OK;
}

void CGameTimer::Priority_Update(_float fTimeDelta)
{

}

void CGameTimer::Update(_float fTimeDelta)
{

}

void CGameTimer::Late_Update(_float fTimeDelta)
{
	if (m_bActive)
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CGameTimer::Render()
{
	//// 시간 차이 계산
	//chrono::seconds Remaining = chrono::duration_cast<chrono::seconds>(m_StartTime - chrono::steady_clock::now());

	//if (Remaining.count() <= 0)
	//{
	//	CDamagePanel* pDamagePanel = static_cast<CDamagePanel*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_DamagePanel")));
	//	if (pDamagePanel)
	//		pDamagePanel->Play_Voice_StartBattle();

	//	m_bActive = false;
	//}

	//// 음수 방지 (시간이 지났으면 0초로 표시)
	//_int iDisplaySeconds = max<_int>(0, static_cast<_int>(Remaining.count()));

	//// 숫자를 문자열로 변환
	//wstring strText = to_wstring(iDisplaySeconds);

	// 폰트 출력
	m_pGameInstance->Draw_Font(TEXT("Font_WarheliosKO"), TEXT("15:00"), _float2(g_iWinSizeX * 0.95f, 0), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f), 0.3f * UI_RATIO);

	return S_OK;
}


CGameTimer* CGameTimer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGameTimer* pInstance = new CGameTimer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGameTimer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGameTimer::Clone(void* pArg)
{
	CGameTimer* pInstance = new CGameTimer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGameTimer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameTimer::Free()
{
	__super::Free();
}
