#include "Skydome.h"

#include "GameInstance.h"

CSkydome::CSkydome(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CSkydome::CSkydome(const CSkydome& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CSkydome::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkydome::Initialize(void* pArg)
{
	GAMEOBJECT_DESC			Desc{};
	Desc.fRotationPerSec = 0.f;
	Desc.fSpeedPerSec = 0.f;
	lstrcpy(Desc.szName, TEXT("Skydome"));

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Rotation(0.f, XMConvertToRadians(90.f), 0.f);

	return S_OK;
}


void CSkydome::Priority_Update(_float fTimeDelta)
{

}

void CSkydome::Update(_float fTimeDelta)
{


}

void CSkydome::Late_Update(_float fTimeDelta)
{
	_matrix mViewMatrix = m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW);

	// ViewMatrix 역행렬 → 카메라 월드행렬
	_matrix mInvViewMatrix = XMMatrixInverse(nullptr, mViewMatrix);

	// InvViewMatrix의 4번째 행이 Position
	_vector vCamPos = mInvViewMatrix.r[3]; // r[3] == m[3][0~3]

	// Y축 내리기
	vCamPos = XMVectorSetY(vCamPos, XMVectorGetY(vCamPos) - 0.1f);

	// Transform Position 세팅
	m_pTransformCom->Set_State(STATE::POSITION, vCamPos);

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_PRIORITY, this);
}

HRESULT CSkydome::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (m_pModelCom)
	{
		_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

		for (_uint i = 0; i < iNumMesh; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Begin(1)))
				return E_FAIL;

			if (FAILED(m_pModelCom->Render(i)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CSkydome::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Skydome"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkydome::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	return S_OK;
}

CSkydome* CSkydome::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSkydome* pInstance = new CSkydome(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSkydome");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkydome::Clone(void* pArg)
{
	CSkydome* pInstance = new CSkydome(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSkydome");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkydome::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
