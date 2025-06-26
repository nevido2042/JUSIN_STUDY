#include "FPS_Renderer.h"

#include "GameInstance.h"

CFPS_Renderer::CFPS_Renderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CFPS_Renderer::CFPS_Renderer(const CFPS_Renderer& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CFPS_Renderer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFPS_Renderer::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CFPS_Renderer::Priority_Update(_float fTimeDelta)
{

}

void CFPS_Renderer::Update(_float fTimeDelta)
{
	m_fAccTime += fTimeDelta;
	++m_iFPS;
	if(m_fAccTime >= 1.f)
	{
		m_fAccTime = 0.f;
		m_iFPS_Render = m_iFPS;
		m_iFPS = 0;
	}
}

void CFPS_Renderer::Late_Update(_float fTimeDelta)
{
#pragma message ("왜왜왜왜 파티클?에 가려지는가??? RG_UI인데? 근데 다른데는 잘나옴")
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CFPS_Renderer::Render()
{
	// FPS 값 문자열로 변환
	wstring fpsText = L"FPS: " + to_wstring(m_iFPS_Render);

	// 폰트 출력
	//m_pGameInstance->Draw_Font(TEXT("Font_WarheliosKO"), fpsText.c_str(), _float2(0.f, 0.f)/*, XMVectorSet(1.f, 0.f, 0.f, 1.f)*/);
	m_pGameInstance->Draw_Font(TEXT("Font_WarheliosKO"), fpsText.c_str(), _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f), 0.3f * UI_RATIO);

	return S_OK;
}


CFPS_Renderer* CFPS_Renderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFPS_Renderer* pInstance = new CFPS_Renderer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFPS_Renderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFPS_Renderer::Clone(void* pArg)
{
	CFPS_Renderer* pInstance = new CFPS_Renderer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CFPS_Renderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFPS_Renderer::Free()
{
	__super::Free();
}
