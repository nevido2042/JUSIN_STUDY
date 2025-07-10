#include "Shark.h"

CShark::CShark(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:C3DCustom(pDevice, pContext)
{
}

CShark::CShark(const CShark& Prototype)
	:C3DCustom(Prototype)
{
}

HRESULT CShark::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CShark::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CShark::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CShark::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CShark::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CShark::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Shark"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

CShark* CShark::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CShark* pInstance = new CShark(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CShark");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CShark::Clone(void* pArg)
{
	CShark* pInstance = new CShark(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CShark");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShark::Free()
{
	__super::Free();
}
