#include "Tool_Base.h"
#include "GameInstance.h"

CTool_Base::CTool_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTool(pDevice, pContext)
{
}

CTool_Base::CTool_Base(const CTool_Base& Prototype)
	: CTool(Prototype)
{
}

HRESULT CTool_Base::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTool_Base::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();

	// 윈도우 배경만 투명하게
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.3f); // Window 배경 투명


	// Win32 + DX11 백엔드 초기화
	ImGui_ImplWin32_Init(g_hWnd); // ← 기존에 만든 윈도우 핸들 사용
	ImGui_ImplDX11_Init(m_pDevice, m_pContext); // DX11 초기화 때 만든 device/context


	return S_OK;
}

void CTool_Base::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	// ImGui 새 프레임 시작
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

void CTool_Base::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	// 뷰포트 기준 위치와 크기 설정 (메뉴바 등 고려된 안전한 방식)
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);

	// Base 창 시작 - 항상 뒤에 있고 완전 투명
	ImGui::Begin("Base", nullptr,
		ImGuiWindowFlags_NoBringToFrontOnFocus |  // 클릭해도 앞으로 안 옴
		ImGuiWindowFlags_NoBackground |           // 배경 안 그림 (투명)
		ImGuiWindowFlags_NoTitleBar |             // 제목 없음
		ImGuiWindowFlags_NoResize |               // 크기 고정
		ImGuiWindowFlags_NoMove |                 // 이동 불가
		ImGuiWindowFlags_NoScrollbar |            // 스크롤바 제거
		ImGuiWindowFlags_NoCollapse |             // 축소 비활성화
		ImGuiWindowFlags_NoSavedSettings          // 설정 저장 안 함
	);

	// DockSpace 생성 - 중앙 영역도 투명
	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);
	ImGui::End();
}

void CTool_Base::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

}

HRESULT CTool_Base::Render()
{
	__super::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());  // ImGui 그리기

	return S_OK;
}

HRESULT CTool_Base::Ready_Components()
{
	// 필요한 컴포넌트들 준비
	return S_OK;
}

CTool_Base* CTool_Base::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTool_Base* pInstance = new CTool_Base(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTool_Base");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTool_Base::Clone(void* pArg)
{
	CTool_Base* pInstance = new CTool_Base(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTool_Base");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTool_Base::Free()
{
	if (m_bCloned)
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	__super::Free();
}
