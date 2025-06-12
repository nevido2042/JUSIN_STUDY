#include "DamageBig.h"

#include "GameInstance.h"

CDamageBig::CDamageBig(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CDamageBig::CDamageBig(const CDamageBig& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CDamageBig::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDamageBig::Initialize(void* pArg)
{
	UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(0.5f, 0.5f, 0.5f);


	_vector vPos = m_pTransformCom->Get_State(STATE::POSITION);
	m_pTransformCom->Set_State(STATE::POSITION, vPos + _vector{0.f, 0.3f, 0.f, 0.f});

	//공격한 적의 위치 (일단  0 0 0)를 저장하고

	

	//매프레임
	//적의 위치를 2d화면상의 위치로 변환
	//카메라 중점의 화면상 위치 와 위의 위치의 방향 벡터를 구한다.
	//그만큼 회전시킨다

	// Transform 컴포넌트가 제공하는 로컬 회전
	m_pTransformCom->Rotation(_vector{ 0.f, 0.f, 1.f, 0.f }, XMConvertToRadians(90.f));

	//// 이후 Update에서 부모와 곱함
	//_matrix matWorld = m_pTransformCom->Get_WorldMatrix();
	//_matrix matParentWorld = XMLoadFloat4x4(m_pParentWorldMatrix);

	//XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixMultiply(matWorld, matParentWorld));


	return S_OK;
}

void CDamageBig::Priority_Update(_float fTimeDelta)
{
	if (g_bWindowResizeRequired)
		Resize(g_iWinSizeX, g_iWinSizeY);
}

void CDamageBig::Update(_float fTimeDelta)
{
	//부모의 월드 행렬을 가져와서 자신의 월드 행렬과 곱해준다.
	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixMultiply(m_pTransformCom->Get_WorldMatrix(), XMLoadFloat4x4(m_pParentWorldMatrix)));
}

void CDamageBig::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CDamageBig::Render()
{
	//파트 오브젝트는 자기 트랜스폼 안써야한다
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	_float fAlpha = { 0.1f };
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

HRESULT CDamageBig::Ready_Components()
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

CDamageBig* CDamageBig::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDamageBig* pInstance = new CDamageBig(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDamageBig");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDamageBig::Clone(void* pArg)
{
	CDamageBig* pInstance = new CDamageBig(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDamageBig");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDamageBig::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
