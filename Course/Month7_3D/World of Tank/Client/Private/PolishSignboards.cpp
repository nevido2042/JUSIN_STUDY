#include "PolishSignboards.h"

#include "GameInstance.h"

CPolishSignboards::CPolishSignboards(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CPolishSignboards::CPolishSignboards(const CPolishSignboards& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CPolishSignboards::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPolishSignboards::Initialize(void* pArg)
{
	GAMEOBJECT_DESC			Desc{};
	Desc.fRotationPerSec = 1.f;
	Desc.fSpeedPerSec = 0.f;
	lstrcpy(Desc.szName, TEXT("PolishSigneboards"));

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pSoundCom->Set3DState(0.f, 30.f);

	return S_OK;
}


void CPolishSignboards::Priority_Update(_float fTimeDelta)
{
	if (m_bFallComplete == true)
		return;

	if (m_bIsFall == false)
		m_pGameInstance->Add_CollisionGroup(ENUM_CLASS(COLLISION_GROUP::TREE), this, TEXT("Com_Collider"));
}

void CPolishSignboards::Update(_float fTimeDelta)
{
	m_pSoundCom->Update3DPosition(m_pTransformCom->Get_State(STATE::POSITION));

	if (m_bFallComplete == true)
		return;

	if (m_bIsFall == false)
	{
		m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
		m_pGameInstance->Check_Collision(ENUM_CLASS(COLLISION_GROUP::BODY), this, TEXT("Com_Collider"), TEXT("Com_Collider"));
	}
	else
	{
		//월드의 업벡터랑, 업벡터랑 내적했을 때 0.3f 이하가 되면 멈춰라
		_vector vWorldUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		_vector vUp = m_pTransformCom->Get_State(STATE::UP);
		_float fDot = XMVectorGetX(XMVector3Dot(vWorldUp, vUp));
		if (fDot < 0.3f)
		{
			m_bFallComplete = true;
			return;
		}

		m_pTransformCom->Turn(XMLoadFloat3(&m_vAxis), fTimeDelta);
	}
}

void CPolishSignboards::Late_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(STATE::POSITION), 10.f))
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);

}

HRESULT CPolishSignboards::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (m_pModelCom)
	{
		_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

		for (_uint i = 0; i < iNumMesh; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Begin(0)))
				return E_FAIL;

			if (FAILED(m_pModelCom->Render(i)))
				return E_FAIL;
		}
	}

#ifdef _DEBUG
	m_pColliderCom->Render();
#endif
	return S_OK;
}

void CPolishSignboards::On_Collision_Stay(CGameObject* pOther, _fvector vNormal)
{
	m_pSoundCom->Play("phys_coll_30");

	m_bIsFall = true;

	//탱크의 반대 방향으로 넘어져야한다.
	_vector vOtherPos = pOther->Get_Transform()->Get_State(STATE::POSITION);
	_vector vTreePos = m_pTransformCom->Get_State(STATE::POSITION);

	_vector vDir = vTreePos - vOtherPos;
	_vector vWorldUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	_vector vAxis = XMVector3Cross(vWorldUp, vDir);

	XMStoreFloat3(&m_vAxis, vAxis);
	//Destroy();
}

HRESULT CPolishSignboards::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_PolishSignboards"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_Sphere::SPHERE_DESC	SphereDesc{};
	SphereDesc.fRadius = 0.5f;
	SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;

	/* For.Com_Sound */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController_Tree3D"),
		TEXT("Com_Sound"), reinterpret_cast<CComponent**>(&m_pSoundCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CPolishSignboards::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	return S_OK;
}

CPolishSignboards* CPolishSignboards::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPolishSignboards* pInstance = new CPolishSignboards(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPolishSignboards");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPolishSignboards::Clone(void* pArg)
{
	CPolishSignboards* pInstance = new CPolishSignboards(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPolishSignboards");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPolishSignboards::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pSoundCom);

}
