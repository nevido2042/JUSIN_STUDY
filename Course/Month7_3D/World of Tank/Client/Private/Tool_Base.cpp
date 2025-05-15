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

	// ������ ��游 �����ϰ�
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.3f); // Window ��� ����


	// Win32 + DX11 �鿣�� �ʱ�ȭ
	ImGui_ImplWin32_Init(g_hWnd); // �� ������ ���� ������ �ڵ� ���
	ImGui_ImplDX11_Init(m_pDevice, m_pContext); // DX11 �ʱ�ȭ �� ���� device/context


	return S_OK;
}

void CTool_Base::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	// ImGui �� ������ ����
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

void CTool_Base::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

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
}

void CTool_Base::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

}

HRESULT CTool_Base::Render()
{
	__super::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());  // ImGui �׸���

	return S_OK;
}

HRESULT CTool_Base::Ready_Components()
{
	// �ʿ��� ������Ʈ�� �غ�
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
