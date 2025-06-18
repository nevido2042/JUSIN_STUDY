#include "FuryGun.h"

#include "GameInstance.h"

CFuryGun::CFuryGun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGun{ pDevice, pContext }
{

}

CFuryGun::CFuryGun(const CFuryGun& Prototype)
	: CGun(Prototype)
{

}

HRESULT CFuryGun::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFuryGun::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	//블렌더에서 가져온걸로
	//1. x 100.f
	//2. 축 순서 변경
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(-0.0041f, 2.2863f, 0.8589f, 1.0f));

	m_pSoundCom->Set3DState(0.f, 100.f);

	return S_OK;
}


void CFuryGun::Priority_Update(_float fTimeDelta)
{
	CGun::Priority_Update(fTimeDelta);

}

void CFuryGun::Update(_float fTimeDelta)
{
	CGun::Update(fTimeDelta);

}

void CFuryGun::Late_Update(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CFuryGun::Render()
{
	return CGun::Render();
}

HRESULT CFuryGun::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_FuryGun"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Sound */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController_TankSound3D"),
		TEXT("Com_Sound"), reinterpret_cast<CComponent**>(&m_pSoundCom))))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_OBB::OBB_DESC	OBBDesc{};
	OBBDesc.vExtents = _float3(0.1f, 0.1f, 1.8f);
	OBBDesc.vCenter = _float3(0.f, 0.f, 1.5f);
	OBBDesc.vRotation = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &OBBDesc)))
		return E_FAIL;

	return S_OK;
}

CFuryGun* CFuryGun::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFuryGun* pInstance = new CFuryGun(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFuryGun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFuryGun::Clone(void* pArg)
{
	CFuryGun* pInstance = new CFuryGun(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CFuryGun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFuryGun::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
