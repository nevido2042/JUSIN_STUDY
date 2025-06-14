#include "DirectionBody.h"

#include "GameInstance.h"

CDirectionBody::CDirectionBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CDirectionBody::CDirectionBody(const CDirectionBody& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CDirectionBody::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDirectionBody::Initialize(void* pArg)
{
	UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(STATE::POSITION, m_pTransformCom->Get_State(STATE::POSITION) + _vector{0.f, -0.05f, 0.f, 0.f});
	m_pTransformCom->Scaling(0.24f, 0.37f, 1.f);

	return S_OK;
}

void CDirectionBody::Priority_Update(_float fTimeDelta)
{
	if (g_bWindowResizeRequired)
		Resize(g_iWinSizeX, g_iWinSizeY);

}

void CDirectionBody::Update(_float fTimeDelta)
{
	//부모의 월드 행렬을 가져와서 자신의 월드 행렬과 곱해준다.
	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixMultiply(m_pTransformCom->Get_WorldMatrix(), XMLoadFloat4x4(m_pParentWorldMatrix)));

	CGameObject* pPlayerTank = m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_PlayerTank"));
	if (pPlayerTank == nullptr)
		return;

	// 탱크 룩 벡터
	_vector vTankLook = pPlayerTank->Get_Transform()->Get_State(STATE::LOOK);
	vTankLook = XMVectorSetY(vTankLook, 0.f);
	vTankLook = XMVector3Normalize(vTankLook);

	// 카메라의 LOOK 벡터는 View 행렬의 Z축
	_matrix matViewInv = XMMatrixInverse(nullptr, m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW));
	_vector vCamLook = matViewInv.r[2]; // Z축
	vCamLook = XMVectorSetY(vCamLook, 0.f);
	vCamLook = XMVector3Normalize(vCamLook);

	// Dot: 각도
	_float fDot = XMVectorGetX(XMVector3Dot(vTankLook, vCamLook));
	_float fAngle = acosf(fDot); // 항상 0~π (0~180도)

	// Cross: 방향성
	_vector vCross = XMVector3Cross(vTankLook, vCamLook);
	_float fCrossY = XMVectorGetY(vCross);

	// 방향 처리
	if (fCrossY < 0.f)
		fAngle = -fAngle; // 시계 방향으로 회전

	// 회전 수행 (Y축 기준)
	m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), fAngle);

}

void CDirectionBody::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);

}

HRESULT CDirectionBody::Render()
{
	//파트 오브젝트는 자기 트랜스폼 안써야한다
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	_float fAlpha = { 0.05f };
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

HRESULT CDirectionBody::Ready_Components()
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_DirectionBody"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CDirectionBody* CDirectionBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDirectionBody* pInstance = new CDirectionBody(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDirectionBody");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDirectionBody::Clone(void* pArg)
{
	CDirectionBody* pInstance = new CDirectionBody(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDirectionBody");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDirectionBody::Free()
{
	__super::Free();

	for (auto& Pair : m_PartObjects)
		Safe_Release(Pair.second);
	m_PartObjects.clear();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
