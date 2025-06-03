#include "Module.h"

#include "GameInstance.h"

CModule::CModule(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CModule::CModule(const CModule& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CModule::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CModule::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}



void CModule::Priority_Update(_float fTimeDelta)
{


}

void CModule::Update(_float fTimeDelta)
{

}

void CModule::Late_Update(_float fTimeDelta)
{

}

HRESULT CModule::Render()
{

	return S_OK;
}


HRESULT CModule::Ready_Components()
{

	return S_OK;
}





void CModule::Free()
{
	__super::Free();

}
