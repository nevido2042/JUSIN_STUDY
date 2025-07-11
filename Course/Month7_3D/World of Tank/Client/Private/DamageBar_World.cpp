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

#pragma region 체력 업데이트
	if (m_pParent)
	{
		CTank* pTank = static_cast<CTank*>(m_pParent);
		m_iHP = static_cast<_int>(pTank->m_fHP);
		m_iMaxHP = static_cast<_int>(pTank->m_fMaxHP);
	}

#pragma endregion


#pragma region 데미지 딜레이 표현
	if (m_fTimeAcc > 0.5f)
	{
		//m_fFillDelay -= fTimeDelta * 0.3f;
		//if (m_fFillDelay < m_fFillAmount)
		//	m_fFillDelay = m_fFillAmount;

		m_fFillDelayValue += fTimeDelta * 0.8f;
		if (m_fFillDelayValue > 1.f)
			m_fFillDelayValue = 1.f;
	}
	else
	{
		m_fTimeAcc += fTimeDelta;
	}
#pragma endregion

}

void CDamageBar_World::Late_Update(_float fTimeDelta)
{

	_vector vScale = {};
	_vector vRot = {};
	_vector vWorldPos = {};
	XMMatrixDecompose(&vScale, &vRot, &vWorldPos, XMLoadFloat4x4(m_pParentWorldMatrix));

	vWorldPos += XMVectorSet(0.f, 5.f, 0.f, 0.f);

	_matrix matView = m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW);
	_matrix matProj = m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ);


	//_float3  m_vClip;
	_vector vClipPos = XMVector3TransformCoord(vWorldPos, matView * matProj);

	XMStoreFloat3(&m_vClip, vClipPos);

	// vClip은 NDC 좌표 (-1 ~ 1)
	if (m_vClip.x < -1.f || m_vClip.x > 1.f ||
		m_vClip.y < -1.f || m_vClip.y > 1.f ||
		m_vClip.z < 0.f || m_vClip.z > 1.f) // z는 0~1 사이
	{
		// 화면 밖이므로 표시하지 않음
		m_bVisible = false;
	}
	else
	{
		m_bVisible = true;

		_float2 vScreenPos;
		vScreenPos.x = (m_vClip.x * 0.5f) * g_iWinSizeX;
		vScreenPos.y = (m_vClip.y * 0.5f) * g_iWinSizeY;

		m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(vScreenPos.x, vScreenPos.y, m_fDepth, 1.f));
	}

	//_float2 vScreenPos;
	//vScreenPos.x = (vClip.x * 0.5f) * g_iWinSizeX;
	//vScreenPos.y = (vClip.y * 0.5f) * g_iWinSizeY;

	//m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(vScreenPos.x, vScreenPos.y, m_fDepth, 1.f));



	//_vector vScale = {};
	//_vector vRot = {};
	//_vector vWorldPos = {};
	//XMMatrixDecompose(&vScale, &vRot, &vWorldPos, XMLoadFloat4x4(m_pParentWorldMatrix));

	//vWorldPos += XMVectorSet(0.f, 5.f, 0.f, 0.f);

	//if(m_pGameInstance->Is_In_Frustum(vWorldPos, 1.f))
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);



}

HRESULT CDamageBar_World::Render()
{

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(4)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	wstring strText = to_wstring(m_iHP) + L"/" + to_wstring(m_iMaxHP);
	size_t strLength = strText.length();

	_float2 vScreenPos;
	vScreenPos.x = (m_vClip.x * 0.5f + 0.5f - strLength * 0.002f) * g_iWinSizeX;
	vScreenPos.y = (-m_vClip.y * 0.5f + 0.49f) * g_iWinSizeY;

	m_pGameInstance->Draw_Font(
		TEXT("Font_WarheliosKO"),
		strText.c_str(),
		vScreenPos,
		XMVectorSet(1.f, 1.f, 1.f, 1.f),
		0.f,
		_float2(0.f, 0.f),
		0.2f * UI_RATIO
	);

	//m_pGameInstance->Draw_Font(TEXT("Font_WarheliosKO"), TEXT("Font_WarheliosKO"), _float2(500.f, 500.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f), 100.f);
	//m_pGameInstance->Draw_Font(TEXT("Font_WarheliosKO"), TEXT("Font_WarheliosKO"), _float2(g_iWinSizeX * 0.45f, g_iWinSizeY * 0.5f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f), 0.3f);

	return S_OK;
}

void CDamageBar_World::Fill(_float fFillAmount)
{
	m_fTimeAcc = 0.f;
	m_fFillDelay = m_fFillAmount;
	m_fFillDelayValue = 0.f;

	m_fFillAmount = fFillAmount;
}

//void CDamageBar_World::Set_Text(_float fHP, _float fMaxHP)
//{
//	m_fHP = fHP;
//	m_fMaxHP = fMaxHP;
//}

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

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fFillDelayValue", &m_fFillDelayValue, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fFillDelay", &m_fFillDelay, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fFill", &m_fFillAmount, sizeof(_float))))
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
