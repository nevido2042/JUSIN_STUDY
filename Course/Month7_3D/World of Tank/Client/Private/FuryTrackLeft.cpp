#include "FuryTrackLeft.h"

#include "GameInstance.h"

CFuryTrackLeft::CFuryTrackLeft(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTrack(pDevice, pContext)
{

}

CFuryTrackLeft::CFuryTrackLeft(const CFuryTrackLeft& Prototype)
	: CTrack(Prototype)
{

}

HRESULT CFuryTrackLeft::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFuryTrackLeft::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}


void CFuryTrackLeft::Priority_Update(_float fTimeDelta)
{
	CTrack::Priority_Update(fTimeDelta);
}

void CFuryTrackLeft::Update(_float fTimeDelta)
{
	CTrack::Update(fTimeDelta);
}

void CFuryTrackLeft::Late_Update(_float fTimeDelta)
{
	CTrack::Late_Update(fTimeDelta);
}

HRESULT CFuryTrackLeft::Render()
{
	CTrack::Render();

	return S_OK;
}


HRESULT CFuryTrackLeft::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_FuryTrackLeft"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_OBB::OBB_DESC	OBBDesc{};
	OBBDesc.vExtents = _float3(0.3f, 0.5f, 2.7f);
	OBBDesc.vCenter = _float3(-1.15f, OBBDesc.vExtents.y, -0.1f);
	OBBDesc.vRotation = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &OBBDesc)))
		return E_FAIL;

	return S_OK;
}

CFuryTrackLeft* CFuryTrackLeft::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFuryTrackLeft* pInstance = new CFuryTrackLeft(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFuryTrackLeft");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFuryTrackLeft::Clone(void* pArg)
{
	CFuryTrackLeft* pInstance = new CFuryTrackLeft(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CFuryTrackLeft");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFuryTrackLeft::Free()
{
	__super::Free();

}
