#pragma once
#include "AssimpBinary_Defines.h"
#include <windows.h>

#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

class CAssimpBinaryConverter
{
public:
	HRESULT Initialize(const _char* pModelFilePath);

private:
	Assimp::Importer		m_Importer;
	/* �𵨿� ���� ��� ������ ��� �ִ� ����ü. */
	const aiScene* m_pAIScene = { nullptr };
};

