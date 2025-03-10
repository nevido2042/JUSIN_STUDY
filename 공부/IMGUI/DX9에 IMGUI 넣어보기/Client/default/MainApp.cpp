#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <tchar.h>

#include "MainApp.h"
#include "GameInstance.h"

#include "Level_Logo.h"
#include "Level_Loading.h"

CMainApp::CMainApp()
	: m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	ENGINE_DESC		Desc{};
	Desc.hWnd = g_hWnd;
	Desc.isWindowed = true;
	Desc.iWinSizeX = g_iWinSizeX;
	Desc.iWinSizeY = g_iWinSizeY;
	Desc.iNumLevels = LEVEL_END;

	if (FAILED(m_pGameInstance->Initialize_Engine(Desc, &m_pGraphic_Device)))
		return E_FAIL;

	if (FAILED(Ready_Component_For_Static()))
		return E_FAIL;

	if (FAILED(Ready_Default_Setting()))
		return E_FAIL;

	/* ���� ������ ������ �Ҵ�����. */
	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;
	

	return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
	m_pGameInstance->Update_Engine(fTimeDelta);
}

HRESULT CMainApp::Render()
{
	m_pGameInstance->Draw();

	// ���� ������ -> 145�ٷ� �ٷ� ���� ��
	int iWindowSizeX = GetSystemMetrics(SM_CXSCREEN);
	int iWindowSizeY = GetSystemMetrics(SM_CYSCREEN);
	bool opened = true;
	bool checkBox = false;

	int iSlider = 10;
	float fSlider = 10.f;

	char buf[128] = {};

	float fColor[3] = { 0.0f, 0.0f, 0.0f };

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (ImGui::Begin("ImGui Tutorial", &opened)) 
	{
		//���ڰ� ��� ���۵� ���� x ��ǥ
		//ImGui::SetCursorPosX(40);
		//���� ���
		//ImGui::Text("Hello to my test"); 

		//���ڸ� ��� ��½�Ű��
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - ImGui::CalcTextSize("Hello to my test").x / 2);
		ImGui::Text("Hello to my test");

		//��ư ��� - �̸��� ũ��
		if (ImGui::Button("BUTTON", ImVec2(100, 30))) {
		}

		// üũ �ڽ� ���
		ImGui::Checkbox("My Checkbox", &checkBox);

		//�����̴� - �̸�, ������ int, �ּ�, �ִ�
		ImGui::SliderInt("Int Value", &iSlider, 0, 100);
		ImGui::SliderFloat("Float Value", &fSlider, -50.f, 150.f);

		// ����, ��� ������ ������ ��, �󸶾� ������ų ��, ������Ű�� �ӵ�
		ImGui::InputInt("Input Int", &iSlider, 1, 100);

		// �ؽ�Ʈ �Է�
		// �Ű������� ImGuiInputTextFlags_Password �ϸ� ���� ���̰� �� �� �� ����
		ImGui::InputText("PASSWORD", buf, IM_ARRAYSIZE(buf));

		// �� ���ϴ� �� ���� ImGuiColorEditFlags_�� ������ �̰͵� ������ �پ��ѵ� (fColor�ڿ� �Ű����� �߰�)
		ImGui::ColorPicker3("Pick Color", fColor);

	}ImGui::End();

	// Rendering
	ImGui::EndFrame();
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
	//D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f), (int)(clear_color.y * clear_color.w * 255.0f), (int)(clear_color.z * clear_color.w * 255.0f), (int)(clear_color.w * 255.0f));
	//m_pGraphic_Device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	if (m_pGraphic_Device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		m_pGraphic_Device->EndScene();
	}
	HRESULT result = m_pGraphic_Device->Present(nullptr, nullptr, nullptr, nullptr);
	if (result == D3DERR_DEVICELOST)
		//g_DeviceLost = true;

		// ----------- ��Ÿ�� ���� ------------
		ImGuiStyle& style = ImGui::GetStyle();

	return S_OK;
}

HRESULT CMainApp::Ready_Default_Setting()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls


	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX9_Init(m_pGraphic_Device);

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;

	return S_OK;
}

HRESULT CMainApp::Ready_Component_For_Static()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVEL eLevelID)
{
	if (FAILED(m_pGameInstance->Change_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, eLevelID))))
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

	// Cleanup
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	Safe_Release(m_pGraphic_Device);

	m_pGameInstance->Release_Engine();

	/* ������� �����Ѵ�.*/	
	Safe_Release(m_pGameInstance);

}
