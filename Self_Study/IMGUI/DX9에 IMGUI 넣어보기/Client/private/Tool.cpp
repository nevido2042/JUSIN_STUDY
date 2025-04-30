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

	// Dear ImGui 프레임 시작
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// 1. 큰 데모 창 표시 (대부분의 샘플 코드는 ImGui::ShowDemoWindow()에 있음! Dear ImGui를 더 잘 이해하려면 해당 코드를 살펴볼 수 있음)
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. 직접 만든 간단한 창 표시. Begin/End 쌍을 사용하여 이름이 지정된 창을 생성함.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // "Hello, world!"라는 창을 생성하고 내용을 추가함.

		ImGui::Text("This is some useful text.");               // 텍스트 표시 (형식 문자열도 사용할 수 있음)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // 창의 열림/닫힘 상태를 저장하는 bool 값을 편집
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // 0.0f에서 1.0f까지 조절 가능한 슬라이더 생성
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // 3개의 float 값을 편집하여 색상을 조절

		if (ImGui::Button("Button"))                            // 버튼을 클릭하면 true 반환 (대부분의 위젯은 편집되거나 활성화될 때 true 반환)
			counter++;
		ImGui::SameLine();                                      // 같은 줄에 다음 위젯 배치
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
	}

	// 3. 또 다른 간단한 창 표시.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // bool 변수를 전달하면 창의 닫기 버튼을 클릭하면 해당 변수가 false로 설정됨
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	// 렌더링
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
