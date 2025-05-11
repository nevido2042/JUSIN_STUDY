#include "MapTool.h"
#include "GameInstance.h"

#include "Terrain.h"

CMapTool::CMapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CMapTool::CMapTool(const CMapTool& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CMapTool::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMapTool::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTerrain = static_cast<CTerrain*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_Terrain"), 0));
	if (nullptr == m_pTerrain)
		return E_FAIL;
	Safe_AddRef(m_pTerrain);

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

void CMapTool::Priority_Update(_float fTimeDelta)
{
	// ImGui 새 프레임 시작
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

void CMapTool::Update(_float fTimeDelta)
{
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
	
	//하이어라키 창
	ImGui::Begin("Hierarchy", nullptr);

	ImGui::End();

	//Asset 창
	ImGui::Begin("Asset", nullptr);
	ImGui::End();

	//Detail 창
	ImGui::Begin("Detail", nullptr);
	ImGui::Text("Terrain_PickedPos");

	_float3 vPos = m_pTerrain->Get_PickedPos();
	ImGui::Text("x:%.2f, y:%.2f, z:%.2f", vPos.x, vPos.y, vPos.z);
	ImGui::End();

	ImGui::ShowDemoWindow();

#pragma region 기즈모
	// viewMatrix = 카메라 View 행렬
	// projMatrix = 카메라 Projection 행렬
	// objectMatrix = 조작 대상 행렬
	ImGuizmo::SetDrawlist();

	// 윈도우 위치 및 크기 설정 (옵션)
	ImGuizmo::SetRect(0, 0, (float)g_iWinSizeX, (float)g_iWinSizeY);

	// 1. XMMATRIX로부터 XMFLOAT4X4로 변환
	XMMATRIX viewMatrix = m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW);
	XMMATRIX projMatrix = m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ);
	XMMATRIX worldMatrix = XMMatrixInverse(nullptr, viewMatrix);

	// ImGuizmo는 float*를 받기 때문에 XMFLOAT4X4로 변환해야 함
	XMFLOAT4X4 viewMatrixF, projMatrixF, worldMatrixF;
	XMStoreFloat4x4(&viewMatrixF, XMMatrixTranspose(viewMatrix));
	XMStoreFloat4x4(&projMatrixF, XMMatrixTranspose(projMatrix));
	XMStoreFloat4x4(&worldMatrixF, XMMatrixTranspose(worldMatrix));

	// 2. ImGuizmo에 전달
	ImGuizmo::Manipulate(
		&viewMatrixF.m[0][0],
		&projMatrixF.m[0][0],
		ImGuizmo::TRANSLATE,   // 또는 ROTATE / SCALE
		ImGuizmo::LOCAL,
		&worldMatrixF.m[0][0]
	);

	// 3. 결과를 다시 XMMATRIX로 가져오기 (선택 사항)
	worldMatrix = XMMatrixTranspose(XMLoadFloat4x4(&worldMatrixF));
#pragma endregion
}

void CMapTool::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);

	// 렌더링 처리
	ImGui::Render();  // 렌더링 처리
}

HRESULT CMapTool::Render()
{
	// ImGui 그리기
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());  // ImGui 그리기

	return S_OK;
}

HRESULT CMapTool::Ready_Components()
{
	// 필요한 컴포넌트들 준비
	return S_OK;
}

CMapTool* CMapTool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMapTool* pInstance = new CMapTool(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMapTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMapTool::Clone(void* pArg)
{
	CMapTool* pInstance = new CMapTool(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMapTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMapTool::Free()
{
	if (m_bCloned)
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	Safe_Release(m_pTerrain);

	__super::Free();
}
