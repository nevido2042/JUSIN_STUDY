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

	m_pSoundCom[PLANE_SOUND_FLY]->Set3DState(0.f, 200.f);
	m_pSoundCom[PLANE_SOUND_FLY]->SetVolume(0.0f);

	m_pSoundCom[PLANE_SOUND_GUN]->Set3DState(0.f, 100.f);
	m_pSoundCom[PLANE_SOUND_GUN]->SetVolume(0.0f);

	m_pSoundCom[PLANE_SOUND_FLY]->Play("Fly");
	m_pSoundCom[PLANE_SOUND_FLY]->Set_Loop("Fly");

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	Reset_Plane();

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
	if (m_fProgress > 0.1f)
	{
		m_pSoundCom[PLANE_SOUND_FLY]->SetVolume(0.2f);
		m_pSoundCom[PLANE_SOUND_GUN]->SetVolume(0.2f);
	}

	m_pSoundCom[PLANE_SOUND_FLY]->Update3DPosition(m_pTransformCom->Get_State(STATE::POSITION));
	m_pSoundCom[PLANE_SOUND_GUN]->Update3DPosition(m_pTransformCom->Get_State(STATE::POSITION));

	_float3 vControlPos = { TERRAIN_SIZE * TERRAIN_OFFSET_WIDTH * 0.5f, m_fControlHeight, TERRAIN_SIZE * TERRAIN_OFFSET_WIDTH * 0.5f };

	_vector vCurPos = Bezier2(m_vStartPos, vControlPos, m_vEndPos, m_fProgress);
	_vector vNextPos = Bezier2(m_vStartPos, vControlPos, m_vEndPos, m_fProgress + fTimeDelta * 0.05f);

	m_pTransformCom->Set_State(STATE::POSITION, vCurPos);

	m_pTransformCom->LookAt(vNextPos);

	m_fProgress += fTimeDelta * 0.05f;

	if (m_fProgress > 1.f)
	{
		Reset_Plane();
	}

#pragma region 랜덤 소리 재생

	if (!m_pSoundCom[PLANE_SOUND_GUN]->IsPlaying("Gun") &&
		!m_pSoundCom[PLANE_SOUND_GUN]->IsPlaying("GunMix") &&
		!m_pSoundCom[PLANE_SOUND_GUN]->IsPlaying("PlaneGun"))
	{
		switch (rand() % 3)
		{
		case 0:
			m_pSoundCom[PLANE_SOUND_GUN]->Play("Gun");
			break;
		case 1:
			m_pSoundCom[PLANE_SOUND_GUN]->Play("GunMix");
			break;
		case 2:
			m_pSoundCom[PLANE_SOUND_GUN]->Play("PlaneGun");
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

void CPlane::Reset_Plane()
{
	m_bVisible = false;

	//위치 초기화
	m_fProgress = 0.f;
	m_fAccTime = 0.f;
	m_fWaitTime = rand() % 20 + 0.f; //랜덤으로 대기시간 설정

	_float fHeight = 200.f + rand() % 100; //랜덤으로 높이 설정
	m_fControlHeight = -10.f + rand() % 10; //랜덤으로 컨트롤 높이 설정
	//위치 랜덤	
	switch (rand() % PLANE_DIR_END)
	{
	case PLANE_DIR_RIGHT:
		m_vStartPos = { -500.f, fHeight, rand() % 1501 - 500.f };
		m_vEndPos = { 1000.f, fHeight, rand() % 1501 - 500.f };
		break;
	case PLANE_DIR_LEFT:

		m_vStartPos = { 1000.f, fHeight, rand() % 1501 - 500.f };
		m_vEndPos = { -500.f, fHeight, rand() % 1501 - 500.f };
		break;
	case PLANE_DIR_UP:
		m_vStartPos = { rand() % 1501 - 500.f, fHeight,   -500.f };
		m_vEndPos = { rand() % 1501 - 500.f , fHeight, 1000.f };
		break;
	case PLANE_DIR_DOWN:
		m_vStartPos = { rand() % 1501 - 500.f , fHeight, 1000.f };
		m_vEndPos = { rand() % 1501 - 500.f, fHeight,   -500.f };
		break;
	default:
		break;
	}
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
		TEXT("Com_Sound_Fly"), reinterpret_cast<CComponent**>(&m_pSoundCom[PLANE_SOUND_FLY]))))
		return E_FAIL;

	/* For.Com_Sound */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SoundController_Plane"),
		TEXT("Com_Sound_Gun"), reinterpret_cast<CComponent**>(&m_pSoundCom[PLANE_SOUND_GUN]))))
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

	for (auto pSoundCom : m_pSoundCom)
	{
		Safe_Release(pSoundCom);
	}

}
