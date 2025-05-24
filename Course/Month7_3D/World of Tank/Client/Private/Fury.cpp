#include "Fury.h"

#include "GameInstance.h"
#include "Engine.h"
#include "FuryTrackLeft.h"
#include "FuryTrackRight.h"
#include "FuryTurret.h"
#include "FuryGun.h"

CFury::CFury(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLandObject{ pDevice, pContext }
{

}

CFury::CFury(const CFury& Prototype)
	: CLandObject(Prototype)
{

}

HRESULT CFury::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFury::Initialize(void* pArg)
{
	FURY_DESC* pDesc = static_cast<FURY_DESC*>(pArg);
	m_iID = pDesc->iID;

	LANDOBJECT_DESC		Desc{};
	Desc.fRotationPerSec = 0.1f;
	Desc.fSpeedPerSec = 0.5f;
	lstrcpy(Desc.szName, TEXT("Fury"));

#pragma message ("New레벨에 소환한다는게 위험할 수도 있겠다. 다른 플레이어 소환할때...지금은 괜찮지만")
	Desc.iLevelIndex = m_pGameInstance->Get_NewLevel_Index();
	Desc.strLayerTag = TEXT("Layer_Terrain");
	Desc.strComponentTag = TEXT("Com_VIBuffer");
	Desc.iIndex = 0;
	Desc.vInitPosition = pDesc->vInitPosition;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	// 새 RasterizerState 설정
	D3D11_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_FRONT;
	rasterDesc.FrontCounterClockwise = FALSE;

	m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterState);

	return S_OK;
}


void CFury::Priority_Update(_float fTimeDelta)
{
	Move(fTimeDelta);

	if (m_pGameInstance->Key_Down(DIK_F1))
	{
		Destroyed();
	}

	__super::SetUp_Height_Normal(m_pTransformCom, fTimeDelta, 0.5f);


	CGameObject::Priority_Update(fTimeDelta);

}

void CFury::Update(_float fTimeDelta)
{

	CGameObject::Update(fTimeDelta);

	if (m_pGameInstance->Get_ID() == m_iID && GetForegroundWindow() == g_hWnd && m_pGameInstance->Get_NewLevel_Index() != ENUM_CLASS(LEVEL::PRACTICE))
	{
		if (m_fTimeAcc > m_fSyncInterval)
		{
			m_fTimeAcc = 0.f;

			MATRIX_DESC Desc{};
			Desc.iID = m_pGameInstance->Get_ID();
			XMStoreFloat4x4(&Desc.Matrix, m_pTransformCom->Get_WorldMatrix());

			m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_MATRIX), &Desc);
		}
		else
			m_fTimeAcc += fTimeDelta;
	}
	
}

void CFury::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);

	if (m_bDestroyed)
		return;


	CGameObject::Late_Update(fTimeDelta);

}


HRESULT CFury::Render()
{	
	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (m_pModelCom && !m_bDestroyed)
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
	else if(m_pModelCom_Destroyed)
	{
		_uint		iNumMesh = m_pModelCom_Destroyed->Get_NumMeshes();

		for (_uint i = 0; i < iNumMesh; i++)
		{
			if (FAILED(m_pModelCom_Destroyed->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Begin(0)))
				return E_FAIL;

			if (FAILED(m_pModelCom_Destroyed->Render(i)))
				return E_FAIL;
		}
	}

	if (FAILED(Release_RenderState()))
		return E_FAIL;


	return S_OK;
}

void CFury::Destroyed()
{
	m_bDestroyed = true;
}

void CFury::Move(_float fTimeDelta)
{
	CEngine*	pEngin = static_cast<CEngine*>(Find_PartObject(TEXT("Part_Engine")));

	if (!pEngin->Get_isOn())
		return;

	CFuryTrackLeft* pTrackLeft = static_cast<CFuryTrackLeft*>(Find_PartObject(TEXT("Part_TrackLeft")));
	CFuryTrackRight* pTrackRight = static_cast<CFuryTrackRight*>(Find_PartObject(TEXT("Part_TrackRight")));

	pTrackLeft->Set_Speed(0.f);
	pTrackRight->Set_Speed(0.f);
	//m_fTurnDirection = 0.f;

	_float fMovePower = { pEngin->Get_MovePower() };
	_float fRPMPower = { pEngin->Get_RPM() };

	_float SpeedTrackLeft = 0.f;
	_float SpeedTrackRight = 0.f;

	if (m_pGameInstance->Key_Pressing(DIK_W))
	{
		SpeedTrackLeft += -fRPMPower;
		SpeedTrackRight += -fRPMPower;

		if (m_pGameInstance->Key_Pressing(DIK_A))
		{
			SpeedTrackRight += -fRPMPower;
		}
		else if (m_pGameInstance->Key_Pressing(DIK_D))
		{
			SpeedTrackLeft += -fRPMPower;
		}
	}
	else if (m_pGameInstance->Key_Pressing(DIK_S))
	{
		SpeedTrackLeft += fRPMPower;
		SpeedTrackRight += fRPMPower;

		if (m_pGameInstance->Key_Pressing(DIK_A))
		{
			SpeedTrackRight += fRPMPower;
		}
		else if (m_pGameInstance->Key_Pressing(DIK_D))
		{
			SpeedTrackLeft += fRPMPower;
		}
	}
	else
	{
		if (m_pGameInstance->Key_Pressing(DIK_A))
		{
			SpeedTrackLeft += fRPMPower * 0.3f;
			SpeedTrackRight += -fRPMPower;
		}
		else if (m_pGameInstance->Key_Pressing(DIK_D))
		{
			SpeedTrackLeft += -fRPMPower;
			SpeedTrackRight += fRPMPower * 0.3f;
		}
	}

	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), pEngin->Get_TurnPower());

	//if(pEngin->Get_Gear() != GEAR::LEFT && pEngin->Get_Gear() != GEAR::RIGHT)
		m_pTransformCom->Go_Straight(fMovePower);

	pTrackLeft->Set_Speed(SpeedTrackLeft);
	pTrackRight->Set_Speed(SpeedTrackRight);
}

HRESULT CFury::SetUp_RenderState()
{
	m_pContext->RSGetState(&m_pOldRasterState);
	m_pContext->RSSetState(m_pRasterState);

	return S_OK;
}

HRESULT CFury::Release_RenderState()
{
	m_pContext->RSSetState(m_pOldRasterState);

	return S_OK;
}

HRESULT CFury::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_Light(0);

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

HRESULT CFury::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_FuryBody"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Model_Destroyed */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_FuryDestroyed"),
		TEXT("Com_Model_Destroyed"), reinterpret_cast<CComponent**>(&m_pModelCom_Destroyed))))
		return E_FAIL;

	return S_OK;
}

HRESULT CFury::Ready_PartObjects()
{
	/* 포탑을 추가한다. */
	CGameObject::GAMEOBJECT_DESC Desc{};
	Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Desc.fRotationPerSec = 1.f;

	lstrcpy(Desc.szName, TEXT("Turret"));
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_FuryTurret"), TEXT("Part_Turret"), &Desc)))
		return E_FAIL;

	CEngine::ENGINE_DESC EngineDesc{};
	EngineDesc.pParentWorldMatrix = Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	EngineDesc.fRotationPerSec = 1.f;
	EngineDesc.iID = m_iID;
	/* 엔진을 추가한다. */
	lstrcpy(Desc.szName, TEXT("Engine"));
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Engine"), TEXT("Part_Engine"), &EngineDesc)))
		return E_FAIL;

	/* 왼쪽 궤도를 추가한다. */
	lstrcpy(Desc.szName, TEXT("TrackLeft"));
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_FuryTrackLeft"), TEXT("Part_TrackLeft"), &Desc)))
		return E_FAIL;

	/* 오른쪽 궤도를 추가한다. */
	lstrcpy(Desc.szName, TEXT("TrackRight"));
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_FuryTrackRight"), TEXT("Part_TrackRight"), &Desc)))
		return E_FAIL;

	return S_OK;
}

CFury* CFury::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFury* pInstance = new CFury(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFury");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFury::Clone(void* pArg)
{
	CFury* pInstance = new CFury(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CFury");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFury::Free()
{
	__super::Free();

	for (auto& Pair : m_PartObjects)
		Safe_Release(Pair.second);
	m_PartObjects.clear();

	Safe_Release(m_pRasterState);
	Safe_Release(m_pOldRasterState);

	Safe_Release(m_pModelCom_Destroyed);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
