#include "TigerTrackRight.h"

#include "GameInstance.h"

CTigerTrackRight::CTigerTrackRight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTrack(pDevice, pContext)
{

}

CTigerTrackRight::CTigerTrackRight(const CTigerTrackRight& Prototype)
	: CTrack(Prototype)
{

}

HRESULT CTigerTrackRight::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTigerTrackRight::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}


void CTigerTrackRight::Priority_Update(_float fTimeDelta)
{
	CTrack::Priority_Update(fTimeDelta);
}

void CTigerTrackRight::Update(_float fTimeDelta)
{
	CTrack::Update(fTimeDelta);
}

void CTigerTrackRight::Late_Update(_float fTimeDelta)
{
	CTrack::Late_Update(fTimeDelta);
}

HRESULT CTigerTrackRight::Render()
{
	CTrack::Render();

	return S_OK;
}


HRESULT CTigerTrackRight::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_TigerTrackRight"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

CTigerTrackRight* CTigerTrackRight::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTigerTrackRight* pInstance = new CTigerTrackRight(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTigerTrackRight");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTigerTrackRight::Clone(void* pArg)
{
	CTigerTrackRight* pInstance = new CTigerTrackRight(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTigerTrackRight");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTigerTrackRight::Free()
{
	__super::Free();

}
