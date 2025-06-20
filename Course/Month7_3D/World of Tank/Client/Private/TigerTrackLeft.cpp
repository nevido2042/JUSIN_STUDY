#include "TigerTrackLeft.h"

#include "GameInstance.h"

CTigerTrackLeft::CTigerTrackLeft(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTrack(pDevice, pContext)
{

}

CTigerTrackLeft::CTigerTrackLeft(const CTigerTrackLeft& Prototype)
	: CTrack(Prototype)
{

}

HRESULT CTigerTrackLeft::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTigerTrackLeft::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}


void CTigerTrackLeft::Priority_Update(_float fTimeDelta)
{
	CTrack::Priority_Update(fTimeDelta);
}

void CTigerTrackLeft::Update(_float fTimeDelta)
{
	CTrack::Update(fTimeDelta);
}

void CTigerTrackLeft::Late_Update(_float fTimeDelta)
{
	CTrack::Late_Update(fTimeDelta);
}

HRESULT CTigerTrackLeft::Render()
{
	CTrack::Render();

	return S_OK;
}


HRESULT CTigerTrackLeft::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_TigerTrackLeft"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_OBB::OBB_DESC	OBBDesc{};
	OBBDesc.vExtents = _float3(0.4f, 0.6f, 2.9f);
	OBBDesc.vCenter = _float3(-1.4f, OBBDesc.vExtents.y, 0.f);
	OBBDesc.vRotation = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &OBBDesc)))
		return E_FAIL;

	return S_OK;
}

CTigerTrackLeft* CTigerTrackLeft::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTigerTrackLeft* pInstance = new CTigerTrackLeft(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTigerTrackLeft");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTigerTrackLeft::Clone(void* pArg)
{
	CTigerTrackLeft* pInstance = new CTigerTrackLeft(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTigerTrackLeft");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTigerTrackLeft::Free()
{
	__super::Free();

}
