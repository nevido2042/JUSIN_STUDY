#include "DamageBar_World.h"

#include "GameInstance.h"
#include "Terrain.h"
#include "Tank.h"

CDamageBar_World::CDamageBar_World(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CDamageBar_World::CDamageBar_World(const CDamageBar_World& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CDamageBar_World::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CDamageBar_World::Initialize(void* pArg)
{
	DAMAGEBAR_WORLD_DESC* pDesc = static_cast<DAMAGEBAR_WORLD_DESC*>(pArg);
	m_eTeam = pDesc->eTeam;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}


void CDamageBar_World::Priority_Update(_float fTimeDelta)
{
	if (g_bWindowResizeRequired)
		Resize(g_iWinSizeX, g_iWinSizeY);
}

void CDamageBar_World::Update(_float fTimeDelta)
{
	_vector vScale = {};
	_vector vRot = {};
	_vector vWorldPos = {};
	XMMatrixDecompose(&vScale, &vRot, &vWorldPos, XMLoadFloat4x4(m_pParentWorldMatrix));

	vWorldPos += XMVectorSet(0.f, 5.f, 0.f, 0.f);

	_matrix matView = m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW);
	_matrix matProj = m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ);

	_vector vClipPos = XMVector3TransformCoord(vWorldPos, matView * matProj);

	_float3 vClip;
	XMStoreFloat3(&vClip, vClipPos);

	_float2 vScreenPos;
	vScreenPos.x = (vClip.x * 0.5f) * g_iWinSizeX;
	vScreenPos.y = (vClip.y * 0.5f) * g_iWinSizeY;

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(vScreenPos.x, vScreenPos.y, m_fDepth, 1.f));


#pragma region 데미지 딜레이 표현
	if (m_fTimeAcc > 1.0f)
	{
		m_fFillDelay -= fTimeDelta * 0.3f;
		if (m_fFillDelay < m_fFillAmount)
			m_fFillDelay = m_fFillAmount;
	}
	else
	{
		m_fTimeAcc += fTimeDelta;
	}
#pragma endregion

}

void CDamageBar_World::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CDamageBar_World::Render()
{

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	//배경
	//_float4 vBaseColor = { 0.f, 0.f, 0.f, 1.f };
	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vBaseColor", &vBaseColor, sizeof(_float4))))
	//	return E_FAIL;

	//if (FAILED(m_pTextureCom_Background->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
	//	return E_FAIL;

	//if (FAILED(m_pVIBufferCom->Bind_Buffers()))
	//	return E_FAIL;

	//if (FAILED(m_pShaderCom->Begin(3)))
	//	return E_FAIL;

	//if (FAILED(m_pVIBufferCom->Render()))
	//	return E_FAIL;

	//바

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fFillDelay", &m_fFillDelay, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fFill", &m_fFillAmount, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(4)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CDamageBar_World::Fill(_float fFillAmount)
{
	m_fTimeAcc = 0.f;
	m_fFillDelay = m_fFillAmount;
	m_fFillAmount = fFillAmount;
}

HRESULT CDamageBar_World::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	CGameObject* pGameObject = m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_PlayerTank"));
	if (pGameObject)
	{
		CTank* pPlayerTank = static_cast<CTank*>(pGameObject);

		if (pPlayerTank->Get_Team() == m_eTeam)
		{
			/* For.Com_Texture */
			if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_DamageBar_World_Green"),
				TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
				return E_FAIL;
		}
		else
		{
			/* For.Com_Texture */
			if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_DamageBar_World_Red"),
				TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
				return E_FAIL;
		}
	}

	if (nullptr == m_pTextureCom)
		m_bActive = false;

	///* For.Com_Texture_Background */
	//if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_DamageBar"),
	//	TEXT("Com_Texture_Background"), reinterpret_cast<CComponent**>(&m_pTextureCom_Background))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CDamageBar_World::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	//_float fAlpha = { 0.1f };
	//if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &fAlpha, sizeof(_float))))
	//	return E_FAIL;

	return S_OK;
}

CDamageBar_World* CDamageBar_World::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDamageBar_World* pInstance = new CDamageBar_World(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDamageBar_World");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDamageBar_World::Clone(void* pArg)
{
	CDamageBar_World* pInstance = new CDamageBar_World(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDamageBar_World");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDamageBar_World::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	//Safe_Release(m_pTextureCom_Background);
	Safe_Release(m_pVIBufferCom);
}
