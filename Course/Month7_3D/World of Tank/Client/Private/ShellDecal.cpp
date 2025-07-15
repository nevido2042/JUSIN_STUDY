#include "ShellDecal.h"

#include "GameInstance.h"

CShellDecal::CShellDecal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CShellDecal::CShellDecal(const CShellDecal& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CShellDecal::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CShellDecal::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	SHELLDECAL_DESC* pShellDecalDesc = static_cast<SHELLDECAL_DESC*>(pArg);

	if (pShellDecalDesc->bIsGround)
	{
		//m_pTransformCom->LookAt(XMVectorSet(300.f, 1000.f, 300.f, 1.f));

		_vector vPos = m_pTransformCom->Get_State(STATE::POSITION);
		vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + 100.f);
		vPos = XMVectorSetX(vPos, XMVectorGetX(vPos) + 0.01f);//짐벌락때문에 약간 수정
		m_pTransformCom->LookAt(vPos); //위를 바라보도록
		m_pTransformCom->Scaling(3.f, 3.f, SHELL_DIG_DEPTH);
	}
	else
	{
		m_pTransformCom->LookAt(XMVectorSetW(XMLoadFloat3(&pShellDecalDesc->vFirePos), 1.f));
		m_pTransformCom->Scaling(3.f, 3.f, 1.f);
	}



	return S_OK;
}


void CShellDecal::Priority_Update(_float fTimeDelta)
{

}

void CShellDecal::Update(_float fTimeDelta)
{
	m_fAccTime += fTimeDelta;
	if(m_fAccTime > 3.f)
	{
		Destroy();
	}
}

void CShellDecal::Late_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(STATE::POSITION), 5.f))
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_DECAL, this);

}

HRESULT CShellDecal::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	//if (FAILED(m_pShaderCom->Begin(1)))
	//	return E_FAIL;

	//if (FAILED(m_pVIBufferCom->Bind_Buffers()))
	//	return E_FAIL;

	//if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShellDecal::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPos"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_VolumeMesh"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_ShellHole"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CShellDecal::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Depth"), m_pShaderCom, "g_DepthTexture")))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	_float4x4 WorldMatrixInv = {};
	XMStoreFloat4x4(&WorldMatrixInv, m_pTransformCom->Get_WorldMatrix_Inverse());
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrixInv", &WorldMatrixInv)))
		return E_FAIL;

	_float4x4 ViewMatrixInv = {};
	XMStoreFloat4x4(&ViewMatrixInv, m_pGameInstance->Get_Transform_Matrix_Inv(D3DTS::VIEW));
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrixInv", &ViewMatrixInv)))
		return E_FAIL;

	_float4x4 ProjMatrixInv = {};
	XMStoreFloat4x4(&ProjMatrixInv, m_pGameInstance->Get_Transform_Matrix_Inv(D3DTS::PROJ));
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrixInv", &ProjMatrixInv)))
		return E_FAIL;

	//matrix g_ProjMatrixInv;
	//matrix g_ViewMatrixInv;

	return S_OK;
}

CShellDecal* CShellDecal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CShellDecal* pInstance = new CShellDecal(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CShellDecal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CShellDecal::Clone(void* pArg)
{
	CShellDecal* pInstance = new CShellDecal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CShellDecal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShellDecal::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
