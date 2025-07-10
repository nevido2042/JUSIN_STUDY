#include "Speaker.h"

CSpeaker::CSpeaker(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:C3DCustom(pDevice, pContext)
{
}

CSpeaker::CSpeaker(const CSpeaker& Prototype)
	:C3DCustom(Prototype)
{
}

HRESULT CSpeaker::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CSpeaker::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CSpeaker::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CSpeaker::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CSpeaker::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CSpeaker::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Speaker"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

CSpeaker* CSpeaker::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSpeaker* pInstance = new CSpeaker(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSpeaker");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpeaker::Clone(void* pArg)
{
	CSpeaker* pInstance = new CSpeaker(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSpeaker");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpeaker::Free()
{
	__super::Free();
}
