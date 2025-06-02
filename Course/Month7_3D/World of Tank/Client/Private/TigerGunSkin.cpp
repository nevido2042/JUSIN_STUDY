#include "TigerGunSkin.h"

#include "GameInstance.h"

CTigerGunSkin::CTigerGunSkin(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGun{ pDevice, pContext }
{

}

CTigerGunSkin::CTigerGunSkin(const CTigerGunSkin& Prototype)
	: CGun(Prototype)
{

}

HRESULT CTigerGunSkin::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTigerGunSkin::Initialize(void* pArg)
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


void CTigerGunSkin::Priority_Update(_float fTimeDelta)
{
	CGun::Priority_Update(fTimeDelta);
}

void CTigerGunSkin::Update(_float fTimeDelta)
{
	CGun::Update(fTimeDelta);
}

void CTigerGunSkin::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CTigerGunSkin::Render()
{
	//return CGun::Render();

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (m_pModelCom)
	{
		_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

		for (_uint i = 0; i < iNumMesh; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Begin(3))) //이놈은 논 컬해야함
				return E_FAIL;

			if (FAILED(m_pModelCom->Render(i)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CTigerGunSkin::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_TigerGunSkin"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Sound */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController_TankSound3D"),
		TEXT("Com_Sound"), reinterpret_cast<CComponent**>(&m_pSoundCom))))
		return E_FAIL;

	return S_OK;
}

CTigerGunSkin* CTigerGunSkin::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTigerGunSkin* pInstance = new CTigerGunSkin(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTigerGunSkin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTigerGunSkin::Clone(void* pArg)
{
	CTigerGunSkin* pInstance = new CTigerGunSkin(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTigerGunSkin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTigerGunSkin::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
