#include "BackGround.h"

#include "Transform.h"
#include "GameInstance.h"

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject { pGraphic_Device }
{
}

CBackGround::CBackGround(const CBackGround& Prototype)
    : CGameObject { Prototype }
{
}

HRESULT CBackGround::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBackGround::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

    return S_OK;
}

void CBackGround::Priority_Update(_float fTimeDelta)
{
	int a = 10;
}

void CBackGround::Update(_float fTimeDelta)
{
	int a = 10;
}

void CBackGround::Late_Update(_float fTimeDelta)
{
	
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this)))
		return;
	
	
}

HRESULT CBackGround::Render()
{


	/* ��ġ�� �ؽ��ĸ� �ϳ� ������ ���´�. */
	/* ���� �������ϴµ� �̿��ϴ� ������ �ؽ���带 ��� �ִٸ� */
	/* �ش� �������� ���� ������ �ȼ����� ���ø��Ǳ����� �ؽ��Ĵ�. */
	if (FAILED(m_pTextureCom->Bind_Resource(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	/* ������ �׸���. */
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


    return S_OK;
}

HRESULT CBackGround::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */ 
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;
	

	

	return S_OK;
}

CBackGround* CBackGround::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBackGround* pInstance = new CBackGround(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBackGround::Clone(void* pArg)
{	
	CBackGround* pInstance = new CBackGround(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CBackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBackGround::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

}
