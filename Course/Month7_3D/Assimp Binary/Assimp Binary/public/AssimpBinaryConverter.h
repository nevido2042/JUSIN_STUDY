#pragma once
#include "AssimpBinary_Defines.h"

class CAssimpBinaryConverter
{
public:
	HRESULT Initialize(const _char* pModelFilePath);
	HRESULT Initialize(const wstring& wstrModelFilePath);
private:
	HRESULT Export();

private:
	string m_ModelFilePath = {};

private:
	Assimp::Importer		m_Importer;
	/* 모델에 대한 모든 정보를 담고 있는 구조체. */
	const aiScene* m_pAIScene = { nullptr };
};

