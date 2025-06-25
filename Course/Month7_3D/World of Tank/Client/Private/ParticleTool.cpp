#include "ParticleTool.h"
#include "GameInstance.h"

#include "VIBuffer_Point_Instance.h"

CParticleTool::CParticleTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CTool(pDevice, pContext)
{

}
CParticleTool::CParticleTool(const CParticleTool& Prototype)
	:CTool(Prototype)
{

}

HRESULT CParticleTool::Initialize_Prototype()
{

	return S_OK;
}
HRESULT CParticleTool::Initialize(void* pArg)
{
	CGameObject* pEffect = m_pGameInstance->Get_Last_GameObject(ENUM_CLASS(LEVEL::PARTICLETOOL), TEXT("Layer_Effect"));
	if (pEffect)
	{
		CComponent* pVIBuffer = pEffect->Get_Component(TEXT("Com_VIBuffer"));
		m_VIBuffer = static_cast<CVIBuffer_Point_Instance*>(pVIBuffer);
		Safe_AddRef(m_VIBuffer);
	}

	return S_OK;
}
void CParticleTool::Priority_Update(_float fTimeDelta)
{

}
void CParticleTool::Update(_float fTimeDelta)
{
	ImGui::Begin("Particle");

#pragma region 파티클 갯수
	const _int iMinCount = 1;
	const _int iMaxCount = 10000;

	// 텍스트 입력 바
	if (ImGui::InputInt("Instance Count", &m_iNumInstances))
	{
		m_iNumInstances = clamp(m_iNumInstances, iMinCount, iMaxCount);
		m_VIBuffer->Change_NumInstance(m_iNumInstances);
		m_VIBuffer->Replay();
	}

	// 슬라이더
	if (ImGui::SliderInt("##Slider", &m_iNumInstances, iMinCount, iMaxCount))
	{
		m_VIBuffer->Change_NumInstance(m_iNumInstances);
		m_VIBuffer->Replay();
	}

	// 상태 출력
	ImGui::Text("Current: %d / Max: %d", m_iNumInstances, iMaxCount);
#pragma endregion

	ImGui::Separator(); // 구분선 추가

#pragma region 스폰 범위
	if (ImGui::DragFloat3("Spawn Range", reinterpret_cast<_float*>(&m_vRange), 0.1f, 0.f, 1000.f))
	{
		m_VIBuffer->Change_Range(m_vRange);
		m_VIBuffer->Replay(); 
	}
#pragma endregion

	ImGui::Separator(); // 구분선 추가

#pragma region 방출 모양

	const _char* szShapes[] = { "DROP", "SPREAD", "END" };

	_int iCurrent = static_cast<_int>(m_eEmissionShape);
	if (ImGui::Combo("Emission Shape", &iCurrent, szShapes, IM_ARRAYSIZE(szShapes)))
	{
		m_eEmissionShape = static_cast<EMISSION_SHAPE>(iCurrent);

		// 선택이 바뀌었을 때 처리
		m_VIBuffer->Set_EmissionShape(m_eEmissionShape);
		m_VIBuffer->Replay(); 
	}
#pragma endregion

	ImGui::Separator(); // 구분선 추가

#pragma region 방출 피벗
	if (ImGui::DragFloat3("Emission Pivot", reinterpret_cast<_float*>(&m_vPivot), 0.1f, -100.f, 100.f))
	{
		m_VIBuffer->Change_Pivot(m_vPivot);
		m_VIBuffer->Replay(); 
	}
#pragma endregion

	ImGui::Separator(); // 구분선 추가

#pragma region 파티클 사이즈
	if (ImGui::DragFloat2("Particle Size", reinterpret_cast<_float*>(&m_vSize), 0.1f, 0.f, 100.f))
	{
		m_VIBuffer->Change_Size(m_vSize);
		m_VIBuffer->Replay();
	}
#pragma endregion

	ImGui::Separator(); // 구분선 추가

#pragma region 라이프 타임
	if (ImGui::DragFloat2("Life Time", reinterpret_cast<_float*>(&m_vLifeTime), 0.1f, 0.f, 100.f))
	{
		m_VIBuffer->Change_LifeTime(m_vLifeTime);
		m_VIBuffer->Replay(); 
	}
#pragma endregion

	ImGui::Separator(); // 구분선 추가

#pragma region 파티클 속도
	if (ImGui::DragFloat2("Particle Speed", reinterpret_cast<_float*>(&m_vSpeed), 0.1f, -100.f, 100.f))
	{
		m_VIBuffer->Change_Speed(m_vSpeed);
		m_VIBuffer->Replay(); 
	}
#pragma endregion

	ImGui::Separator(); // 구분선 추가

#pragma region 루프

	if (ImGui::Checkbox("Loop", &m_bLoop))
	{
		m_VIBuffer->Change_isLoop(m_bLoop);  // 루프 여부 전달
		m_VIBuffer->Replay();            
	}

#pragma endregion

	ImGui::Separator(); // 구분선 추가

#pragma region 리플레이
	if (ImGui::Button("Replay", ImVec2(80.f, 30.f)))
	{
		m_VIBuffer->Replay(); 
	}
#pragma endregion

	ImGui::End();

}
void CParticleTool::Late_Update(_float fTimeDelta)
{


	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);

}
HRESULT CParticleTool::Render()
{
	ImGui::Render();  // 렌더링 처리
	return S_OK;
}


CParticleTool* CParticleTool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CParticleTool* pInstance = new CParticleTool(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticleTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject* CParticleTool::Clone(void* pArg)
{

	CParticleTool* pInstance = new CParticleTool(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CParticleTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CParticleTool::Free()
{
	__super::Free();

	Safe_Release(m_VIBuffer);
}