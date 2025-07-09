#include "Drum.h"

CDrum::CDrum(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:C3DCustom(pDevice, pContext)
{
}

CDrum::CDrum(const CDrum& Prototype)
	:C3DCustom(Prototype)
{
}

HRESULT CDrum::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CDrum::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CDrum::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CDrum::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CDrum::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CDrum::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Drum"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

CDrum* CDrum::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDrum* pInstance = new CDrum(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDrum");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDrum::Clone(void* pArg)
{
	CDrum* pInstance = new CDrum(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDrum");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDrum::Free()
{
	__super::Free();
}
