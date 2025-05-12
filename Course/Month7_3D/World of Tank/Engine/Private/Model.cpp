#include "Component.h"

#include "Material.h"
#include "Model.h"
#include "Mesh.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath)
	: CComponent{ pDevice, pContext },
	m_ModelFilePath{ pModelFilePath }

{

}

CModel::CModel(const CModel& Prototype)
	: CComponent (Prototype)
	, m_iNumMeshes { Prototype.m_iNumMeshes }
	, m_Meshes { Prototype.m_Meshes }
	, m_iNumMaterials { Prototype.m_iNumMaterials }
	, m_Materials { Prototype.m_Materials }
	, m_ModelFilePath{ Prototype.m_ModelFilePath }
{
	for (auto& pMaterial : m_Materials)
		Safe_AddRef(pMaterial);

	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);

}

HRESULT CModel::Bind_Material(CShader* pShader, const _char* pConstantName, _uint iMeshIndex, aiTextureType eType, _uint iTextureIndex)
{
	if (iMeshIndex >= m_iNumMeshes)
		return E_FAIL;		

	_uint		iMaterialIndex = m_Meshes[iMeshIndex]->Get_MaterialIndex();

	if (iMaterialIndex >= m_iNumMaterials)
		return E_FAIL;

	return m_Materials[iMaterialIndex]->Bind_ShaderResource(pShader, pConstantName, eType, iTextureIndex);	
}

HRESULT CModel::Initialize_Prototype(const _char* pModelFilePath)
{
	m_ModelFilePath = pModelFilePath;  // 경로 저장

	if (IsBinaryModelAvailable())
	{
		if (FAILED(LoadBinaryModel(m_ModelFilePath.c_str())))
			return E_FAIL;
	}
	else
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CModel::Render(_uint iMeshIndex)
{	
	m_Meshes[iMeshIndex]->Bind_Buffers();
	m_Meshes[iMeshIndex]->Render();	

	return S_OK;
}

HRESULT CModel::LoadBinaryModel(const _char* pModelFilePath)
{
	FILE* fp = nullptr;

	char szDrive[MAX_PATH] = {};
	char szDir[MAX_PATH] = {};
	char szFileName[MAX_PATH] = {};
	char szExt[MAX_PATH] = {};

	_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDir, MAX_PATH, szFileName, MAX_PATH, szExt, MAX_PATH);

	std::string modelDirPath = std::string(szDrive) + std::string(szDir);
	std::string binFilePath = modelDirPath + std::string(szFileName) + ".bin";

	fopen_s(&fp, binFilePath.c_str(), "rb");
	if (!fp)
		return E_FAIL;

	// 메쉬 개수 읽기
	fread(&m_iNumMeshes, sizeof(uint32_t), 1, fp);

	// 메테리얼 개수 읽기
	m_iNumMaterials = 0;
	fread(&m_iNumMaterials, sizeof(uint32_t), 1, fp);

	for (uint32_t i = 0; i < m_iNumMeshes; ++i)
	{
		uint32_t vertexCount, indexCount;
		fread(&vertexCount, sizeof(uint32_t), 1, fp);
		fread(&indexCount, sizeof(uint32_t), 1, fp);

		std::vector<XMFLOAT3> positions(vertexCount);
		std::vector<XMFLOAT3> normals(vertexCount);
		std::vector<XMFLOAT3> tangents(vertexCount);
		std::vector<XMFLOAT2> uvs(vertexCount);
		std::vector<uint32_t> indices(indexCount);

		fread(positions.data(), sizeof(XMFLOAT3), vertexCount, fp);
		fread(normals.data(), sizeof(XMFLOAT3), vertexCount, fp);
		fread(tangents.data(), sizeof(XMFLOAT3), vertexCount, fp);

		for (uint32_t v = 0; v < vertexCount; ++v)
		{
			fread(&uvs[v].x, sizeof(float), 1, fp);
			fread(&uvs[v].y, sizeof(float), 1, fp);
		}

		fread(indices.data(), sizeof(uint32_t), indexCount, fp);

		uint32_t materialIndex;
		fread(&materialIndex, sizeof(uint32_t), 1, fp);

		char texPath[256] = {};
		fread(texPath, sizeof(char), 256, fp);

		char texDrive[MAX_PATH] = {};
		char texDir[MAX_PATH] = {};
		char texFileName[MAX_PATH] = {};
		char texExt[MAX_PATH] = {};

		_splitpath_s(texPath, texDrive, MAX_PATH, texDir, MAX_PATH, texFileName, MAX_PATH, texExt, MAX_PATH);

		std::string texFilePath = modelDirPath + texFileName + texExt;

		CMaterial* pMaterial = CMaterial::Create(m_pDevice, m_pContext, texFilePath.c_str());
		if (nullptr == pMaterial)
		{
			fclose(fp);
			return E_FAIL;
		}

		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, positions, normals, tangents, uvs, indices, materialIndex);
		if (nullptr == pMesh)
		{
			fclose(fp);
			return E_FAIL;
		}

		m_Meshes.push_back(pMesh);
		m_Materials.push_back(pMaterial);
	}

	fclose(fp);
	return S_OK;
}

_bool CModel::IsBinaryModelAvailable()
{
	// 파일 경로에서 .fbx 확장자를 제거하고 .bin을 추가하는 처리
	std::string binFilePath = m_ModelFilePath;

	// .fbx 확장자가 있는지 확인
	size_t pos = binFilePath.find(".fbx");
	if (pos != std::string::npos)
	{
		binFilePath = binFilePath.substr(0, pos);  // .fbx 부분을 제거
	}

	binFilePath += ".bin";  // .bin 확장자 추가

	std::ifstream file(binFilePath.c_str());
	return file.good();  // 파일이 존재하면 true 반환
}





CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath)
{
	CModel* pInstance = new CModel(pDevice, pContext, pModelFilePath);

	if (FAILED(pInstance->Initialize_Prototype(pModelFilePath)))
	{
		MSG_BOX("Failed to Created : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CModel::Clone(void* pArg)
{
	CModel* pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel::Free()
{
	__super::Free();

	for (auto& pMaterial : m_Materials)
		Safe_Release(pMaterial);

	for (auto& pMesh : m_Meshes)
		Safe_Release(pMesh);

	
	m_Meshes.clear();
}
