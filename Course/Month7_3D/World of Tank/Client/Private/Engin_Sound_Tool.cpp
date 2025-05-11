#include "Engin_Sound_Tool.h"
#include "GameInstance.h"

CEngin_Sound_Tool::CEngin_Sound_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CTool(pDevice, pContext)
{

}

HRESULT CEngin_Sound_Tool::Initialize()
{

	return S_OK;
}

void CEngin_Sound_Tool::Priority_Update(_float fTimeDelta)
{

}

void CEngin_Sound_Tool::Update(_float fTimeDelta)
{

}

void CEngin_Sound_Tool::Late_Update(_float fTimeDelta)
{

}

HRESULT CEngin_Sound_Tool::Render()
{

	return S_OK;
}

CEngin_Sound_Tool* CEngin_Sound_Tool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEngin_Sound_Tool* pInstance = new CEngin_Sound_Tool(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CEngin_Sound_Tool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEngin_Sound_Tool::Free()
{
	__super::Free();

}
