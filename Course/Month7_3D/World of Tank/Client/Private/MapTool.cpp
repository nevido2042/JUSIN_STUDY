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

	// ��Ÿ�� ���� (����)
	ImGui::StyleColorsDark();

	// Win32 + DX11 �鿣�� �ʱ�ȭ
	ImGui_ImplWin32_Init(g_hWnd); // �� ������ ���� ������ �ڵ� ���
	ImGui_ImplDX11_Init(m_pDevice, m_pContext); // DX11 �ʱ�ȭ �� ���� device/context

	return S_OK;
}

void CMapTool::Priority_Update(_float fTimeDelta)
{
	// ImGui �� ������ ����
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

void CMapTool::Update(_float fTimeDelta)
{
	// ����Ʈ ���� ��ġ�� ũ�� ���� (�޴��� �� ����� ������ ���)
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);

	// Base â ���� - �׻� �ڿ� �ְ� ���� ����
	ImGui::Begin("Base", nullptr,
		ImGuiWindowFlags_NoBringToFrontOnFocus |  // Ŭ���ص� ������ �� ��
		ImGuiWindowFlags_NoBackground |           // ��� �� �׸� (����)
		ImGuiWindowFlags_NoTitleBar |             // ���� ����
		ImGuiWindowFlags_NoResize |               // ũ�� ����
		ImGuiWindowFlags_NoMove |                 // �̵� �Ұ�
		ImGuiWindowFlags_NoScrollbar |            // ��ũ�ѹ� ����
		ImGuiWindowFlags_NoCollapse |             // ��� ��Ȱ��ȭ
		ImGuiWindowFlags_NoSavedSettings          // ���� ���� �� ��
	);

	// DockSpace ���� - �߾� ������ ����
	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);

	ImGui::End();
	
	//���̾��Ű â
	ImGui::Begin("Hierarchy", nullptr);

	ImGui::End();

	//Asset â
	ImGui::Begin("Asset", nullptr);
	ImGui::End();

	//Detail â
	ImGui::Begin("Detail", nullptr);
	ImGui::Text("Terrain_PickedPos");

	_float3 vPos = m_pTerrain->Get_PickedPos();
	ImGui::Text("x:%.2f, y:%.2f, z:%.2f", vPos.x, vPos.y, vPos.z);
	ImGui::End();

	ImGui::ShowDemoWindow();

#pragma region �����
	// viewMatrix = ī�޶� View ���
	// projMatrix = ī�޶� Projection ���
	// objectMatrix = ���� ��� ���
	ImGuizmo::SetDrawlist();

	// ������ ��ġ �� ũ�� ���� (�ɼ�)
	ImGuizmo::SetRect(0, 0, (float)g_iWinSizeX, (float)g_iWinSizeY);

	// 1. XMMATRIX�κ��� XMFLOAT4X4�� ��ȯ
	XMMATRIX viewMatrix = m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW);
	XMMATRIX projMatrix = m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ);
	XMMATRIX worldMatrix = XMMatrixInverse(nullptr, viewMatrix);

	// ImGuizmo�� float*�� �ޱ� ������ XMFLOAT4X4�� ��ȯ�ؾ� ��
	XMFLOAT4X4 viewMatrixF, projMatrixF, worldMatrixF;
	XMStoreFloat4x4(&viewMatrixF, XMMatrixTranspose(viewMatrix));
	XMStoreFloat4x4(&projMatrixF, XMMatrixTranspose(projMatrix));
	XMStoreFloat4x4(&worldMatrixF, XMMatrixTranspose(worldMatrix));

	// 2. ImGuizmo�� ����
	ImGuizmo::Manipulate(
		&viewMatrixF.m[0][0],
		&projMatrixF.m[0][0],
		ImGuizmo::TRANSLATE,   // �Ǵ� ROTATE / SCALE
		ImGuizmo::LOCAL,
		&worldMatrixF.m[0][0]
	);

	// 3. ����� �ٽ� XMMATRIX�� �������� (���� ����)
	worldMatrix = XMMatrixTranspose(XMLoadFloat4x4(&worldMatrixF));
#pragma endregion
}

void CMapTool::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);

	// ������ ó��
	ImGui::Render();  // ������ ó��
}

HRESULT CMapTool::Render()
{
	// ImGui �׸���
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());  // ImGui �׸���

	return S_OK;
}

HRESULT CMapTool::Ready_Components()
{
	// �ʿ��� ������Ʈ�� �غ�
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
