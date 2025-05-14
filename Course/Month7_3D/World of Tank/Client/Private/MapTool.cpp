#include "MapTool.h"
#include "GameInstance.h"

#include "Terrain.h"
#include "Layer.h"

#include "MapObject.h"

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
			if (pLayer.first.find(L"MapObject") == wstring::npos)
				continue;

			// �� �̸����� �׷�ȭ
			map<string, vector<CGameObject*>> ModelGroups;

			for (auto pGameObject : pLayer.second->Get_GameObjects())
			{
				wstring prefix = L"Prototype_Component_Model_";
				wstring wstrModelCom = static_cast<CMapObject*>(pGameObject)->Get_ModelCom();

				size_t PosPrefix = wstrModelCom.find(prefix);

				if (PosPrefix == wstring::npos)
					continue;

				string ModelName = WStringToString(wstrModelCom.substr(PosPrefix + prefix.length()));
				ModelGroups[ModelName].push_back(pGameObject);
			}

			// �� �̸��� TreeNode
			for (auto& group : ModelGroups)
			{
				const string& ModelName = group.first;

				if (ImGui::TreeNode(ModelName.c_str()))
				{
					_uint j = 0;
					for (auto pGameObject : group.second)
					{
						string strHierarchyName = ModelName + '_' + to_string(j);
						++j;

						const _bool isSelected = (m_iSelectedHierarchyIndex == i);

						HierarchyNames.push_back(strHierarchyName);

						if (ImGui::Selectable(strHierarchyName.c_str(), isSelected))
						{
							m_iSelectedHierarchyIndex = i;
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();

						++i;
					}

					ImGui::TreePop(); // ModelName �ݱ�
				}
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
		Save_Map(HierarchyNames);
	}
	if (ImGui::Button("Load Map"))
	{
		//������Ÿ�� ��¼�� �� �����ϰ�
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
			CMapObject::MAPOBJECT_DESC Desc = {};
			Desc.wstrModelCom = m_AssetNames[m_iSelectedAssetIndex]; //Prototype_Component_Model_Fury

			wstring PreFix = L"Prototype_Component_Model";
			wstring Name = Desc.wstrModelCom.substr(PreFix.length());

			//Layer_MapObject_Fury
			m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_MapObject"), ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_MapObject") + Name, &Desc);
		}
	}

	if (ImGui::BeginListBox("##AssetList", ImVec2(-FLT_MIN, 200)))
	{
		for (_int i = 0; i < m_AssetNames.size(); ++i)
		{
			string strAssetName;
			strAssetName.resize(MAX_PATH);  // ���� Ȯ��
			size_t convertedChars = 0;
			wcstombs_s(&convertedChars, &strAssetName[0], MAX_PATH, m_AssetNames[i].c_str(), _TRUNCATE);
			strAssetName.resize(convertedChars > 0 ? convertedChars - 1 : 0);  // �� ���� �����ϰ� �ڸ���

			const _bool isSelected = (m_iSelectedAssetIndex == i);
			if (ImGui::Selectable(strAssetName.c_str(), isSelected))
			{
				m_iSelectedAssetIndex = i;
			}

			// ���õ� �׸� ��Ŀ��
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}

	ImGui::End();
#pragma endregion

#pragma region Detail

	ImGui::Begin("Detail", nullptr);


	// ��ŷ�� ��ġ ǥ��
	ImGui::Text("Terrain_PickedPos(L-Ctrl: Set Pos) ");
	_float3 vPos = m_pTerrain->Get_PickedPos();
	ImGui::Text("x:%.2f, y:%.2f, z:%.2f", vPos.x, vPos.y, vPos.z);

	ImGui::Separator();

	// ���õ� GameObject ��������
	ImGui::Text("Position");
	CGameObject* pGameObject = Get_Selected_GameObject(HierarchyNames);

	if (pGameObject != nullptr)
	{
		// Transform Component���� ���� ��� ���
		CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(g_strTransformTag));

		if(m_pGameInstance->Key_Pressing(DIK_LCONTROL) && m_pGameInstance->Mouse_Down(ENUM_CLASS(DIMK::LBUTTON)))
			pTransform->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&vPos), 1.f));
#pragma region Guizmo
		_float4x4 WorldMatrix;
		XMStoreFloat4x4(&WorldMatrix, pTransform->Get_WorldMatrix()); // <- XMMATRIX���� ��ȯ�� ����

		// ����� float �迭�� ��ȯ (ImGuizmo�� float[16] �迭 ��)
		_float matrix[16];
		memcpy(matrix, &WorldMatrix, sizeof(_float) * 16);

		// View / Projection ��Ʈ���� (ī�޶� ����)
		_matrix View = m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW);
		_matrix Projection = m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ);

		// View / Projection�� float[16] �迭�� ��ȯ
		_float viewMatrix[16];
		_float projMatrix[16];
		XMStoreFloat4x4(reinterpret_cast<_float4x4*>(viewMatrix), View);
		XMStoreFloat4x4(reinterpret_cast<_float4x4*>(projMatrix), Projection);

		// ImGuizmo ���� (��ü ȭ�� �������� ���)
		ImGuizmo::BeginFrame();
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());
		ImVec2 displaySize = ImGui::GetIO().DisplaySize;
		ImGuizmo::SetRect(0, 0, displaySize.x, displaySize.y);

		// Guizmo ���� (Translate / Rotate / Scale)
		ImGuizmo::Manipulate(viewMatrix, projMatrix, ImGuizmo::TRANSLATE, ImGuizmo::LOCAL, matrix);

		// Guizmo ������̸� Transform �ݿ�
		if (ImGuizmo::IsUsing())
		{
			memcpy(&WorldMatrix, matrix, sizeof(_float) * 16);
			pTransform->Set_WorldMatrix(WorldMatrix);
		}
#pragma endregion
#pragma region Input Position
		// ���� Position �� ���
		XMStoreFloat3(&vPos, pTransform->Get_State(STATE::POSITION));

		//Position�� ���� �Է¹ޱ� (InputFloat3)
		if (ImGui::InputFloat3("", reinterpret_cast<_float*>(&vPos), "%.2f"))
		{
			// �Էµ� ������ Transform ������ �ٽ� ����
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

	// ������ ó��
	ImGui::Render();  // ������ ó��
}

HRESULT CMapTool::Render()
{


	return S_OK;
}

CGameObject* CMapTool::Get_Selected_GameObject(vector<string>& HierarchyNames)
{
	if (m_iSelectedHierarchyIndex < HierarchyNames.size())
	{
		return Get_GameObject(HierarchyNames[m_iSelectedHierarchyIndex]);
	}

	return nullptr;
}

CGameObject* CMapTool::Get_GameObject(string HierarchyName)
{
	// ��: "Fury_0"

	size_t UnderbarPos = HierarchyName.find('_');
	if (UnderbarPos == string::npos)
		return nullptr; // ����� ������ �߸��� �̸�

	// �� �̸� : Fury
	string ModelName = HierarchyName.substr(0, UnderbarPos);

	// �ѹ� : 0
	string ObjectNumberStr = HierarchyName.substr(UnderbarPos + 1);

	if (ObjectNumberStr.empty())
		return nullptr; // �ѹ��� ��������� ����

	_int iObjectNumber = stoi(ObjectNumberStr);

	// ���� ���̾� �̸� : Layer_MapObject_Fury
	string LayerName = "Layer_MapObject_" + ModelName;

	return m_pGameInstance->Get_GameObject(
		ENUM_CLASS(LEVEL::MAPTOOL),
		wstring(LayerName.begin(), LayerName.end()),
		iObjectNumber
	);

	return nullptr;
}


HRESULT CMapTool::Delete_All()
{
	const wstring wstrPrefix = L"Layer_MapObject";

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


HRESULT CMapTool::Save_Map(vector<string>& HierarchyNames)
{
	// ���� ������ ���� (���ص� �ȴٸ� ���� ����)
	if (!filesystem::exists("../Bin/Map/"))
		filesystem::create_directories("../Bin/Map/");

	filesystem::path MapPath = "../Bin/Map/Map.bin";

	FILE* fp = nullptr;
	fopen_s(&fp, MapPath.string().c_str(), "wb");
	if (!fp)
		return E_FAIL;

	for (string HierarchyName : HierarchyNames)
	{
		//Fury_0
		CGameObject* pGameObject = Get_GameObject(HierarchyName);

		// Transform ��ġ ��������
		CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(g_strTransformTag));
		_float3 vPos = {};
		if (pTransform != nullptr)
		{
			XMStoreFloat3(&vPos, pTransform->Get_State(STATE::POSITION));
		}

		size_t UnderbarPos = HierarchyName.find('_');

		// �� �̸� : Fury
		string ModelName = HierarchyName.substr(0, UnderbarPos);

		// �̸� ���� ���� ����
		_uint iNameLength = static_cast<_uint>(ModelName.length());
		fwrite(&iNameLength, sizeof(_uint), 1, fp);

		// �̸� ������ ����
		fwrite(ModelName.c_str(), sizeof(char), iNameLength, fp);

		// ��ġ ������ ����
		fwrite(&vPos, sizeof(_float3), 1, fp);
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

		// �� �̻� ������ ������ ����
		if (readCount != 1)
			break;

		// �̸� �б�
		string Name;
		Name.resize(iNameLength);
		fread(&Name[0], sizeof(char), iNameLength, fp);

		// ��ġ �б�
		_float3 vPos = {};
		fread(&vPos, sizeof(_float3), 1, fp);

		//"Layer_MapObject_Fury"
		wstring LayerName = L"Layer_MapObject_" + wstring(Name.begin(), Name.end());
		//"Prototype_Component_Model_Fury"
		wstring ModelComName = L"Prototype_Component_Model_" + wstring(Name.begin(), Name.end());

		// GameObject ����
		CMapObject::MAPOBJECT_DESC Desc = {};
		Desc.wstrModelCom = ModelComName;

		m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_MapObject"), ENUM_CLASS(LEVEL::MAPTOOL), LayerName, &Desc);
		CGameObject* pGameObject = m_pGameInstance->Get_Last_GameObject(ENUM_CLASS(LEVEL::MAPTOOL), LayerName);
		if (pGameObject == nullptr)
			continue;

		// Transform ��ġ ����
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
#pragma message ("�� ������Ʈ�� �����´�")
	const PROTOTYPES map_Prototypes = m_pGameInstance->Get_Prototypes(ENUM_CLASS(LEVEL::STATIC));
	m_AssetNames.reserve(map_Prototypes.size());
	wstring wstrGameObject = L"Model";
	for (auto& prototype : map_Prototypes)
	{
		if(prototype.first.find(wstrGameObject) != wstring::npos)
			m_AssetNames.push_back(prototype.first);
	}

	return S_OK;
}

HRESULT CMapTool::Ready_Components()
{
	// �ʿ��� ������Ʈ�� �غ�
	return S_OK;
}

string CMapTool::WStringToString(const wstring& wstr)
{
	string str;
	size_t convertedChars = 0;
	str.resize(MAX_PATH);
	wcstombs_s(&convertedChars, &str[0], MAX_PATH, wstr.c_str(), _TRUNCATE);
	str.resize(convertedChars > 0 ? convertedChars - 1 : 0);
	return str;
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
