#include "MapTool.h"
#include "GameInstance.h"

#include "Terrain.h"

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

	if (FAILED(Get_Asset()))
		return E_FAIL;

	m_pTerrain = static_cast<CTerrain*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_Terrain"), 0));
	if (nullptr == m_pTerrain)
		return E_FAIL;
	Safe_AddRef(m_pTerrain);

	return S_OK;
}

void CMapTool::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CMapTool::Update(_float fTimeDelta)
{
	//���̾��Ű â
	ImGui::Begin("Hierarchy", nullptr);

	ImGui::End();

#pragma region Asset
	ImGui::Begin("Asset", nullptr);

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

	//Detail â
	ImGui::Begin("Detail", nullptr);
	ImGui::Text("Terrain_PickedPos");

	_float3 vPos = m_pTerrain->Get_PickedPos();
	ImGui::Text("x:%.2f, y:%.2f, z:%.2f", vPos.x, vPos.y, vPos.z);
	ImGui::End();

	ImGui::ShowDemoWindow();

#pragma region �����
	// viewMatrix = ī�޶� View ���
	// projMatrix = ī�޶� Projection ���
	// objectMatrix = ���� ��� ���
	ImGuizmo::SetDrawlist();

	// ������ ��ġ �� ũ�� ���� (�ɼ�)
	ImGuizmo::SetRect(0, 0, (float)g_iWinSizeX, (float)g_iWinSizeY);

	// 1. XMMATRIX�κ��� XMFLOAT4X4�� ��ȯ
	XMMATRIX viewMatrix = m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW);
	XMMATRIX projMatrix = m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ);
	XMMATRIX worldMatrix = XMMatrixInverse(nullptr, viewMatrix);

	// ImGuizmo�� float*�� �ޱ� ������ XMFLOAT4X4�� ��ȯ�ؾ� ��
	XMFLOAT4X4 viewMatrixF, projMatrixF, worldMatrixF;
	XMStoreFloat4x4(&viewMatrixF, XMMatrixTranspose(viewMatrix));
	XMStoreFloat4x4(&projMatrixF, XMMatrixTranspose(projMatrix));
	XMStoreFloat4x4(&worldMatrixF, XMMatrixTranspose(worldMatrix));

	// 2. ImGuizmo�� ����
	ImGuizmo::Manipulate(
		&viewMatrixF.m[0][0],
		&projMatrixF.m[0][0],
		ImGuizmo::TRANSLATE,   // �Ǵ� ROTATE / SCALE
		ImGuizmo::LOCAL,
		&worldMatrixF.m[0][0]
	);

	// 3. ����� �ٽ� XMMATRIX�� �������� (���� ����)
	worldMatrix = XMMatrixTranspose(XMLoadFloat4x4(&worldMatrixF));
#pragma endregion
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

HRESULT CMapTool::Get_Asset()
{
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
	// �ʿ��� ������Ʈ�� �غ�
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
