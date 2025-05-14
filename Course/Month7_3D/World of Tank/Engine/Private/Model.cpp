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

HRESULT CModel::Initialize_Prototype(MODEL eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
	m_ModelFilePath = pModelFilePath;  // 경로 저장

	XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);

	if (FAILED(Load_Binary_Model(eType, m_ModelFilePath.c_str())))
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

HRESULT CModel::Load_Binary_Model(MODEL eType, const _char* pModelFilePath)
{
	FILE* fp = nullptr;

	_char szDrive[MAX_PATH] = {};
	_char szDir[MAX_PATH] = {};
	_char szFileName[MAX_PATH] = {};
	_char szExt[MAX_PATH] = {};

	_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDir, MAX_PATH, szFileName, MAX_PATH, szExt, MAX_PATH);

	string modelDirPath = string(szDrive) + string(szDir);
	string binFilePath = modelDirPath + string(szFileName) + ".bin";

	fopen_s(&fp, binFilePath.c_str(), "rb");
	if (!fp)
		return E_FAIL;

	// 메쉬 개수 읽기
	fread(&m_iNumMeshes, sizeof(_uint), 1, fp);

	// 메테리얼 개수 읽기
	m_iNumMaterials = 0;
	fread(&m_iNumMaterials, sizeof(_uint), 1, fp);

	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		_uint vertexCount, indexCount;
		fread(&vertexCount, sizeof(_uint), 1, fp);
		fread(&indexCount, sizeof(_uint), 1, fp);

		MESH_DESC Desc;
		Desc.Positions.resize(vertexCount);
		Desc.Normals.resize(vertexCount);
		Desc.Tangents.resize(vertexCount);
		Desc.TexCoords.resize(vertexCount);
		Desc.Indices.resize(indexCount);

		fread(Desc.Positions.data(), sizeof(_float3), vertexCount, fp);
		fread(Desc.Normals.data(), sizeof(_float3), vertexCount, fp);
		fread(Desc.Tangents.data(), sizeof(_float3), vertexCount, fp);

		for (_uint v = 0; v < vertexCount; ++v)
		{
			fread(&Desc.TexCoords[v].x, sizeof(_float), 1, fp);
			fread(&Desc.TexCoords[v].y, sizeof(_float), 1, fp);
		}

		fread(Desc.Indices.data(), sizeof(_uint), indexCount, fp);

		//_uint materialIndex;
		fread(&Desc.MaterialIndex, sizeof(_uint), 1, fp);

		_char texPath[256] = {};
		fread(texPath, sizeof(_char), 256, fp);

		_char texDrive[MAX_PATH] = {};
		_char texDir[MAX_PATH] = {};
		_char texFileName[MAX_PATH] = {};
		_char texExt[MAX_PATH] = {};

		_splitpath_s(texPath, texDrive, MAX_PATH, texDir, MAX_PATH, texFileName, MAX_PATH, texExt, MAX_PATH);

		string texFilePath = modelDirPath + texFileName + texExt;

		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, Desc, eType, XMLoadFloat4x4(&m_PreTransformMatrix));
		if (nullptr == pMesh)
		{
			fclose(fp);
			return E_FAIL;
		}

		CMaterial* pMaterial = CMaterial::Create(m_pDevice, m_pContext, texFilePath.c_str());
		if (nullptr == pMaterial)
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


CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
	CModel* pInstance = new CModel(pDevice, pContext, pModelFilePath);

	if (FAILED(pInstance->Initialize_Prototype(eType, pModelFilePath, PreTransformMatrix)))
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
