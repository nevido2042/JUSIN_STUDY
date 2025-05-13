#include "MapTool.h"
#include "GameInstance.h"

#include "Terrain.h"
#include "Layer.h"

CMapTool::CMapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTool{ pDevice, pContext }
{
}

CMapTool::CMapTool(const CMapTool& Prototype)
	: CTool(Prototype)
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

	if (FAILED(Get_Assets()))
		return E_FAIL;

	m_pTerrain = static_cast<CTerrain*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_Terrain"), 0));
	if (nullptr == m_pTerrain)
		return E_FAIL;
	Safe_AddRef(m_pTerrain);

	if (FAILED(Load_Map()))
		return E_FAIL;

	return S_OK;
}

void CMapTool::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CMapTool::Update(_float fTimeDelta)
{
#pragma region Hierarchy
	ImGui::Begin("Hierarchy", nullptr);

	vector<string> HierarchyNames = {};
	if (ImGui::BeginListBox("##HierarchyList", ImVec2(-FLT_MIN, 300)))
	{
		_uint i = 0;
		for (auto& pLayer : m_pGameInstance->Get_Layers(ENUM_CLASS(LEVEL::MAPTOOL)))
		{
			string strLayerName;
			strLayerName.resize(MAX_PATH);
			size_t convertedChars = 0;
			wcstombs_s(&convertedChars, &strLayerName[0], MAX_PATH, pLayer.first.c_str(), _TRUNCATE);
			strLayerName.resize(convertedChars > 0 ? convertedChars - 1 : 0);

			//  TreeNode로 접기 / 펼치기 만들기
			if (ImGui::TreeNode(strLayerName.c_str()))
			{
				_uint j = 0;
				for (auto pGameObject : pLayer.second->Get_GameObjects())
				{
					string strHierachyName = strLayerName + '_' + to_string(j);
					++j;

					// "Prototype_GameObject_" 필터링 (원하면 주석 풀고)
					// if (strHierachyName.find("Prototype_GameObject_") == string::npos)
					//     continue;

					const _bool isSelected = (m_iSelectedHierarchyIndex == i);
					HierarchyNames.push_back(strHierachyName);

					if (ImGui::Selectable(strHierachyName.c_str(), isSelected))
					{
						m_iSelectedHierarchyIndex = i;
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();

					++i;
				}

				ImGui::TreePop(); // 접기 닫기
			}
		}
		ImGui::EndListBox();
	}

	
	if (ImGui::Button("Delete"))
	{
		CGameObject* pGameObject = Get_Selected_GameObject(HierarchyNames);
		if (nullptr != pGameObject)
			pGameObject->Destroy();
	}
	if (ImGui::Button("Save Map"))
	{
		Save_Map();
	}
	if (ImGui::Button("Load Map"))
	{
		//프로토타입 어쩌구 다 삭제하고
		Load_Map();
	}
	ImGui::End();
#pragma endregion

#pragma region Asset
	ImGui::Begin("Asset", nullptr);

	if (ImGui::Button("Clone"))
	{
		if (m_iSelectedAssetIndex != -1 && m_iSelectedAssetIndex < m_AssetNames.size())
		{
			m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), m_AssetNames[m_iSelectedAssetIndex].c_str(), ENUM_CLASS(LEVEL::MAPTOOL), m_AssetNames[m_iSelectedAssetIndex].c_str());
		}
	}

	if (ImGui::BeginListBox("##AssetList", ImVec2(-FLT_MIN, 200)))
	{
		for (_int i = 0; i < m_AssetNames.size(); ++i)
		{
			string strAssetName;
			strAssetName.resize(MAX_PATH);  // 버퍼 확보
			size_t convertedChars = 0;
			wcstombs_s(&convertedChars, &strAssetName[0], MAX_PATH, m_AssetNames[i].c_str(), _TRUNCATE);
			strAssetName.resize(convertedChars > 0 ? convertedChars - 1 : 0);  // 널 문자 제외하고 자르기

			const _bool isSelected = (m_iSelectedAssetIndex == i);
			if (ImGui::Selectable(strAssetName.c_str(), isSelected))
			{
				m_iSelectedAssetIndex = i;
			}

			// 선택된 항목에 포커스
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}

	ImGui::End();
#pragma endregion

#pragma region Detail

	ImGui::Begin("Detail", nullptr);


	// 피킹된 위치 표시
	ImGui::Text("Terrain_PickedPos(L-Ctrl: Set Pos) ");
	_float3 vPos = m_pTerrain->Get_PickedPos();
	ImGui::Text("x:%.2f, y:%.2f, z:%.2f", vPos.x, vPos.y, vPos.z);

	ImGui::Separator();

	// 선택된 GameObject 가져오기
	ImGui::Text("Position");
	CGameObject* pGameObject = Get_Selected_GameObject(HierarchyNames);

	if (pGameObject != nullptr)
	{
		// Transform Component에서 월드 행렬 얻기
		CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(g_strTransformTag));

		if(m_pGameInstance->Key_Pressing(DIK_LCONTROL) && m_pGameInstance->Mouse_Down(ENUM_CLASS(DIMK::LBUTTON)))
			pTransform->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&vPos), 1.f));
#pragma region Guizmo
		_float4x4 WorldMatrix;
		XMStoreFloat4x4(&WorldMatrix, pTransform->Get_WorldMatrix()); // <- XMMATRIX에서 변환한 형태

		// 행렬을 float 배열로 변환 (ImGuizmo는 float[16] 배열 씀)
		_float matrix[16];
		memcpy(matrix, &WorldMatrix, sizeof(_float) * 16);

		// View / Projection 매트릭스 (카메라 기준)
		_matrix View = m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW);
		_matrix Projection = m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ);

		// View / Projection을 float[16] 배열로 변환
		_float viewMatrix[16];
		_float projMatrix[16];
		XMStoreFloat4x4(reinterpret_cast<_float4x4*>(viewMatrix), View);
		XMStoreFloat4x4(reinterpret_cast<_float4x4*>(projMatrix), Projection);

		// ImGuizmo 설정 (전체 화면 기준으로 띄움)
		ImGuizmo::BeginFrame();
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());
		ImVec2 displaySize = ImGui::GetIO().DisplaySize;
		ImGuizmo::SetRect(0, 0, displaySize.x, displaySize.y);

		// Guizmo 조작 (Translate / Rotate / Scale)
		ImGuizmo::Manipulate(viewMatrix, projMatrix, ImGuizmo::TRANSLATE, ImGuizmo::LOCAL, matrix);

		// Guizmo 사용중이면 Transform 반영
		if (ImGuizmo::IsUsing())
		{
			memcpy(&WorldMatrix, matrix, sizeof(_float) * 16);
			pTransform->Set_WorldMatrix(WorldMatrix);
		}
#pragma endregion
#pragma region Input Position
		// 현재 Position 값 얻기
		XMStoreFloat3(&vPos, pTransform->Get_State(STATE::POSITION));

		//Position을 직접 입력받기 (InputFloat3)
		if (ImGui::InputFloat3("", reinterpret_cast<_float*>(&vPos), "%.2f"))
		{
			// 입력된 값으로 Transform 포지션 다시 설정
			pTransform->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&vPos), 1.f));
		}
	}
#pragma endregion
	ImGui::End();

#pragma endregion



	ImGui::ShowDemoWindow();
}

void CMapTool::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);

	// 렌더링 처리
	ImGui::Render();  // 렌더링 처리
}

HRESULT CMapTool::Render()
{


	return S_OK;
}

CGameObject* CMapTool::Get_Selected_GameObject(vector<string>& HierarchyNames)
{
	const string prefix = "Prototype_GameObject_";

	if (m_iSelectedHierarchyIndex < HierarchyNames.size())
	{
		// prefix 제거
		size_t prefixPos = HierarchyNames[m_iSelectedHierarchyIndex].find(prefix);
		if (prefixPos != string::npos) {
			string remain = HierarchyNames[m_iSelectedHierarchyIndex].substr(prefixPos + prefix.length());

			// remain 은 이제 "XXX_0" 형태
			size_t underscorePos = remain.rfind('_');
			string objectName;
			string objectNumber;
			if (underscorePos != string::npos)
			{
				objectName = remain.substr(0, underscorePos);  // XXX
				objectNumber = remain.substr(underscorePos + 1);  // 0
			}

			string LayerName = prefix + objectName;

			return m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::MAPTOOL), wstring(LayerName.begin(), LayerName.end()), stoi(objectNumber));
		}
	}

	return nullptr;
}

HRESULT CMapTool::Delete_All()
{
	const wstring wstrPrefix = L"Prototype_GameObject_";

	for (auto& pLayer : m_pGameInstance->Get_Layers(ENUM_CLASS(LEVEL::MAPTOOL)))
	{
		if (pLayer.first.find(wstrPrefix) != wstring::npos)
		{
			for (auto& pGameObject : pLayer.second->Get_GameObjects())
			{
				pGameObject->Destroy();
			}
		}
	}

	return S_OK;
}


HRESULT CMapTool::Save_Map()
{
	// 폴더 없으면 생성 (안해도 된다면 생략 가능)
	if (!filesystem::exists("../Bin/Map/"))
		filesystem::create_directories("../Bin/Map/");

	filesystem::path MapPath = "../Bin/Map/Map.bin";

	FILE* fp = nullptr;
	fopen_s(&fp, MapPath.string().c_str(), "wb");
	if (!fp)
		return E_FAIL;

	// GameObject 루프
	for (auto& pLayer : m_pGameInstance->Get_Layers(ENUM_CLASS(LEVEL::MAPTOOL)))
	{
		for (auto pGameObject : pLayer.second->Get_GameObjects())
		{
			// 이름 (Layer 이름 기반)
			string strHierachyName;
			strHierachyName.resize(MAX_PATH);  // 버퍼 확보
			size_t convertedChars = 0;
			wcstombs_s(&convertedChars, &strHierachyName[0], MAX_PATH, pLayer.first.c_str(), _TRUNCATE);
			strHierachyName.resize(convertedChars > 0 ? convertedChars - 1 : 0);  // 널 문자 제외

			if (strHierachyName.find("Prototype_GameObject_") == string::npos)
				continue;

			// Transform 위치 가져오기
			CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(g_strTransformTag));
			_float3 vPos = {};
			if (pTransform != nullptr)
			{
				XMStoreFloat3(&vPos, pTransform->Get_State(STATE::POSITION));
			}

			// 이름 길이 먼저 저장
			_uint iNameLength = static_cast<_uint>(strHierachyName.length());
			fwrite(&iNameLength, sizeof(_uint), 1, fp);

			// 이름 데이터 저장
			fwrite(strHierachyName.c_str(), sizeof(char), iNameLength, fp);

			// 위치 데이터 저장
			fwrite(&vPos, sizeof(_float3), 1, fp);
		}
	}

	fclose(fp);

	return S_OK;
}


HRESULT CMapTool::Load_Map()
{
	Delete_All();

	filesystem::path MapPath = "../Bin/Map/Map.bin";

	FILE* fp = nullptr;
	fopen_s(&fp, MapPath.string().c_str(), "rb");
	if (!fp)
		return E_FAIL;

	while (true)
	{
		_uint iNameLength = 0;
		size_t readCount = fread(&iNameLength, sizeof(_uint), 1, fp);

		// 더 이상 읽을게 없으면 종료
		if (readCount != 1)
			break;

		// 이름 읽기
		string strHierachyName;
		strHierachyName.resize(iNameLength);
		fread(&strHierachyName[0], sizeof(char), iNameLength, fp);

		// 위치 읽기
		_float3 vPos = {};
		fread(&vPos, sizeof(_float3), 1, fp);

		wstring GameObjectName = { strHierachyName.begin(), strHierachyName.end() };

		// GameObject 생성
		m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), GameObjectName.c_str(), ENUM_CLASS(LEVEL::MAPTOOL), GameObjectName.c_str());
		CGameObject* pGameObject = m_pGameInstance->Get_Last_GameObject(ENUM_CLASS(LEVEL::MAPTOOL), GameObjectName.c_str());
		if (pGameObject == nullptr)
			continue;

		// Transform 위치 설정
		CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(g_strTransformTag));
		if (pTransform != nullptr)
		{
			pTransform->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&vPos), 1.f));
		}
	}

	fclose(fp);

	return S_OK;
}


HRESULT CMapTool::Get_Assets()
{
#pragma message ("스태틱에 있는거 몽땅 가져오는거 뭔가 별론데 방법 없나")
	const PROTOTYPES map_Prototypes = m_pGameInstance->Get_Prototypes(ENUM_CLASS(LEVEL::STATIC));
	m_AssetNames.reserve(map_Prototypes.size());
	wstring wstrGameObject = L"GameObject";
	for (auto& prototype : map_Prototypes)
	{
		if(prototype.first.find(wstrGameObject) != wstring::npos)
			m_AssetNames.push_back(prototype.first);
	}

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
	Safe_Release(m_pTerrain);

	__super::Free();
}
