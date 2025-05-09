#include "Level_MapTool.h"
#include "GameInstance.h"

CLevel_MapTool::CLevel_MapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_MapTool::Initialize()
{
	if (FAILED(Ready_Imgui()))
		return E_FAIL;

	if (FAILED(Ready_Imgui_Example()))
		return E_FAIL;

	//if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
	//	return E_FAIL;

	return S_OK;
}

void CLevel_MapTool::Update(_float fTimeDelta)
{
	int a = 10;
}

HRESULT CLevel_MapTool::Render()
{
    // ImGui �� ������ ����
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // DockSpace �����츦 �����
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    // Ŭ���̾�Ʈ ���� ũ�� ��������
    RECT rect;
    GetClientRect(g_hWnd, &rect);  // Ŭ���̾�Ʈ ���� ũ�� ��������

	ImGui::SetNextWindowPos({ 0.f, 0.f });   // �۾� ������ ��ġ
	ImGui::SetNextWindowSize({ static_cast<_float>(g_iWinSizeX), static_cast<_float>(g_iWinSizeY) });  // �׻� �ڵ� ũ�� ����
    ImGui::SetNextWindowViewport(viewport->ID);    // ����Ʈ ����

    // ��Ÿ�� ���� (������ �׵θ�, �𼭸� �� ����)
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_AlwaysAutoResize;

    // DockSpace â ����
    ImGui::Begin("DockSpace Demo", nullptr, window_flags);
    ImGui::PopStyleVar(2); // ��Ÿ�� ���� ����

    // DockSpace ID ��������
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
    ImGui::End();  // DockSpace â ����

    // UI â�� ����
    ImGui::Begin("Example Window", nullptr, ImGuiWindowFlags_AlwaysAutoResize);  // â ũ�� �ڵ� ����
    ImGui::Text("Hello, World!");  // �ؽ�Ʈ ǥ��
    if (ImGui::Button("Click Me"))  // ��ư Ŭ�� ��
    {
        ImGui::Text("Button clicked!");  // ��ư Ŭ�� �� �ؽ�Ʈ ����
    }
    ImGui::End();  // UI â ����

    // ������ ó��
    ImGui::Render();  // ������ ó��

    // ImGui �׸���
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());  // ImGui �׸���

    return S_OK;
}

HRESULT CLevel_MapTool::Ready_Layer_BackGround(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Terrain"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_MapTool::Ready_Layer_Camera(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::HANGER), TEXT("Prototype_GameObject_Camera_Free"),
		ENUM_CLASS(LEVEL::HANGER), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_MapTool::Ready_Imgui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// ��Ÿ�� ���� (����)
	ImGui::StyleColorsDark();

	// Win32 + DX11 �鿣�� �ʱ�ȭ
	ImGui_ImplWin32_Init(g_hWnd); // �� ������ ���� ������ �ڵ� ���
	ImGui_ImplDX11_Init(m_pDevice, m_pContext); // DX11 �ʱ�ȭ �� ���� device/context

	return S_OK;
}

HRESULT CLevel_MapTool::Ready_Imgui_Example()
{
	return S_OK;
}

CLevel_MapTool* CLevel_MapTool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_MapTool* pInstance = new CLevel_MapTool(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_MapTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_MapTool::Free()
{
	__super::Free();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

}
