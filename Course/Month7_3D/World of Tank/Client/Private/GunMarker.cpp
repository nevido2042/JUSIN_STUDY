#include "GunMarker.h"

#include "GameInstance.h"
//#include "Terrain.h"
#include "PickedManager.h"

CGunMarker::CGunMarker(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CGunMarker::CGunMarker(const CGunMarker& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CGunMarker::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGunMarker::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}


void CGunMarker::Priority_Update(_float fTimeDelta)
{

}

void CGunMarker::Update(_float fTimeDelta)
{


}

void CGunMarker::Late_Update(_float fTimeDelta)
{

	_float3 vGunPickedPos = {};
	CPickedManager* pPickedManager = static_cast<CPickedManager*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_PickedManager")));
	if (pPickedManager)
		vGunPickedPos = pPickedManager->Get_GunPickedPos();

	_matrix matView = m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW);
	_matrix matProj = m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ);

	_vector vClipPos = XMVector3TransformCoord(XMVectorSetW(XMLoadFloat3(&vGunPickedPos), 1.f), matView * matProj);

	_float3 vClip;
	XMStoreFloat3(&vClip, vClipPos);

	_float2 vScreenPos;
	vScreenPos.x = (vClip.x * 0.5f) * g_iWinSizeX;
	vScreenPos.y = (vClip.y * 0.5f) * g_iWinSizeY;

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(vScreenPos.x, vScreenPos.y, m_fDepth, 1.f));

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);

}

HRESULT CGunMarker::Render()
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

HRESULT CGunMarker::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_GunMarker"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CGunMarker::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	_float fAlpha = { 0.1f };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &fAlpha, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CGunMarker* CGunMarker::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGunMarker* pInstance = new CGunMarker(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGunMarker");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGunMarker::Clone(void* pArg)
{
	CGunMarker* pInstance = new CGunMarker(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGunMarker");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGunMarker::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
