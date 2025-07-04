#include "AssimpBinaryConverter.h"

HRESULT CAssimpBinaryConverter::Initialize(const _char* pModelFilePath)
{
	m_ModelFilePath = pModelFilePath;

	_uint iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;

	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
	 
	if (nullptr == m_pAIScene)
		return E_FAIL;

	Export();

	return S_OK;

}

HRESULT CAssimpBinaryConverter::Initialize(const wstring& wstrModelFilePath)
{
	// wchar_t → multibyte 변환
	_char path[MAX_PATH] = {};
	wcstombs_s(nullptr, path, wstrModelFilePath.c_str(), MAX_PATH);

	// 기존 Initialize(char*) 재사용
	return Initialize(path);
}

HRESULT CAssimpBinaryConverter::Export()
{
	filesystem::path modelPath = m_ModelFilePath;
	modelPath.replace_extension(".bin");

	string exportPath = modelPath.string();

	FILE* fp = nullptr;
	fopen_s(&fp, exportPath.c_str(), "wb");
	if (!fp)
		return E_FAIL;

	// 메쉬 개수 저장
	_uint meshCount = m_pAIScene->mNumMeshes;
	fwrite(&meshCount, sizeof(_uint), 1, fp);

	// 메테리얼 개수 저장
	_uint materialCount = m_pAIScene->mNumMaterials;
	fwrite(&materialCount, sizeof(_uint), 1, fp);

	for (_uint i = 0; i < meshCount; ++i)
	{
		aiMesh* mesh = m_pAIScene->mMeshes[i];

		_uint vertexCount = mesh->mNumVertices;
		_uint indexCount = mesh->mNumFaces * 3;

		fwrite(&vertexCount, sizeof(_uint), 1, fp);
		fwrite(&indexCount, sizeof(_uint), 1, fp);

		fwrite(mesh->mVertices, sizeof(aiVector3D), vertexCount, fp);
		fwrite(mesh->mNormals, sizeof(aiVector3D), vertexCount, fp);

		if (mesh->HasTangentsAndBitangents())
		{
			fwrite(mesh->mTangents, sizeof(aiVector3D), vertexCount, fp);
		}
		else
		{
			aiVector3D zero = { 0.0f, 0.0f, 0.0f };
			for (_uint v = 0; v < vertexCount; ++v)
				fwrite(&zero, sizeof(aiVector3D), 1, fp);
		}

		if (mesh->HasTextureCoords(0))
		{
			for (_uint v = 0; v < vertexCount; ++v)
			{
				_float uv[2] = { mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y };
				fwrite(uv, sizeof(_float), 2, fp);
			}
		}
		else
		{
			_float zeroUV[2] = { 0.0f, 0.0f };
			for (_uint v = 0; v < vertexCount; ++v)
				fwrite(zeroUV, sizeof(_float), 2, fp);
		}

		for (_uint f = 0; f < mesh->mNumFaces; ++f)
		{
			fwrite(mesh->mFaces[f].mIndices, sizeof(_uint), 3, fp);
		}

		_uint materialIndex = mesh->mMaterialIndex;
		fwrite(&materialIndex, sizeof(_uint), 1, fp);

		_char texPath[256] = {};
		if (mesh->mMaterialIndex < m_pAIScene->mNumMaterials)
		{
			aiString path;
			aiMaterial* pMaterial = m_pAIScene->mMaterials[mesh->mMaterialIndex];

			// 파일에 저장할 구조체
			struct TextureInfo
			{
				_int type;           // aiTextureType
				_int index;          // 텍스처 번호
				_char path[256];     // 경로
			};

			// 예시: mesh별 텍스처 정보를 저장
			vector<TextureInfo> vecTexInfos;

			// 텍스처가 여러 개일 수 있으므로 반복
			for (_int type = 1; type < AI_TEXTURE_TYPE_MAX; ++type)
			{
				aiTextureType texType = static_cast<aiTextureType>(type);
				UINT textureCount = pMaterial->GetTextureCount(texType);

				for (UINT texIndex = 0; texIndex < textureCount; ++texIndex)
				{
					aiString path;
					if (pMaterial->GetTexture(texType, texIndex, &path) == AI_SUCCESS)
					{
						TextureInfo info = {};
						info.type = texType;
						info.index = texIndex;
						strncpy_s(info.path, path.C_Str(), 255);

						vecTexInfos.push_back(info);
					}
				}
			}

			// 1. 개수 먼저 저장
			_uint iTexCount = static_cast<_uint>(vecTexInfos.size());
			fwrite(&iTexCount, sizeof(_uint), 1, fp);

			// 2. 각각의 TextureInfo 저장
			for (const auto& tex : vecTexInfos)
			{
				fwrite(&tex, sizeof(TextureInfo), 1, fp);
			}

		}
	}

	fclose(fp);

	return S_OK;
}
