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
	m_BaseParticleDesc.fAlpha = 1.f;
	m_BaseParticleDesc.isLoop = true;

	m_VIBuffer->Change_Desc(m_BaseParticleDesc);

	Load_Particles();

	return S_OK;
}
void CParticleTool::Priority_Update(_float fTimeDelta)
{

}


void CParticleTool::Update(_float fTimeDelta)
{
	ParticleList();

	ParticleControl();
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
	// 저장할 파일 경로
	wstring wstrFullPath = L"../Bin/Particles/" + ParticleName + L".txt";

	// 유니코드 경로를 ANSI 문자열로 변환
	_char szFullPath[MAX_PATH];
	WideCharToMultiByte(CP_ACP, 0, wstrFullPath.c_str(), -1, szFullPath, MAX_PATH, nullptr, nullptr);

	// 파일 열기 (쓰기 모드, 텍스트 저장)
	FILE* pFile = nullptr;
	fopen_s(&pFile, szFullPath, "w");

	if (pFile)
	{
		// 3. 파티클 디스크립션 정보
		const CVIBuffer_Point_Instance::POINT_INSTANCE_DESC& Desc = m_BaseParticleDesc;
		fwprintf(pFile, L"ParticleDesc:\n");
		fwprintf(pFile, L"  ParticleName : %s\n", ParticleName.c_str());
		fwprintf(pFile, L"  SpawnRange: (%f, %f, %f)\n", Desc.vRange.x, Desc.vRange.y, Desc.vRange.z);
		fwprintf(pFile, L"  MinScale: %f, MaxScale: %f\n", Desc.vSize.x, Desc.vSize.y);
		fwprintf(pFile, L"  LifeTimeMin: %f, LifeTimeMax: %f\n", Desc.vLifeTime.x, Desc.vLifeTime.y);
		fwprintf(pFile, L"  EmissionShape: %d\n", Desc.eEmissionShape);
		fwprintf(pFile, L"  NumInstance: %d\n", Desc.iNumInstance);
		fwprintf(pFile, L"  isLoop: %d\n", Desc.isLoop);
		fwprintf(pFile, L"  Alpha: %f\n", Desc.fAlpha);
		fwprintf(pFile, L"  Center: (%f, %f, %f) \n", Desc.vCenter.x, Desc.vCenter.y, Desc.vCenter.z);
		fwprintf(pFile, L"  Pivot: (%f, %f, %f)\n", Desc.vPivot.x, Desc.vPivot.y, Desc.vPivot.z);
		fwprintf(pFile, L"  MinSpeed: %f, MaxSpeed: %f\n", Desc.vSpeed.x, Desc.vSpeed.y);

		fclose(pFile);

		memset(m_szParticleName, 0, sizeof(m_szParticleName));
	}
	else
	{
		MSG_BOX("파일 저장에 실패했습니다.");
	}

	Load_Particles();

	return S_OK;
}


HRESULT CParticleTool::Load_Particles()
{
	m_ParticleDescs.clear();
	vector<wstring> ParticleFileNames = Get_ParticleFileNames();

	for (const wstring& FileName : ParticleFileNames)
	{
		// 저장할 파일 경로
		wstring wstrFullPath = L"../Bin/Particles/" + FileName + L".txt";

		// 유니코드 경로를 ANSI 문자열로 변환
		_char szFullPath[MAX_PATH];
		WideCharToMultiByte(CP_ACP, 0, wstrFullPath.c_str(), -1, szFullPath, MAX_PATH, nullptr, nullptr);

		// 파일 열기
		FILE* pFile = nullptr;
		_wfopen_s(&pFile, wstrFullPath.c_str(), L"r");

		// 파싱용 임시 변수
		_tchar szLine[MAX_PATH];
		CVIBuffer_Point_Instance::POINT_INSTANCE_DESC Desc{};


		if (!pFile)
		{
			MSG_BOX("파일 열기에 실패했습니다.");
			return E_FAIL;
		}

		// 파싱 시작
		while (fgetws(szLine, sizeof(szLine) / sizeof(_tchar), pFile))
		{
			if (wcsstr(szLine, L"ParticleName"))
				swscanf_s(szLine, L"  ParticleName : %ls\n", Desc.szName, (unsigned)_countof(Desc.szName));
			else if (wcsstr(szLine, L"SpawnRange"))
				swscanf_s(szLine, L"  SpawnRange: (%f, %f, %f)", &Desc.vRange.x, &Desc.vRange.y, &Desc.vRange.z);
			else if (wcsstr(szLine, L"MinScale"))
				swscanf_s(szLine, L"  MinScale: %f, MaxScale: %f", &Desc.vSize.x, &Desc.vSize.y);
			else if (wcsstr(szLine, L"LifeTimeMin"))
				swscanf_s(szLine, L"  LifeTimeMin: %f, LifeTimeMax: %f", &Desc.vLifeTime.x, &Desc.vLifeTime.y);
			else if (wcsstr(szLine, L"EmissionShape"))
				swscanf_s(szLine, L"  EmissionShape: %d", (int*)&Desc.eEmissionShape);
			else if (wcsstr(szLine, L"NumInstance"))
				swscanf_s(szLine, L"  NumInstance: %d", &Desc.iNumInstance);
			else if (wcsstr(szLine, L"isLoop"))
			{
				_int iLoop = 0;
				swscanf_s(szLine, L"  isLoop: %d", &iLoop);
				Desc.isLoop = (iLoop != 0);
			}
			else if (wcsstr(szLine, L"Alpha"))
				swscanf_s(szLine, L"  Alpha: %f", &Desc.fAlpha);
			else if (wcsstr(szLine, L"Center"))
				swscanf_s(szLine, L"  Center: (%f, %f, %f)", &Desc.vCenter.x, &Desc.vCenter.y, &Desc.vCenter.z);
			else if (wcsstr(szLine, L"Pivot"))
				swscanf_s(szLine, L"  Pivot: (%f, %f, %f)", &Desc.vPivot.x, &Desc.vPivot.y, &Desc.vPivot.z);
			else if (wcsstr(szLine, L"MinSpeed"))
				swscanf_s(szLine, L"  MinSpeed: %f, MaxSpeed: %f", &Desc.vSpeed.x, &Desc.vSpeed.y);
		}

		fclose(pFile);

		m_ParticleDescs.push_back(Desc);
	}

	return S_OK;

}

vector<wstring> CParticleTool::Get_ParticleFileNames()
{
	vector<wstring> vecFileNames;

	wstring folderPath = L"../Bin/Particles";
	for (const auto& entry : filesystem::directory_iterator(folderPath))
	{
		if (entry.is_regular_file())
		{
			wstring filePath = entry.path().filename().wstring();

			// 확장자가 .txt인지 확인
			if (entry.path().extension() == L".txt")
			{
				// .txt 제거한 순수 파일 이름만 추출
				wstring fileNameWithoutExt = entry.path().stem().wstring();
				vecFileNames.push_back(fileNameWithoutExt);
			}
		}
	}

	return vecFileNames;
}


//HRESULT CParticleTool::Make_Particles_List()
//{
//	vector<CVIBuffer_Point_Instance::POINT_INSTANCE_DESC> m_ParticleDescs = {};
//
//	return E_NOTIMPL;
//}

void CParticleTool::ParticleControl()
{
	ImGui::Begin("Particle Control");

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

#pragma region 투명도
	if (ImGui::DragFloat("Particle Alpha", reinterpret_cast<_float*>(&m_BaseParticleDesc.fAlpha), 0.1f, 0.f, 10.f))
	{
		m_VIBuffer->Change_Alpha(m_BaseParticleDesc.fAlpha);
		//m_VIBuffer->Replay();
	}
#pragma endregion


	ImGui::Separator(); // 구분선 추가

#pragma region 쉐이더 패스
	if (ImGui::SliderInt("Shader Pass", &m_iShaderPass, 0, 1))
	{
		m_pBaseParticle->Change_ShaderPass(m_iShaderPass);
	}
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Select shader pass index (0: Default, 1: Smoke)");
#pragma endregion


	ImGui::Separator(); // 구분선 추가

#pragma region 리플레이
	if (ImGui::Button("Replay", ImVec2(80.f, 30.f)))
	{
		m_VIBuffer->Replay();
	}
#pragma endregion

	ImGui::Separator(); // 구분선 추가

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

void CParticleTool::ParticleList()
{
	ImGui::Begin("Particle List");

	_int iIndex = 0;

	for (const auto& Desc : m_ParticleDescs)
	{
		_bool isSelected = (iIndex == m_SelectedIndex);

		_char szNameUTF8[MAX_PATH] = {};
		WideCharToMultiByte(CP_UTF8, 0, Desc.szName, -1, szNameUTF8, sizeof(szNameUTF8), nullptr, nullptr);

		if (ImGui::Selectable(szNameUTF8, isSelected))
		{
			m_SelectedIndex = iIndex;

			// 선택된 파티클에서 베이스 디스크립션을 가져와 채운다
			m_BaseParticleDesc = Desc;
			m_VIBuffer->Change_Desc(Desc);
			m_VIBuffer->Replay();
		}

		++iIndex;
	}

	ImGui::End();
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