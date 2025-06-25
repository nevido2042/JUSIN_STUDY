#include "ParticleTool.h"
#include "GameInstance.h"

#include "VIBuffer_Point_Instance.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
//
//#define STB_IMAGE_RESIZE_IMPLEMENTATION
//#include "stb_image_resize2.h"
//
//#include "ImGuiFileDialog.h"

#include "BaseParticle.h"

_uint CParticleTool::m_strPrototypeTag_ID = { 0 };

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

		m_pBaseParticle = static_cast<CBaseParticle*>(pEffect);
		Safe_AddRef(m_pBaseParticle);
	}

	m_BaseParticleDesc.iNumInstance = 500;
	m_BaseParticleDesc.vCenter = _float3(0.0f, 0.f, 0.0f);
	m_BaseParticleDesc.vRange = _float3(0.2f, 0.2f, 0.2f);
	m_BaseParticleDesc.vSize = _float2(0.05f, 0.1f);
	m_BaseParticleDesc.vLifeTime = _float2(0.5f, 2.f);
	m_BaseParticleDesc.vSpeed = _float2(1.f, 2.f);
	m_BaseParticleDesc.vPivot = _float3(0.0f, 0.f, 0.f);
	m_BaseParticleDesc.isLoop = true;

	return S_OK;
}
void CParticleTool::Priority_Update(_float fTimeDelta)
{

}


void CParticleTool::Update(_float fTimeDelta)
{
	ImGui::Begin("Particle");

#pragma region 파티클 텍스쳐
	if (ImGui::Button("Change Texture")) 
	{
		wstring pTextureFilePath = OpenFileDialog();

		if (!pTextureFilePath.empty())
		{
			//임시로 사용할 프로토타입 이름 지정
			wstring strPrototypeTag = L"CParticleTool_Temp_" + to_wstring(m_strPrototypeTag_ID);

			//텍스쳐를 로드하고... 로딩이 완료되면 텍스쳐 바꿔 끼워주자.
			if (FAILED(Load_Texture(strPrototypeTag, pTextureFilePath)))
			{
				//이미 있을 수도 있음
				//MSG_BOX("이미지 로드 실패");
			}

			if (FAILED(Change_Texture(strPrototypeTag)))
			{
				MSG_BOX("텍스쳐 변경 실패");
			}
			else
			{
				++m_strPrototypeTag_ID;
			}
				
		}
	}

#pragma endregion

	ImGui::Separator(); // 구분선 추가

#pragma region 파티클 갯수
	const _int iMinCount = 1;
	const _int iMaxCount = 10000;

	// 텍스트 입력 바
	if (ImGui::InputInt("Instance Count", reinterpret_cast<_int*>(&m_BaseParticleDesc.iNumInstance)))
	{
		m_BaseParticleDesc.iNumInstance = clamp(static_cast<_int>(m_BaseParticleDesc.iNumInstance), iMinCount, iMaxCount);
		m_VIBuffer->Change_NumInstance(m_BaseParticleDesc.iNumInstance);
		m_VIBuffer->Replay();
	}

	// 슬라이더
	if (ImGui::SliderInt("##Slider", reinterpret_cast<_int*>(&m_BaseParticleDesc.iNumInstance), iMinCount, iMaxCount))
	{
		m_VIBuffer->Change_NumInstance(m_BaseParticleDesc.iNumInstance);
		m_VIBuffer->Replay();
	}

	// 상태 출력
	ImGui::Text("Current: %d / Max: %d", static_cast<_int>(m_BaseParticleDesc.iNumInstance), iMaxCount);
#pragma endregion

	ImGui::Separator(); // 구분선 추가

#pragma region 스폰 범위
	if (ImGui::DragFloat3("Spawn Range", reinterpret_cast<_float*>(&m_BaseParticleDesc.vRange), 0.1f, 0.f, 1000.f))
	{
		m_VIBuffer->Change_Range(m_BaseParticleDesc.vRange);
		m_VIBuffer->Replay(); 
	}
#pragma endregion

	ImGui::Separator(); // 구분선 추가

#pragma region 방출 모양

	const _char* szShapes[] = { "DROP", "SPREAD", "END" };

	_int iCurrent = static_cast<_int>(m_BaseParticleDesc.eEmissionShape);
	if (ImGui::Combo("Emission Shape", &iCurrent, szShapes, IM_ARRAYSIZE(szShapes)))
	{
		m_BaseParticleDesc.eEmissionShape = static_cast<EMISSION_SHAPE>(iCurrent);

		// 선택이 바뀌었을 때 처리
		m_VIBuffer->Set_EmissionShape(m_BaseParticleDesc.eEmissionShape);
		m_VIBuffer->Replay(); 
	}
#pragma endregion

	ImGui::Separator(); // 구분선 추가

#pragma region 방출 피벗
	if (ImGui::DragFloat3("Emission Pivot", reinterpret_cast<_float*>(&m_BaseParticleDesc.vPivot), 0.1f, -100.f, 100.f))
	{
		m_VIBuffer->Change_Pivot(m_BaseParticleDesc.vPivot);
		m_VIBuffer->Replay(); 
	}
#pragma endregion

	ImGui::Separator(); // 구분선 추가

#pragma region 파티클 사이즈
	if (ImGui::DragFloat2("Particle Size", reinterpret_cast<_float*>(&m_BaseParticleDesc.vSize), 0.001f, 0.001f, 10.f))
	{
		m_VIBuffer->Change_Size(m_BaseParticleDesc.vSize);
		m_VIBuffer->Replay();
	}
#pragma endregion

	ImGui::Separator(); // 구분선 추가

#pragma region 라이프 타임
	if (ImGui::DragFloat2("Life Time", reinterpret_cast<_float*>(&m_BaseParticleDesc.vLifeTime), 0.1f, 0.f, 100.f))
	{
		m_VIBuffer->Change_LifeTime(m_BaseParticleDesc.vLifeTime);
		m_VIBuffer->Replay(); 
	}
#pragma endregion

	ImGui::Separator(); // 구분선 추가

#pragma region 파티클 속도
	if (ImGui::DragFloat2("Particle Speed", reinterpret_cast<_float*>(&m_BaseParticleDesc.vSpeed), 0.1f, -100.f, 100.f))
	{
		m_VIBuffer->Change_Speed(m_BaseParticleDesc.vSpeed);
		m_VIBuffer->Replay(); 
	}
#pragma endregion

	ImGui::Separator(); // 구분선 추가

#pragma region 루프

	if (ImGui::Checkbox("Loop", &m_BaseParticleDesc.isLoop))
	{
		m_VIBuffer->Change_isLoop(m_BaseParticleDesc.isLoop);  // 루프 여부 전달
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

#pragma region 파티클 저장

	ImGui::InputText("Particle Name", m_szParticleName, IM_ARRAYSIZE(m_szParticleName));

	if (ImGui::Button("Save Particle", ImVec2(120.f, 30.f)))
	{
		// 널 스트링 체크
		if (strlen(m_szParticleName) > 0)
		{
			// char, wstring 변환
			wstring wsParticleName(m_szParticleName, m_szParticleName + strlen(m_szParticleName));

			Save_Particle(wsParticleName);
		}
		else
		{
			// 이름이 비어 있으면 경고를 띄우거나 로그 출력
			MSG_BOX("파티클 이름을 입력하세요.\n");
		}
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

#include <windows.h>
#include <commdlg.h>  // GetOpenFileName
wstring CParticleTool::OpenFileDialog()
{
	TCHAR szInitialDir[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, szInitialDir);  // 원래 작업 디렉터리 저장

	OPENFILENAME ofn;       // common dialog box structure
	_tchar szFile[MAX_PATH] = { 0 };       // buffer for file name

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;  // 필요하면 윈도우 핸들 넣기
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Image Files\0*.png;*.jpg;*.dds;*.tga\0All Files\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;



	if (GetOpenFileName(&ofn) == TRUE) 
	{
		// 원래 디렉터리로 복원
		SetCurrentDirectory(szInitialDir);
		return _wstring(ofn.lpstrFile);
	}

	// 원래 디렉터리로 복원
	SetCurrentDirectory(szInitialDir);
	return wstring(); // 취소 등 실패 시 빈 문자열 반환
}

HRESULT CParticleTool::Load_Texture(const wstring& strPrototypeTag, const wstring& pTextureFilePath)
{
	/* For.Prototype_Component_Texture_Smoke*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::PARTICLETOOL), strPrototypeTag,
		CTexture::Create(m_pDevice, m_pContext, pTextureFilePath.c_str(), 1))))
		return E_FAIL;

	return S_OK;

}

HRESULT CParticleTool::Change_Texture(const wstring& strPrototypeTag)
{
	return m_pBaseParticle->Change_Texture(ENUM_CLASS(LEVEL::PARTICLETOOL), strPrototypeTag);

	return S_OK;
}

HRESULT CParticleTool::Save_Particle(const wstring& ParticleName)
{
	/* For.Prototype_Component_VIBuffer_BaseParticle*/
	//CVIBuffer_Point_Instance::POINT_INSTANCE_DESC		BaseDesc{};
	//BaseDesc.iNumInstance = 500;
	//BaseDesc.vCenter = _float3(0.0f, 0.f, 0.0f);
	//BaseDesc.vRange = _float3(0.2f, 0.2f, 0.2f);
	//BaseDesc.vSize = _float2(0.05f, 0.1f);
	//BaseDesc.vLifeTime = _float2(0.5f, 2.f);
	//BaseDesc.vSpeed = _float2(1.f, 2.f);
	//BaseDesc.vPivot = _float3(0.0f, 0.f, 0.f);
	//BaseDesc.isLoop = true;

	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::PARTICLETOOL), TEXT("Prototype_Component_VIBuffer_BaseParticle"),
	//	CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, &BaseDesc))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_BaseParticle */
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::PARTICLETOOL), TEXT("Prototype_GameObject_BaseParticle"),
	//	CBaseParticle::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	//파티클 저정한다. (컴포넌트, 게임오브젝트)
	//1. 컴포넌트 (레벨(static), 컴포넌트 태그)
	//2. 게임오브젝트(레벨(static), 게임오브젝트 태그)
	//3. CVIBuffer_Point_Instance::POINT_INSTANCE_DESC m_BaseParticleDesc

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
	Safe_Release(m_pBaseParticle);
}