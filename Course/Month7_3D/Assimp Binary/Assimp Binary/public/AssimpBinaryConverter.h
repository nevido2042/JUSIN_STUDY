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
	/* �𵨿� ���� ��� ������ ��� �ִ� ����ü. */
	const aiScene* m_pAIScene = { nullptr };
};

