#include "AmmoBay.h"

#include "GameInstance.h"
#include "SoundController.h"

CAmmoBay::CAmmoBay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CModule{ pDevice, pContext }
{

}

CAmmoBay::CAmmoBay(const CAmmoBay& Prototype)
	: CModule(Prototype)
{

}

HRESULT CAmmoBay::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAmmoBay::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pSoundCom->Set3DState(0.f, 30.f);

	return S_OK;
}

void CAmmoBay::Priority_Update(_float fTimeDelta)
{


}

void CAmmoBay::Update(_float fTimeDelta)
{
	//부모의 월드 행렬을 가져와서 자신의 월드 행렬과 곱해준다.
	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixMultiply(m_pTransformCom->Get_WorldMatrix(), XMLoadFloat4x4(m_pParentWorldMatrix)));

	_vector vPos = XMVectorSet(m_CombinedWorldMatrix._41, m_CombinedWorldMatrix._42, m_CombinedWorldMatrix._43, 1.0f);
	m_pSoundCom->Update3DPosition(vPos);

	if (m_bIsLoading)
	{
		// 시간 차이 계산
		m_Remaining = chrono::duration_cast<chrono::duration<_float>>(m_TimeLoadComplete - chrono::high_resolution_clock::now());

		// 음수 방지 (시간이 지났으면 0초로 표시)
		m_fDisplaySeconds = max<_float>(0.f, static_cast<_float>(m_Remaining.count()));

		if (m_fDisplaySeconds < 1.f && m_bIsPlayLoadSound == false)
		{
			m_bIsPlayLoadSound = true;
			m_pSoundCom->Play("gun_reload_1");
		}

		if (m_fDisplaySeconds == 0.f)
		{
			m_bIsLoading = false;
		}
	}

}

void CAmmoBay::Late_Update(_float fTimeDelta)
{
	if(m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::HANGER))
		return;	

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CAmmoBay::Render()
{
	if (m_pGameInstance->Get_ID() == m_iID)
	{
		if (m_bIsLoading)
		{
			wstring strText = format(L"{:.2f}", m_fDisplaySeconds);
			m_pGameInstance->Draw_Font(TEXT("Font_WarheliosKO"), strText.c_str(), _float2(g_iWinSizeX * 0.45f, g_iWinSizeY * 0.5f), XMVectorSet(1.f, 0.f, 0.f, 1.f), 0.f, _float2(0.f, 0.f), 0.3f * UI_RATIO);
		}
		else
		{
			wstring strText = {};
			if (m_eModuleState == MODULE_STATE::FUNCTIONAL)
				strText = format(L"{:.2f}", m_fLoadTime);
			else if (m_eModuleState == MODULE_STATE::DAMAGED)
				strText = format(L"{:.2f}", m_fLoadTime * 2.f);

			m_pGameInstance->Draw_Font(TEXT("Font_WarheliosKO"), strText.c_str(), _float2(g_iWinSizeX * 0.45f, g_iWinSizeY * 0.5f), XMVectorSet(0.f, 1.f, 0.f, 1.f), 0.f, _float2(0.f, 0.f), 0.3f * UI_RATIO);
		}
	}

	return S_OK;
}

void CAmmoBay::Start_Load()
{
	if (m_eModuleState == MODULE_STATE::FUNCTIONAL)
		m_TimeLoadComplete = chrono::steady_clock::now() + chrono::duration<_float>(m_fLoadTime);
	else if (m_eModuleState == MODULE_STATE::DAMAGED)
		m_TimeLoadComplete = chrono::steady_clock::now() + chrono::duration<_float>(m_fLoadTime * 2.f);

	 m_bIsLoading = true;
	 m_bIsPlayLoadSound = false;
}

HRESULT CAmmoBay::is_Load_Complete()
{
	if (m_bIsLoading)
		return E_FAIL;

	return S_OK;
}

HRESULT CAmmoBay::Ready_Components()
{
	/* For.Com_Sound */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController_TankSound3D"),
		TEXT("Com_Sound"), reinterpret_cast<CComponent**>(&m_pSoundCom))))
		return E_FAIL;

	return S_OK;
}

CAmmoBay* CAmmoBay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAmmoBay* pInstance = new CAmmoBay(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CAmmoBay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAmmoBay::Clone(void* pArg)
{
	CAmmoBay* pInstance = new CAmmoBay(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CAmmoBay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAmmoBay::Free()
{
	__super::Free();
	Safe_Release(m_pSoundCom);
}
