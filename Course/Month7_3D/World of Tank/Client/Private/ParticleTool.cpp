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

		m_pSmoke = static_cast<CBaseParticle*>(pEffect);
		Safe_AddRef(m_pSmoke);
	}

	return S_OK;
}
void CParticleTool::Priority_Update(_float fTimeDelta)
{

}


void CParticleTool::Update(_float fTimeDelta)
{
	ImGui::Begin("Particle");

#pragma region ��ƼŬ �ؽ���
	if (ImGui::Button("Change Texture")) 
	{
		wstring pTextureFilePath = OpenFileDialog();

		if (!pTextureFilePath.empty())
		{
			//�ӽ÷� ����� ������Ÿ�� �̸� ����
			wstring strPrototypeTag = L"CParticleTool_Temp_" + to_wstring(m_strPrototypeTag_ID);

			//�ؽ��ĸ� �ε��ϰ�... �ε��� �Ϸ�Ǹ� �ؽ��� �ٲ� ��������.
			if (FAILED(Load_Texture(strPrototypeTag, pTextureFilePath)))
			{
				//�̹� ���� ���� ����
				//MSG_BOX("�̹��� �ε� ����");
			}

			if (FAILED(Change_Texture(strPrototypeTag)))
			{
				MSG_BOX("�ؽ��� ���� ����");
			}
			else
			{
				++m_strPrototypeTag_ID;
			}
				
		}
	}

#pragma endregion

	ImGui::Separator(); // ���м� �߰�

#pragma region ��ƼŬ ����
	const _int iMinCount = 1;
	const _int iMaxCount = 10000;

	// �ؽ�Ʈ �Է� ��
	if (ImGui::InputInt("Instance Count", &m_iNumInstances))
	{
		m_iNumInstances = clamp(m_iNumInstances, iMinCount, iMaxCount);
		m_VIBuffer->Change_NumInstance(m_iNumInstances);
		m_VIBuffer->Replay();
	}

	// �����̴�
	if (ImGui::SliderInt("##Slider", &m_iNumInstances, iMinCount, iMaxCount))
	{
		m_VIBuffer->Change_NumInstance(m_iNumInstances);
		m_VIBuffer->Replay();
	}

	// ���� ���
	ImGui::Text("Current: %d / Max: %d", m_iNumInstances, iMaxCount);
#pragma endregion

	ImGui::Separator(); // ���м� �߰�

#pragma region ���� ����
	if (ImGui::DragFloat3("Spawn Range", reinterpret_cast<_float*>(&m_vRange), 0.1f, 0.f, 1000.f))
	{
		m_VIBuffer->Change_Range(m_vRange);
		m_VIBuffer->Replay(); 
	}
#pragma endregion

	ImGui::Separator(); // ���м� �߰�

#pragma region ���� ���

	const _char* szShapes[] = { "DROP", "SPREAD", "END" };

	_int iCurrent = static_cast<_int>(m_eEmissionShape);
	if (ImGui::Combo("Emission Shape", &iCurrent, szShapes, IM_ARRAYSIZE(szShapes)))
	{
		m_eEmissionShape = static_cast<EMISSION_SHAPE>(iCurrent);

		// ������ �ٲ���� �� ó��
		m_VIBuffer->Set_EmissionShape(m_eEmissionShape);
		m_VIBuffer->Replay(); 
	}
#pragma endregion

	ImGui::Separator(); // ���м� �߰�

#pragma region ���� �ǹ�
	if (ImGui::DragFloat3("Emission Pivot", reinterpret_cast<_float*>(&m_vPivot), 0.1f, -100.f, 100.f))
	{
		m_VIBuffer->Change_Pivot(m_vPivot);
		m_VIBuffer->Replay(); 
	}
#pragma endregion

	ImGui::Separator(); // ���м� �߰�

#pragma region ��ƼŬ ������
	if (ImGui::DragFloat2("Particle Size", reinterpret_cast<_float*>(&m_vSize), 0.001f, 0.001f, 10.f))
	{
		m_VIBuffer->Change_Size(m_vSize);
		m_VIBuffer->Replay();
	}
#pragma endregion

	ImGui::Separator(); // ���м� �߰�

#pragma region ������ Ÿ��
	if (ImGui::DragFloat2("Life Time", reinterpret_cast<_float*>(&m_vLifeTime), 0.1f, 0.f, 100.f))
	{
		m_VIBuffer->Change_LifeTime(m_vLifeTime);
		m_VIBuffer->Replay(); 
	}
#pragma endregion

	ImGui::Separator(); // ���м� �߰�

#pragma region ��ƼŬ �ӵ�
	if (ImGui::DragFloat2("Particle Speed", reinterpret_cast<_float*>(&m_vSpeed), 0.1f, -100.f, 100.f))
	{
		m_VIBuffer->Change_Speed(m_vSpeed);
		m_VIBuffer->Replay(); 
	}
#pragma endregion

	ImGui::Separator(); // ���м� �߰�

#pragma region ����

	if (ImGui::Checkbox("Loop", &m_bLoop))
	{
		m_VIBuffer->Change_isLoop(m_bLoop);  // ���� ���� ����
		m_VIBuffer->Replay();            
	}

#pragma endregion

	ImGui::Separator(); // ���м� �߰�

#pragma region ���÷���
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
	ImGui::Render();  // ������ ó��
	return S_OK;
}

#include <windows.h>
#include <commdlg.h>  // GetOpenFileName
wstring CParticleTool::OpenFileDialog()
{
	TCHAR szInitialDir[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, szInitialDir);  // ���� �۾� ���͸� ����

	OPENFILENAME ofn;       // common dialog box structure
	_tchar szFile[MAX_PATH] = { 0 };       // buffer for file name

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;  // �ʿ��ϸ� ������ �ڵ� �ֱ�
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Image Files\0*.png;*.jpg;*.dds;*.tga\0All Files\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;



	if (GetOpenFileName(&ofn) == TRUE) 
	{
		// ���� ���͸��� ����
		SetCurrentDirectory(szInitialDir);
		return _wstring(ofn.lpstrFile);
	}

	// ���� ���͸��� ����
	SetCurrentDirectory(szInitialDir);
	return wstring(); // ��� �� ���� �� �� ���ڿ� ��ȯ
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
	CBase* pTexture = m_pGameInstance->Find_Prototype(ENUM_CLASS(LEVEL::PARTICLETOOL), strPrototypeTag);

	if(pTexture)
		return m_pSmoke->Change_Texture(ENUM_CLASS(LEVEL::PARTICLETOOL), strPrototypeTag);
	else
		return S_OK;

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
	Safe_Release(m_pSmoke);
}