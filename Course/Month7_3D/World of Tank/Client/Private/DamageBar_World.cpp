#include "DamageBar_World.h"

#include "GameInstance.h"
#include "Terrain.h"

CDamageBar_World::CDamageBar_World(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CDamageBar_World::CDamageBar_World(const CDamageBar_World& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CDamageBar_World::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDamageBar_World::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}


void CDamageBar_World::Priority_Update(_float fTimeDelta)
{
}

void CDamageBar_World::Update(_float fTimeDelta)
{

	//�ͷ������� ���� ��ŷ ��ġ �����ͼ� ��ġ��Ű��
	CTerrain* pTerrain = static_cast<CTerrain*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Terrain")));
	if (nullptr == pTerrain)
		return;

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&pTerrain->Get_PickedPos_Gun()), 1.f));

	m_pTransformCom->LookAt(XMLoadFloat4(m_pGameInstance->Get_CamPosition()));

}

void CDamageBar_World::Late_Update(_float fTimeDelta)
{
	//�Ÿ��� ���� �����ؼ�, �׻� ���� ũ�� ����ϵ���
	_vector vToCamera = XMLoadFloat4(m_pGameInstance->Get_CamPosition()) - m_pTransformCom->Get_State(STATE::POSITION);
	_float fDistance = XMVectorGetX(XMVector3Length(vToCamera));
	_float3 vFinalScale = { m_fBaseScale * fDistance, m_fBaseScale * fDistance, m_fBaseScale * fDistance };
	m_pTransformCom->Scaling(vFinalScale.x, vFinalScale.y, vFinalScale.z);


	//if (m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(STATE::POSITION), 10.f))
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);

}

HRESULT CDamageBar_World::Render()
{

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(2)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
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

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_DamageBar_World"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CDamageBar_World::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	_float fAlpha = { 0.1f };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &fAlpha, sizeof(_float))))
		return E_FAIL;

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
	Safe_Release(m_pVIBufferCom);
}
