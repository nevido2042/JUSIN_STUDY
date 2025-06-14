#include "Minimap_Tank.h"

#include "GameInstance.h"

CMinimap_Tank::CMinimap_Tank(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CMinimap_Tank::CMinimap_Tank(const CMinimap_Tank& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CMinimap_Tank::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMinimap_Tank::Initialize(void* pArg)
{
	MINIMAP_TANK_DESC* pDesc = static_cast<MINIMAP_TANK_DESC*>(pArg);
	m_pTarget = pDesc->pTarget;
	if (m_pTarget == nullptr)
		return E_FAIL;
	Safe_AddRef(m_pTarget);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pDesc->szTextureName)))
		return E_FAIL;

	m_pTransformCom->Scaling(0.2f, 0.2f, 0.2f);

	return S_OK;
}

void CMinimap_Tank::Priority_Update(_float fTimeDelta)
{
	if (g_bWindowResizeRequired)
		Resize(g_iWinSizeX, g_iWinSizeY);
}

void CMinimap_Tank::Update(_float fTimeDelta)
{
	CTransform* pPlayerTransform = m_pTarget->Get_Transform();

	_vector vPlayerPos = pPlayerTransform->Get_State(STATE::POSITION);
	_vector vIconPos = XMVectorSet(
		(-0.5f + (XMVectorGetX(vPlayerPos) / (TERRAIN_SIZE * TERRAIN_OFFSET_WIDTH))) * 1.3f,
		(0.5f - (XMVectorGetZ(vPlayerPos) / (TERRAIN_SIZE * TERRAIN_OFFSET_WIDTH))) * 1.3f,
		m_fDepth,
		1.f);
	m_pTransformCom->Set_State(STATE::POSITION, vIconPos);

	//부모의 월드 행렬을 가져와서 자신의 월드 행렬과 곱해준다.
	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixMultiply(m_pTransformCom->Get_WorldMatrix(), XMLoadFloat4x4(m_pParentWorldMatrix)));
}

void CMinimap_Tank::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CMinimap_Tank::Render()
{
	//파트 오브젝트는 자기 트랜스폼 안써야한다
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	_float fAlpha = { 0.9f };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMinimap_Tank::Ready_Components(_tchar* szTextureName)
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), szTextureName,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CMinimap_Tank* CMinimap_Tank::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMinimap_Tank* pInstance = new CMinimap_Tank(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMinimap_Tank");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMinimap_Tank::Clone(void* pArg)
{
	CMinimap_Tank* pInstance = new CMinimap_Tank(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMinimap_Tank");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMinimap_Tank::Free()
{
	__super::Free();

	Safe_Release(m_pTarget);

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
