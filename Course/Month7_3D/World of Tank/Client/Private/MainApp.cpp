#include "MainApp.h"

#include "GameInstance.h"
#include "DebugUtils.h"

#include "Level_Loading.h"

#include "BackGround_Loading.h"
#include "Loading_Spinner.h"

CMainApp::CMainApp()
	: m_pGameInstance { CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);

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

	Ready_Prototype_Component();
	Ready_Prototype_For_Loading();

	if (FAILED(Start_Level(LEVEL::LOGO)))
		return E_FAIL;

	if(FAILED(Define_Packets()))
		return E_FAIL;

    return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_Network_Status() != NETWORK_STATUS::DISCONNECTED)
	{
		m_fPingElapsed += fTimeDelta;
		if (m_fPingElapsed > PING_TIME && m_bSendPing == false)
		{
			m_fPingElapsed = 0.f;
			m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_PING), nullptr);
			m_bSendPing = true;
		}
	}


	extern _uint g_iWinResizeX;
	extern _uint g_iWinResizeY;
	m_pGameInstance->Update_Engine(fTimeDelta, g_iWinResizeX, g_iWinResizeY);
}

HRESULT CMainApp::Render()
{
	m_pGameInstance->Begin_Draw();

	m_pGameInstance->Draw();

	m_pGameInstance->End_Draw();

    return S_OK;
}

HRESULT CMainApp::Define_Packets()
{
	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::SC_GIVE_ID), [this](void* pArg)
		{
			PACKET_DESC Desc{};
			m_pGameInstance->Output_Data(reinterpret_cast<_byte*>(&Desc), sizeof(PACKET_DESC));
			m_pGameInstance->Set_ID(Desc.iID);

			m_pGameInstance->Set_Network_Status(NETWORK_STATUS::CONNECTED);
		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::CS_PING), [this](void* pArg)
		{
			if(m_pGameInstance->Get_Network_Status() == NETWORK_STATUS::DISCONNECTED)
				return;

			PACKET_DESC Desc{};
			Desc.iID = m_pGameInstance->Get_ID();

			m_pGameInstance->Clear_Packet();

			PACKET_HEADER tHeader{};
			tHeader.byCode = PACKET_CODE;
			tHeader.byType = ENUM_CLASS(PacketType::CS_PING);

			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));
			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(&Desc), sizeof(PACKET_DESC));

			m_pGameInstance->Update_Header();
		})))
		return E_FAIL;

	if(FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::SC_PING), [this](void* pArg) 
		{
			m_pGameInstance->Set_Network_Status(NETWORK_STATUS::CONNECTED);
			::cout << "SC_PING" << endl;
			m_bSendPing = false;
		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::CS_POSITION), [this](void* pArg)
		{
			if (m_pGameInstance->Get_Network_Status() == NETWORK_STATUS::DISCONNECTED)
				return;

			m_pGameInstance->Clear_Packet();

			PACKET_HEADER tHeader{};
			tHeader.byCode = PACKET_CODE;
			tHeader.byType = ENUM_CLASS(PacketType::CS_POSITION);

			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(&tHeader), sizeof(PACKET_HEADER));

			POSITION_DESC* Position_Desc = static_cast<POSITION_DESC*>(pArg);
			Position_Desc->iID = m_pGameInstance->Get_ID();

			m_pGameInstance->Input_Data(reinterpret_cast<_byte*>(Position_Desc), sizeof(POSITION_DESC));
			m_pGameInstance->Update_Header();
		})))
		return E_FAIL;

	if(FAILED(m_pGameInstance->Define_Packet(ENUM_CLASS(PacketType::SC_POSITION), [this](void* pArg)
		{
			POSITION_DESC pDesc{};
			m_pGameInstance->Output_Data(reinterpret_cast<_byte*>(&pDesc), sizeof(POSITION_DESC));

			::cout << pDesc.vPos.x << ' ' << pDesc.vPos.y << ' ' << pDesc.vPos.z << endl;
		})))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{
#pragma region 컴포넌트
	/* For.Prototype_Component_VIBuffer_Rect*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxNorTex */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_SoundController */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController"),
		CSoundController::Create("../Bin/WOT_Resources/Sound/01.Engine/"))))															\
		return E_FAIL;
#pragma endregion

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
	m_pGameInstance->Release_Engine();

	//Safe_Release(m_pGameInstance);

#if defined(_DEBUG)
	DebugUtils::CloseConsole();
#endif
}
