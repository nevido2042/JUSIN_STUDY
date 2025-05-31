#include "TigerGun.h"

#include "GameInstance.h"

CTigerGun::CTigerGun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGun{ pDevice, pContext }
{

}

CTigerGun::CTigerGun(const CTigerGun& Prototype)
	: CGun(Prototype)
{

}

HRESULT CTigerGun::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTigerGun::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	//블렌더에서 가져온걸로
	//1. x 100.f
	//2. 축 순서 변경
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(-0.0098f, 2.2237f, 1.0188f, 1.0f));

	m_pSoundCom->Set3DState(0.f, 100.f);

	return S_OK;
}


void CTigerGun::Priority_Update(_float fTimeDelta)
{
	CGun::Priority_Update(fTimeDelta);
}

void CTigerGun::Update(_float fTimeDelta)
{
	CGun::Update(fTimeDelta);
}

void CTigerGun::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CTigerGun::Render()
{
	return CGun::Render();
}

HRESULT CTigerGun::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_TigerGun"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Sound */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController_TankSound3D"),
		TEXT("Com_Sound"), reinterpret_cast<CComponent**>(&m_pSoundCom))))
		return E_FAIL;

	return S_OK;
}

CTigerGun* CTigerGun::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTigerGun* pInstance = new CTigerGun(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTigerGun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTigerGun::Clone(void* pArg)
{
	CTigerGun* pInstance = new CTigerGun(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTigerGun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTigerGun::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
