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
    // ImGui 새 프레임 시작
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // DockSpace 윈도우를 만든다
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    // 클라이언트 영역 크기 가져오기
    RECT rect;
    GetClientRect(g_hWnd, &rect);  // 클라이언트 영역 크기 가져오기

	ImGui::SetNextWindowPos({ 0.f, 0.f });   // 작업 영역의 위치
	ImGui::SetNextWindowSize({ static_cast<_float>(g_iWinSizeX), static_cast<_float>(g_iWinSizeY) });  // 항상 자동 크기 조정
    ImGui::SetNextWindowViewport(viewport->ID);    // 뷰포트 설정

    // 스타일 설정 (윈도우 테두리, 모서리 등 설정)
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_AlwaysAutoResize;

    // DockSpace 창 시작
    ImGui::Begin("DockSpace Demo", nullptr, window_flags);
    ImGui::PopStyleVar(2); // 스타일 설정 해제

    // DockSpace ID 가져오기
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
    ImGui::End();  // DockSpace 창 종료

    // UI 창을 띄운다
    ImGui::Begin("Example Window", nullptr, ImGuiWindowFlags_AlwaysAutoResize);  // 창 크기 자동 조정
    ImGui::Text("Hello, World!");  // 텍스트 표시
    if (ImGui::Button("Click Me"))  // 버튼 클릭 시
    {
        ImGui::Text("Button clicked!");  // 버튼 클릭 시 텍스트 변경
    }
    ImGui::End();  // UI 창 종료

    // 렌더링 처리
    ImGui::Render();  // 렌더링 처리

    // ImGui 그리기
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());  // ImGui 그리기

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

	// 스타일 설정 (선택)
	ImGui::StyleColorsDark();

	// Win32 + DX11 백엔드 초기화
	ImGui_ImplWin32_Init(g_hWnd); // ← 기존에 만든 윈도우 핸들 사용
	ImGui_ImplDX11_Init(m_pDevice, m_pContext); // DX11 초기화 때 만든 device/context

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
