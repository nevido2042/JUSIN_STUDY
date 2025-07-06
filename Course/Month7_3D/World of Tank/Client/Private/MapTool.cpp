#include "MapTool.h"
#include "GameInstance.h"

#include "Terrain.h"
#include "Layer.h"

#include "MapObject.h"
#include "VIBuffer_Point_Instance.h"

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

	CGameObject* pMapVegetation = m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_MapVegetation"), 0);
	if (pMapVegetation == nullptr)
		return E_FAIL;

	m_pVegeVIBuffer = static_cast<CVIBuffer_Point_Instance*>(pMapVegetation->Get_Component(TEXT("Com_VIBuffer")));
	if (nullptr == m_pVegeVIBuffer)
		return E_FAIL;
	Safe_AddRef(m_pVegeVIBuffer);

	if (FAILED(Load_Map()))
		return E_FAIL;

	if (FAILED(Load_BoundaryPoints()))
		return E_FAIL;

	Load_Vegetation();

#pragma region ���ѱ��� ����
	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
	m_pEffect = new BasicEffect(m_pDevice);

	const void* pShaderByteCode = { nullptr };
	size_t		iShaderByteCodeLength = {  };

	m_pEffect->SetVertexColorEnabled(true);

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount,
		pShaderByteCode, iShaderByteCodeLength, &m_pInputLayout)))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

void CMapTool::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	if (m_pGameInstance->Key_Down(DIK_E))
		m_currentOperation = ImGuizmo::ROTATE;
	else if (m_pGameInstance->Key_Down(DIK_R))
		m_currentOperation = ImGuizmo::SCALE;
	else if (m_pGameInstance->Key_Down(DIK_T))
		m_currentOperation = ImGuizmo::TRANSLATE;
	else if (m_pGameInstance->Key_Down(DIK_ESCAPE))
		m_iSelectedHierarchyIndex = -1;
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

				// �ϴ� TreeNode ���� Ȯ��
				_bool bOpen = ImGui::TreeNodeEx(ModelName.c_str(), ImGuiTreeNodeFlags_DefaultOpen);

				_uint j = 0;
				for (auto pGameObject : group.second)
				{
					string strHierarchyName = ModelName + '_' + to_string(j);
					++j;

					// **�׻� HierarchyNames�� ä���ش� (�׸� ���ο� �����ϰ�)**
					HierarchyNames.push_back(strHierarchyName);

					const _bool isSelected = (m_iSelectedHierarchyIndex == i);

					if (bOpen)
					{
						// ������ ���� Selectable ���
						if (ImGui::Selectable(strHierarchyName.c_str(), isSelected))
						{
							m_iSelectedHierarchyIndex = i;
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					++i;
				}

				if (bOpen)
					ImGui::TreePop(); // ModelName �ݱ�
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

	// ���� ���� Dummy�� ä���� Load Map ��ư�� �Ʒ��� �б�
	ImVec2 available = ImGui::GetContentRegionAvail();
	ImGui::Dummy(ImVec2(0, available.y - 30)); // 30�� ��ư ���� ������ (������ ����)

	if (ImGui::Button("Load Map"))
	{
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
	ImGui::Text("Transform");
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

		// Guizmo ���� (Translate / Rotate / Scale �� ���õ� �͸� ����)
		ImGuizmo::Manipulate(viewMatrix, projMatrix, m_currentOperation, ImGuizmo::LOCAL, matrix);

		// Guizmo ������̸� Transform �ݿ�
		if (ImGuizmo::IsUsing())
		{
			memcpy(&WorldMatrix, matrix, sizeof(_float) * 16);
			pTransform->Set_WorldMatrix(WorldMatrix);
		}
#pragma endregion


#pragma region Input Position
		XMStoreFloat3(&vPos, pTransform->Get_State(STATE::POSITION));
		if (ImGui::InputFloat3("##Position", reinterpret_cast<_float*>(&vPos), "%.2f"))
		{
			pTransform->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&vPos), 1.f));
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Position", m_currentOperation == ImGuizmo::TRANSLATE)) m_currentOperation = ImGuizmo::TRANSLATE;

#pragma endregion

#pragma region Input Rotation
		_float3 vRot = pTransform->Get_RotationEuler();
		if (ImGui::InputFloat3("##Rotation", reinterpret_cast<_float*>(&vRot), "%.2f"))
		{
#pragma message("���� �Է����� �� �ٸ� ����� 0���� �ʱ�ȭ �Ǵ� ���� ����")
			_vector vRotation = XMVectorSet(XMConvertToRadians(vRot.x), XMConvertToRadians(vRot.y), XMConvertToRadians(vRot.z), 0.f);
			pTransform->Set_RotationEuler(vRotation);
			//pTransform->Rotation(vRot.x, vRot.y, vRot.z);
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Rotation", m_currentOperation == ImGuizmo::ROTATE)) m_currentOperation = ImGuizmo::ROTATE;

#pragma endregion

#pragma region Input Scale
		_float3 vScale = pTransform->Get_Scaled();
		if (ImGui::InputFloat3("##Scale", reinterpret_cast<_float*>(&vScale), "%.2f"))
		{
			pTransform->Scaling(vScale);
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Scale", m_currentOperation == ImGuizmo::SCALE)) m_currentOperation = ImGuizmo::SCALE;

#pragma endregion
	}
	ImGui::End();


#pragma endregion

	ImGui::ShowDemoWindow();

#pragma region ���� ���� ����
	if (m_pGameInstance->Key_Pressing(DIK_B))
	{
		if (m_pGameInstance->Mouse_Down(ENUM_CLASS(DIMK::LBUTTON)))
		{
			_float3 vPos = m_pTerrain->Get_PickedPos();

			//// Y���� ���� ���� �� ���� ���� ������ ��ü
			//if (!m_BoundaryPoints.empty())
			//{
			//	_float fMaxY = vPos.y; // �⺻�� ���� Y
			//	for (const _float3& _vPos : m_BoundaryPoints)
			//	{
			//		fMaxY = max(fMaxY, _vPos.y);
			//	}
			//	vPos.y = fMaxY;
			//}

			m_BoundaryPoints.push_back(vPos);
		}
		else if (m_pGameInstance->Mouse_Down(ENUM_CLASS(DIMK::RBUTTON)))
		{
			if (m_BoundaryPoints.empty())
				return;

			m_BoundaryPoints.pop_back();
		}
		else if (m_pGameInstance->Key_Down(DIK_S))
		{
			if(FAILED(Save_BoundaryPoints()))
				cout << "Save_BoundaryPoints Fail !!!!!!!!!!!!" << endl;
			else
				cout << "Save_BoundaryPoints Success !" << endl;
		}
	}

#pragma endregion

#pragma region �Ļ� ����
	Control_Vegetation();
#pragma endregion

}

void CMapTool::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	//m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);

	//// ������ ó��
	//ImGui::Render();  // ������ ó��
}

HRESULT CMapTool::Render()
{
	__super::Render();

	m_pEffect->SetWorld(XMMatrixIdentity());
	m_pEffect->SetView(m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW));
	m_pEffect->SetProjection(m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ));

	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pEffect->Apply(m_pContext);

	m_pBatch->Begin();

	Draw_Boundary();

	m_pBatch->End();

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
	// ���� ������ ����
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
		_float4x4 WorldMatrix = {};
		if (pTransform != nullptr)
		{
			XMStoreFloat4x4(&WorldMatrix, pTransform->Get_WorldMatrix());
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
		fwrite(&WorldMatrix, sizeof(_float4x4), 1, fp);
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
		return S_OK;

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
		_float4x4 mWorldMatrix = {};
		fread(&mWorldMatrix, sizeof(_float4x4), 1, fp);

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
			pTransform->Set_WorldMatrix(mWorldMatrix);
		}
	}

	fclose(fp);

	return S_OK;
}

HRESULT CMapTool::Save_BoundaryPoints()
{
	// ���� ������ ����
	if (!filesystem::exists("../Bin/Map/"))
		filesystem::create_directories("../Bin/Map/");

	filesystem::path Path = "../Bin/Map/BoundaryPoints.bin";

	FILE* fp = nullptr;
	fopen_s(&fp, Path.string().c_str(), "wb");
	if (!fp)
		return E_FAIL;

	for (_float3& vPos : m_BoundaryPoints)
	{
		fwrite(&vPos, sizeof(_float3), 1, fp);
	}

	fclose(fp);

	return S_OK;
}

HRESULT CMapTool::Load_BoundaryPoints()
{
	m_BoundaryPoints.clear();

	filesystem::path Path = "../Bin/Map/BoundaryPoints.bin";

	FILE* fp = nullptr;
	fopen_s(&fp, Path.string().c_str(), "rb");
	if (!fp)
		return S_OK;

	while (true)
	{

		_float3 vPos = {};
		size_t readCount = fread(&vPos, sizeof(_float3), 1, fp);

		// �� �̻� ������ ������ ����
		if (readCount != 1)
			break;

		m_BoundaryPoints.push_back(vPos);

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

void CMapTool::Draw_Boundary()
{
	if (m_BoundaryPoints.size() < 2)
		return;

	_float3 vPointA = {};
	_float3 vPointB = {};

	for (_uint i = 0; i < m_BoundaryPoints.size(); ++i)
	{
		if (i == m_BoundaryPoints.size() - 1)
		{
			vPointA = m_BoundaryPoints.at(i);
			vPointB = m_BoundaryPoints.at(0);
		}
		else
		{
			vPointA = m_BoundaryPoints.at(i);
			vPointB = m_BoundaryPoints.at(i + 1);
		}

		DX::DrawRay(
			m_pBatch,
			XMVectorSetW(XMLoadFloat3(&vPointA), 1.f),     // ������ A
			XMVectorSubtract(XMLoadFloat3(&vPointB), XMLoadFloat3(&vPointA)), // ���� ���� (B - A)
			false,
			DirectX::Colors::Red
		);

	}
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

void CMapTool::Control_Vegetation()
{
	if (m_pGameInstance->Mouse_Down(ENUM_CLASS(DIMK::LBUTTON)) && m_pGameInstance->Key_Pressing(DIK_V))
	{
		_float3 vPos = m_pTerrain->Get_PickedPos();
		vPos.y += 1.5f;

		m_VegeTranslates.push_back(vPos);
		m_pVegeVIBuffer->Change_NumInstance(static_cast<_uint>(m_VegeTranslates.size()));
		m_pVegeVIBuffer->Change_Translation(m_VegeTranslates);
		m_pVegeVIBuffer->Change_Size_XY(_float2(209.f * 0.01f, 345.f * 0.01f));
	}

	if (m_pGameInstance->Key_Down(DIK_S) && m_pGameInstance->Key_Pressing(DIK_V))
	{
		Save_Vegetation();
	}
}

void CMapTool::Save_Vegetation()
{
	// ���� ������ ����
	if (!filesystem::exists("../Bin/Map/"))
		filesystem::create_directories("../Bin/Map/");

	filesystem::path Path = "../Bin/Map/VegeTranslates.bin";

	FILE* fp = nullptr;
	fopen_s(&fp, Path.string().c_str(), "wb");
	if (!fp)
	{
		cout << "Fail!!!!!!    Save_Vegetation" << endl;
		return;
	}

	size_t Count = { m_VegeTranslates.size() };
	fwrite(&Count, sizeof(size_t), 1, fp);

	for (_float3& vPos : m_VegeTranslates)
	{
		fwrite(&vPos, sizeof(_float3), 1, fp);
	}

	fclose(fp);

	cout << "Save_Vegetation" << endl;
}

void CMapTool::Load_Vegetation()
{
	m_VegeTranslates.clear();

	filesystem::path Path = "../Bin/Map/VegeTranslates.bin";

	FILE* fp = nullptr;
	fopen_s(&fp, Path.string().c_str(), "rb");
	if (!fp)
	{
		cout << "Fail!!!!!! Load_Vegetation" << endl;
		return;
	}

	size_t Count = { 0 };
	fread(&Count, sizeof(size_t), 1, fp);

	m_VegeTranslates.reserve(Count);


	while (true)
	{
		_float3 vPos = {};
		size_t readCount = fread(&vPos, sizeof(_float3), 1, fp);

		// �� �̻� ������ ������ ����
		if (readCount != 1)
			break;

		m_VegeTranslates.push_back(vPos);
	}

	m_pVegeVIBuffer->Change_NumInstance(static_cast<_uint>(m_VegeTranslates.size()));
	m_pVegeVIBuffer->Change_Translation(m_VegeTranslates);
	m_pVegeVIBuffer->Change_Size_XY(_float2(209.f * 0.01f, 345.f * 0.01f));

	fclose(fp);

	cout << "(Load)Vege Count:" << m_VegeTranslates.size() << endl;
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
	__super::Free();

	Safe_Release(m_pVegeVIBuffer);

	Safe_Release(m_pTerrain);

	Safe_Release(m_pInputLayout);
	Safe_Delete(m_pBatch);
	Safe_Delete(m_pEffect);

}
