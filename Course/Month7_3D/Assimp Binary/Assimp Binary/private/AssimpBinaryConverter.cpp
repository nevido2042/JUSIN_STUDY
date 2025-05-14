#include "AssimpBinaryConverter.h"

HRESULT CAssimpBinaryConverter::Initialize(const _char* pModelFilePath)
{
	_uint iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;

	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
	
	return S_OK;

}
