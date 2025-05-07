#include "MainApp.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "DebugUtils.h"
#include "BackGround_Loading.h"
#include "Loading_Spinner.h"

CMainApp::CMainApp()
	: m_pGameInstance { CGameInstance::Get_Instance() },
	m_pNetwork{ CNetwork::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pNetwork);

}

HRESULT CMainApp::Initialize()
{
#if defined(_DEBUG)
	DebugUtils::ShowConsole();
#endif

	ENGINE_DESC			EngineDesc{};

	EngineDesc.hInst = g_hInst;
	EngineDesc.hWnd = g_hWnd;
	EngineDesc.iWinSizeX = g_iWinSizeX;
	EngineDesc.iWinSizeY = g_iWinSizeY;
	EngineDesc.isWindowed = true;
	EngineDesc.iNumLevels = static_cast<_uint>(LEVEL::END);

	if (FAILED(m_pGameInstance->Initialize_Engine(EngineDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_For_Loading()))
		return E_FAIL;
	
	if (FAILED(Start_Level(LEVEL::LOGO)))
		return E_FAIL;

	m_pNetwork->Initialize();

    return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
	m_pNetwork->Priority_Update(fTimeDelta);

	m_pGameInstance->Update_Engine(fTimeDelta);
	m_pNetwork->Update(fTimeDelta);

	m_pNetwork->Late_Update(fTimeDelta);
}

HRESULT CMainApp::Render()
{
	m_pGameInstance->Begin_Draw();

	m_pGameInstance->Draw();

	m_pGameInstance->End_Draw();

    return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{

	/* For.Prototype_Component_VIBuffer_Rect*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_For_Loading()
{

#pragma region 텍스쳐
	/* For.Prototype_Component_Texture_Background_Loading*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Background_Loading"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Loading/Background/99_poland.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Loading_Spinner*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Loading_Spinner"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/WOT_Resources/UI/Loading/Spinner/0.dds"), 1))))
		return E_FAIL;
	
#pragma endregion


#pragma region 원형 객체
	/* For.Prototype_GameObject_Background_Loading */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Background_Loading"),
		CBackGround_Loading::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Loading_Spinner */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Loading_Spinner"),
		CLoading_Spinner::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion



	return S_OK;
}

HRESULT CMainApp::Start_Level(LEVEL eStartLevel)
{
	if (FAILED(m_pGameInstance->Change_Level(ENUM_CLASS(LEVEL::LOADING), CLevel_Loading::Create(m_pDevice, m_pContext, eStartLevel))))
		return E_FAIL;

	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
    __super::Free();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	Safe_Release(m_pNetwork);

	m_pGameInstance->Release_Engine();

	Safe_Release(m_pGameInstance);

#if defined(_DEBUG)
	DebugUtils::CloseConsole();
#endif
}
