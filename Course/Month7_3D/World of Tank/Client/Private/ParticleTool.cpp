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

	//if (m_pGameInstance->Key_Down(DIK_1))
	//{
	//	m_VIBuffer->Change_Range(_float3(1.f, 1.f, 1.f));
	//	m_VIBuffer->Reset();
	//}
	//if (m_pGameInstance->Key_Down(DIK_2))
	//{
	//	m_VIBuffer->Change_Range(_float3(2.f, 2.f, 2.f));
	//	m_VIBuffer->Reset();
	//}
	//if (m_pGameInstance->Key_Down(DIK_3))
	//{
	//	m_VIBuffer->Change_Range(_float3(3.f, 3.f, 3.f));
	//	m_VIBuffer->Reset();
	//}

	ImGui::Begin("Particle");

#pragma region 파티클 갯수
	const _int iMinCount = 1;
	const _int iMaxCount = 10000;

	// 텍스트 입력 바
	if (ImGui::InputInt("Instance Count", &m_iNumInstances))
	{
		m_iNumInstances = clamp(m_iNumInstances, iMinCount, iMaxCount);
		m_VIBuffer->Change_NumInstance(m_iNumInstances);
		m_VIBuffer->Reset();
	}

	// 슬라이더
	if (ImGui::SliderInt("Slider", &m_iNumInstances, iMinCount, iMaxCount))
	{
		m_VIBuffer->Change_NumInstance(m_iNumInstances);
		m_VIBuffer->Reset();
	}

	// 상태 출력
	ImGui::Text("Current: %d / Max: %d", m_iNumInstances, iMaxCount);
#pragma endregion

#pragma region 스폰 범위
	if (ImGui::DragFloat3("Spawn Range", reinterpret_cast<_float*>(&m_vRange), 0.1f, 0.f, 1000.f))
	{
		m_VIBuffer->Change_Range(m_vRange);
		m_VIBuffer->Reset(); // 변경 즉시 반영
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