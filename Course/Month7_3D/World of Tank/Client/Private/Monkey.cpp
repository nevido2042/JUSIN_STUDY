#include "Monkey.h"

CMonkey::CMonkey(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:C3DCustom(pDevice, pContext)
{
}

CMonkey::CMonkey(const CMonkey& Prototype)
	:C3DCustom(Prototype)
{
}

HRESULT CMonkey::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	
	return S_OK;
}

void CMonkey::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CMonkey::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CMonkey::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CMonkey::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CMonkey::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Monkey"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

CMonkey* CMonkey::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonkey* pInstance = new CMonkey(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMonkey");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonkey::Clone(void* pArg)
{
	CMonkey* pInstance = new CMonkey(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMonkey");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonkey::Free()
{
	__super::Free();
}
