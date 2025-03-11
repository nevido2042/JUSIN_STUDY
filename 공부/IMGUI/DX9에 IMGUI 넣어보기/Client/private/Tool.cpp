#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <tchar.h>

#include "Tool.h"
#include "GameInstance.h"

CTool::CTool(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject { pGraphic_Device }
{

}

CTool::CTool(const CTool& Prototype)
    : CGameObject { Prototype }
{

}

HRESULT CTool::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTool::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

    return S_OK;
}

void CTool::Priority_Update(_float fTimeDelta)
{
	int a = 10;
}

void CTool::Update(_float fTimeDelta)
{

}

void CTool::Late_Update(_float fTimeDelta)
{	
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;
}

HRESULT CTool::Render()
{

	ImGuiIO& io = ImGui::GetIO();

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Dear ImGui ������ ����
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// 1. ū ���� â ǥ�� (��κ��� ���� �ڵ�� ImGui::ShowDemoWindow()�� ����! Dear ImGui�� �� �� �����Ϸ��� �ش� �ڵ带 ���캼 �� ����)
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. ���� ���� ������ â ǥ��. Begin/End ���� ����Ͽ� �̸��� ������ â�� ������.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // "Hello, world!"��� â�� �����ϰ� ������ �߰���.

		ImGui::Text("This is some useful text.");               // �ؽ�Ʈ ǥ�� (���� ���ڿ��� ����� �� ����)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // â�� ����/���� ���¸� �����ϴ� bool ���� ����
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // 0.0f���� 1.0f���� ���� ������ �����̴� ����
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // 3���� float ���� �����Ͽ� ������ ����

		if (ImGui::Button("Button"))                            // ��ư�� Ŭ���ϸ� true ��ȯ (��κ��� ������ �����ǰų� Ȱ��ȭ�� �� true ��ȯ)
			counter++;
		ImGui::SameLine();                                      // ���� �ٿ� ���� ���� ��ġ
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
	}

	// 3. �� �ٸ� ������ â ǥ��.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // bool ������ �����ϸ� â�� �ݱ� ��ư�� Ŭ���ϸ� �ش� ������ false�� ������
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	// ������
	ImGui::EndFrame();
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
	//D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f), (int)(clear_color.y * clear_color.w * 255.0f), (int)(clear_color.z * clear_color.w * 255.0f), (int)(clear_color.w * 255.0f));
	//m_pGraphic_Device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return S_OK;
}


HRESULT CTool::Ready_Components()
{

	return S_OK;
}

CTool* CTool::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTool* pInstance = new CTool(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTool::Clone(void* pArg)
{	
	CTool* pInstance = new CTool(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTool::Free()
{
	__super::Free();

}
