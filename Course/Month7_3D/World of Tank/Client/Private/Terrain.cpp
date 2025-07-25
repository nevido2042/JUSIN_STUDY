﻿#include "Terrain.h"

#include "PickedManager.h"
#include "GameInstance.h"

CTerrain::CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject { pDevice, pContext }
{

}

CTerrain::CTerrain(const CTerrain& Prototype)
	: CGameObject (Prototype)
{

}

HRESULT CTerrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTerrain::Initialize(void* pArg)
{
	GAMEOBJECT_DESC			Desc{};

	Desc.fRotationPerSec = 0.f;
	Desc.fSpeedPerSec = 0.f;
	lstrcpy(Desc.szName, TEXT("Terrain"));	

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

void CTerrain::Priority_Update(_float fTimeDelta)
{
	Picking_Mouse();

	//Picking_ScreenCenter();

	//Picking_Gun();

}

void CTerrain::Update(_float fTimeDelta)
{
	//if (m_pGameInstance->Key_Down(DIK_K))
	//{
	//	_vector vTerrainCenter = m_pVIBufferCom->Compute_HeightPosition(XMVectorSet(TERRAIN_SIZE * TERRAIN_OFFSET_WIDTH * 0.5f,  0.f, TERRAIN_SIZE * TERRAIN_OFFSET_WIDTH * 0.5f, 1.f));
	//	_float3 vDigCenter = {};
	//	XMStoreFloat3(&vDigCenter, vTerrainCenter);
	//	m_pVIBufferCom->DigGround(vDigCenter, TERRAIN_SIZE, 100.f);
	//}

}

void CTerrain::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CTerrain::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

_bool CTerrain::Pick(_fvector vOrigin, _fvector vRayDir, _float& fOutDist)
{
	// 쿼드트리 픽킹
	_float fDist = { 0 }; //거리
	_uint iPickedTri = { 0 }; //피킹된 인덱스
	_float3 vPos = {}; //포지션
	if (m_pVIBufferCom->PickQuadTreeNode(vPos, fDist, iPickedTri, vOrigin, vRayDir))
	{
		fOutDist = fDist;
		return true;
	}
	
	return false;
}

HRESULT CTerrain::Picking_Mouse()
{
	_uint iLevelIndex = { m_pGameInstance->Get_NewLevel_Index() };

	if (iLevelIndex == ENUM_CLASS(LEVEL::MAPTOOL) && m_pGameInstance->Mouse_Pressing(ENUM_CLASS(DIMK::LBUTTON)))
	{
		// 쿼드트리 픽킹
		_float fDist = { 0 }; //거리
		_uint iPickedTri = { 0 }; //피킹된 인덱스
		_float3 vPos = {}; //포지션
		if (m_pVIBufferCom->PickQuadTreeNode(vPos, fDist, iPickedTri, XMLoadFloat3(&m_pGameInstance->Get_MousePos()), XMLoadFloat3(&m_pGameInstance->Get_MouseRay())))
		{
			m_vPickedPos = vPos;
			//m_pVIBufferCom->DigGround(m_vPickedPos, 10.f, 10.f);
		}
	}

	return S_OK;
}

HRESULT CTerrain::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxNorTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Terrain"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Terrain"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_DIFFUSE]))))
		return E_FAIL;

	/* For.Com_Mask*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Mask"),
		TEXT("Com_Mask"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_MASK]))))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	/* dx9 : 장치에 뷰, 투영행렬을 저장해두면 렌더링시 알아서 정점에 Transform해주었다. */
	/* dx11 : 셰이더에 뷰, 투영행렬을 저장해두고 우리가 직접 변환해주어야한다. */

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom[TEXTURE_DIFFUSE]->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(m_pTextureCom[TEXTURE_MASK]->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture", 0)))
		return E_FAIL;

	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
	//	return E_FAIL;

	return S_OK;
}

CTerrain* CTerrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTerrain* pInstance = new CTerrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTerrain::Clone(void* pArg)
{
	CTerrain* pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	//Safe_Release(m_pTextureCom);

	for (auto& pTextureCom : m_pTextureCom)
		Safe_Release(pTextureCom);
}
