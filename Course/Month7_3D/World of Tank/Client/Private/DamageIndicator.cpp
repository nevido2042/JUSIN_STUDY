#include "DamageIndicator.h"

#include "GameInstance.h"

CDamageIndicator::CDamageIndicator(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CDamageIndicator::CDamageIndicator(const CDamageIndicator& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CDamageIndicator::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDamageIndicator::Initialize(void* pArg)
{
	DAMAGE_INDICATOR_DESC* pDesc = static_cast<DAMAGE_INDICATOR_DESC*>(pArg);

	m_vFirePos = pDesc->vFirePos;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

void CDamageIndicator::Priority_Update(_float fTimeDelta)
{
	if (g_bWindowResizeRequired)
		Resize(g_iWinSizeX, g_iWinSizeY);
}

void CDamageIndicator::Update(_float fTimeDelta)
{
	Rotate_FirePos();
	
	m_fTimeAcc += fTimeDelta;

	if (m_fTimeAcc > 3.f)
	{
		Destroy();
	}
}




void CDamageIndicator::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CDamageIndicator::Render()
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

void CDamageIndicator::Rotate_FirePos()
{
	// ī�޶� ���� ���� ����(�� ���)��
	// ȭ�鿡 ��� ������ ����ϴ� ����(���� ���)�� �����´�
	_matrix matView = m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW);
	_matrix matProj = m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ);

	// �Ѿ��̳� ������ ���ƿ� ��ġ�� �ҷ��´� (���� ���� ���� ��ǥ)
	_vector vWorldPos = XMLoadFloat3(&m_vFirePos);

	// �� ��ġ�� ī�޶� �������� �ٲ۴� (ī�޶� �߽��� ��ǥ��)
	_vector vViewPos = XMVector3TransformCoord(vWorldPos, matView);

	// �̰� �ٽ� ȭ�� �������� �ٲ۴� ([-1, 1] ���� ������ ��ȯ��)
	_vector vProjPos = XMVector3TransformCoord(vViewPos, matProj);

	// ���� ȭ�� �ȼ� ��ǥ�� �ٲ��ش�
	_float3 vScreenPos;
	vScreenPos.x = (XMVectorGetX(vProjPos) + 1.f) * 0.5f * g_iWinSizeX; // X ��ǥ 0~ȭ�� �ʺ�
	vScreenPos.y = (1.f - XMVectorGetY(vProjPos)) * 0.5f * g_iWinSizeY; // Y ��ǥ ���Ʒ� ����
	vScreenPos.z = XMVectorGetZ(vProjPos); // Z���� �� �ᵵ �ȴ�

	// ī�޶� ��ġ�� �����´�
	_vector vCamPos = XMLoadFloat4(m_pGameInstance->Get_CamPosition());

	// ���� ��ġ���� ī�޶���� ���� ���͸� ���Ѵ�
	_vector vDirWorld = vWorldPos - vCamPos;

	// �� ���⵵ ī�޶� �������� �ٲ۴�
	_vector vDirView = XMVector3TransformNormal(vDirWorld, matView);

	// XZ ��� �������� ������ ���Ѵ�
	_float fRotation = atan2f(XMVectorGetX(vDirView), XMVectorGetZ(vDirView));

	// ���� ������ ī�޶� ���ʿ��� �Դٸ� 180�� �����ش�
	if (XMVectorGetZ(vProjPos) < 0.f)
		fRotation += XM_PI;

	// ���� ������ UI�� ������
	m_pTransformCom->Rotation(0.f, 0.f, -fRotation);
}

HRESULT CDamageIndicator::Ready_Components()
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_DamageBig"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CDamageIndicator* CDamageIndicator::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDamageIndicator* pInstance = new CDamageIndicator(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDamageIndicator");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDamageIndicator::Clone(void* pArg)
{
	CDamageIndicator* pInstance = new CDamageIndicator(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDamageIndicator");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDamageIndicator::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
