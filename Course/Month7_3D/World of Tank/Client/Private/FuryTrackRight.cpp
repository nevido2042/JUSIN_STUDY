#include "FuryTrackRight.h"

#include "GameInstance.h"

CFuryTrackRight::CFuryTrackRight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTrack(pDevice, pContext)
{

}

CFuryTrackRight::CFuryTrackRight(const CFuryTrackRight& Prototype)
	: CTrack(Prototype)
{

}

HRESULT CFuryTrackRight::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFuryTrackRight::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}


void CFuryTrackRight::Priority_Update(_float fTimeDelta)
{
	CTrack::Priority_Update(fTimeDelta);
}

void CFuryTrackRight::Update(_float fTimeDelta)
{
	CTrack::Update(fTimeDelta);
}

void CFuryTrackRight::Late_Update(_float fTimeDelta)
{
	CTrack::Late_Update(fTimeDelta);
}

HRESULT CFuryTrackRight::Render()
{
	CTrack::Render();

	return S_OK;
}


HRESULT CFuryTrackRight::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_FuryTrackRight"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

CFuryTrackRight* CFuryTrackRight::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFuryTrackRight* pInstance = new CFuryTrackRight(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFuryTrackRight");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFuryTrackRight::Clone(void* pArg)
{
	CFuryTrackRight* pInstance = new CFuryTrackRight(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CFuryTrackRight");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFuryTrackRight::Free()
{
	__super::Free();

}
