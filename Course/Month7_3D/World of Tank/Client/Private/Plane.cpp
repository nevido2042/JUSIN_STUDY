#include "Plane.h"

#include "GameInstance.h"


CPlane::CPlane(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CPlane::CPlane(const CPlane& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CPlane::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlane::Initialize(void* pArg)
{
	GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
	//(*pDesc).fSpeedPerSec = 20.f;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pSoundCom->Set3DState(0.f, 200.f);
	m_pSoundCom->SetVolume(0.0f);

	m_pSoundCom->Play("Fly");
	m_pSoundCom->Set_Loop("Fly");

	m_fWaitTime = rand() % 5 + 15.f; //5초 ~ 10초 사이에 랜덤으로 대기시간 설정

	//m_pSoundCom->Play("GunMix");
	//m_pSoundCom->Set_Loop("GunMix");

//위치 랜덤	
	switch (rand() % PLANE_DIR_END)
	{
	case PLANE_DIR_RIGHT:
		m_vStartPos = { -500.f, 200.f, rand() % 1501 - 500.f };
		m_vEndPos = { 1000.f, 200.f, rand() % 1501 - 500.f };
		break;
	case PLANE_DIR_LEFT:

		m_vStartPos = { 1000.f, 200.f, rand() % 1501 - 500.f };
		m_vEndPos = { -500.f, 200.f, rand() % 1501 - 500.f };
		break;
	case PLANE_DIR_UP:
		m_vStartPos = { rand() % 1501 - 500.f, 200.f,   -500.f };
		m_vEndPos = { rand() % 1501 - 500.f , 200.f, 1000.f };
		break;
	case PLANE_DIR_DOWN:
		m_vStartPos = { rand() % 1501 - 500.f , 200.f, 1000.f };
		m_vEndPos = { rand() % 1501 - 500.f, 200.f,   -500.f };
		break;
	default:
		break;
	}

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	return S_OK;
}


void CPlane::Priority_Update(_float fTimeDelta)
{

	CGameObject::Priority_Update(fTimeDelta);
}

void CPlane::Update(_float fTimeDelta)
{
	m_fAccTime += fTimeDelta;
	if (m_fWaitTime > m_fAccTime)
		return;

	m_bVisible = true;

	//화면전환시 처음 소리 나는거 싫어서
	if (fProgress > 0.1f)
	{
		m_pSoundCom->SetVolume(0.2f);
	}

	m_pSoundCom->Update3DPosition(m_pTransformCom->Get_State(STATE::POSITION));

	_float3 vControlPos = { TERRAIN_SIZE * TERRAIN_OFFSET_WIDTH * 0.5f, 0.f, TERRAIN_SIZE * TERRAIN_OFFSET_WIDTH * 0.5f };

	_vector vCurPos = Bezier2(m_vStartPos, vControlPos, m_vEndPos, fProgress);
	_vector vNextPos = Bezier2(m_vStartPos, vControlPos, m_vEndPos, fProgress + fTimeDelta * 0.05f);

	m_pTransformCom->Set_State(STATE::POSITION, vCurPos);

	m_pTransformCom->LookAt(vNextPos);

	fProgress += fTimeDelta * 0.05f;

	if (fProgress > 1.f)
	{
		m_bVisible = false;

		//위치 초기화
		fProgress = 0.f;
		m_fAccTime = 0.f;
		m_fWaitTime = rand() % 5 + 5.f; //5초 ~ 10초 사이에 랜덤으로 대기시간 설정

		//위치 랜덤	
		switch (rand()% PLANE_DIR_END)
		{
		case PLANE_DIR_RIGHT:
			m_vStartPos = { -500.f, 200.f, rand() % 1501 - 500.f };
			m_vEndPos = { 1000.f, 200.f, rand() % 1501 - 500.f };
			break;
		case PLANE_DIR_LEFT:
		
			m_vStartPos = { 1000.f, 200.f, rand() % 1501 - 500.f };
			m_vEndPos = { -500.f, 200.f, rand() % 1501 - 500.f };
			break;
		case PLANE_DIR_UP:
			m_vStartPos = { rand() % 1501 - 500.f, 200.f,   -500.f };
			m_vEndPos = { rand() % 1501 - 500.f , 200.f, 1000.f };
			break;
		case PLANE_DIR_DOWN:
			m_vStartPos = { rand() % 1501 - 500.f , 200.f, 1000.f };
			m_vEndPos = { rand() % 1501 - 500.f, 200.f,   -500.f };
			break;
		default:
			break;
		}


	}

#pragma region 랜덤 소리 재생

	if (!m_pSoundCom->IsPlaying("Gun") &&
		!m_pSoundCom->IsPlaying("GunMix") &&
		!m_pSoundCom->IsPlaying("PlaneGun"))
	{
		switch (rand() % 3)
		{
		case 0:
			m_pSoundCom->Play("Gun");
			break;
		case 1:
			m_pSoundCom->Play("GunMix");
			break;
		case 2:
			m_pSoundCom->Play("PlaneGun");
			break;
		default:
			break;
		}
	}
#pragma endregion




	CGameObject::Update(fTimeDelta);
}

void CPlane::Late_Update(_float fTimeDelta)
{
	//if (m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(STATE::POSITION), 30.f))
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_SHADOW, this);

	if (m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(STATE::POSITION), 30.f))
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);

	CGameObject::Late_Update(fTimeDelta);
}

HRESULT CPlane::Render()
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

			/*if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS, 0)))
				return E_FAIL;*/

			if (FAILED(m_pShaderCom->Begin(5)))
				return E_FAIL;

			if (FAILED(m_pModelCom->Render(i)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CPlane::Render_Shadow()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Light_ViewMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Light_ProjMatrix())))
		return E_FAIL;

	_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMesh; i++)
	{
		if (FAILED(m_pShaderCom->Begin(4)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

_vector CPlane::Bezier2(_float3 vStart, _float3 vControl, _float3 vEnd, _float fT)
{
	_vector vA = XMVectorSetW(XMLoadFloat3(&vStart), 1.f);
	_vector vB = XMVectorSetW(XMLoadFloat3(&vControl), 1.f);
	_vector vC = XMVectorSetW(XMLoadFloat3(&vEnd), 1.f);

	_vector vAB = XMVectorLerp(vA, vB, fT);
	_vector vBC = XMVectorLerp(vB, vC, fT);
	return XMVectorLerp(vAB, vBC, fT);
}

HRESULT CPlane::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Plane"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Sound */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController_Plane"),
		TEXT("Com_Sound"), reinterpret_cast<CComponent**>(&m_pSoundCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlane::Ready_PartObjects()
{
	//GAMEOBJECT_DESC SmokeDesc = {};
	//SmokeDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	//SmokeDesc.vInitPosition = _float3(0.f, 0.f, -2.0f);
	//if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_MissileSmoke"), TEXT("Part_Smoke"), &SmokeDesc)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CPlane::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	return S_OK;
}

CPlane* CPlane::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlane* pInstance = new CPlane(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlane");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlane::Clone(void* pArg)
{
	CPlane* pInstance = new CPlane(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlane");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlane::Free()
{
	__super::Free();

	for (auto& Pair : m_PartObjects)
		Safe_Release(Pair.second);
	m_PartObjects.clear();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pSoundCom);

}
