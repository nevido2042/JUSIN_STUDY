#include "DeadSmoke.h"

#include "GameInstance.h"

CDeadSmoke::CDeadSmoke(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CDeadSmoke::CDeadSmoke(const CDeadSmoke& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CDeadSmoke::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDeadSmoke::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CDeadSmoke::Priority_Update(_float fTimeDelta)
{
	//if (m_pGameInstance->Key_Pressing(DIK_1))
	//{
	//	m_pVIBufferCom->Change_NumInstance(100 + rand() % 50);
	//}

}

void CDeadSmoke::Update(_float fTimeDelta)
{
	//���� Ŀ����
	m_fSize += fTimeDelta * 0.1f;
	if (m_fSize > 1.f)
		m_fSize = 1.f;
	m_pTransformCom->Scaling(m_fSize, m_fSize, m_fSize);

	//�θ��� ���� ����� �����ͼ� �ڽ��� ���� ��İ� �����ش�.
	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixMultiply(m_pTransformCom->Get_WorldMatrix(), XMLoadFloat4x4(m_pParentWorldMatrix)));

	m_pVIBufferCom->Emission(fTimeDelta);
	//m_pVIBufferCom->Drop(fTimeDelta);
	//m_pVIBufferCom->Spread(fTimeDelta);

}

void CDeadSmoke::Late_Update(_float fTimeDelta)
{
	// ��ġ�� ����
	_vector vPos = XMVectorSet(
		m_CombinedWorldMatrix._41,
		m_CombinedWorldMatrix._42,
		m_CombinedWorldMatrix._43,
		1.f
	);

	// �������� �ȿ� ������ ������ �߰�
	if (m_pGameInstance->Is_In_Frustum(vPos, 0.1f))
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONLIGHT, this);


	/* WeightBlend */
	//m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CDeadSmoke::Render()
{

	//if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
	//	return E_FAIL;

	//��Ʈ ������Ʈ�� �ڱ� Ʈ������ �Ƚ���Ѵ�
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpah", &m_pVIBufferCom->Get_Desc().fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(1)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDeadSmoke::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosInstance"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_DeadSmoke"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_DeadSmoke"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CDeadSmoke* CDeadSmoke::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDeadSmoke* pInstance = new CDeadSmoke(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDeadSmoke");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDeadSmoke::Clone(void* pArg)
{
	CDeadSmoke* pInstance = new CDeadSmoke(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDeadSmoke");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDeadSmoke::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
