#include "DirectionTurret.h"

#include "GameInstance.h"

CDirectionTurret::CDirectionTurret(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CDirectionTurret::CDirectionTurret(const CDirectionTurret& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CDirectionTurret::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDirectionTurret::Initialize(void* pArg)
{
	UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(STATE::POSITION, m_pTransformCom->Get_State(STATE::POSITION) + _vector{ 0.f, -0.05f, 0.f, 0.f });
	m_pTransformCom->Scaling(0.24f, 0.61f, 1.f);

	return S_OK;
}

void CDirectionTurret::Priority_Update(_float fTimeDelta)
{
	if (g_bWindowResizeRequired)
		Resize(g_iWinSizeX, g_iWinSizeY);

}

void CDirectionTurret::Update(_float fTimeDelta)
{
	//부모의 월드 행렬을 가져와서 자신의 월드 행렬과 곱해준다.
	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixMultiply(m_pTransformCom->Get_WorldMatrix(), XMLoadFloat4x4(m_pParentWorldMatrix)));

	CGameObject* pPlayerTank = m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_PlayerTank"));
	if (pPlayerTank == nullptr)
		return;

	//터렛 룩벡터
	_vector vTurretLook = pPlayerTank->Find_PartObject(TEXT("Part_Turret"))->Get_CombinedWorldMatrix().r[2];
	vTurretLook = XMVectorSetY(vTurretLook, 0.f);
	vTurretLook = XMVector3Normalize(vTurretLook);

	//바디 룩 벡터
	_vector vBodyLook = pPlayerTank->Get_Transform()->Get_State(STATE::LOOK);
	vBodyLook = XMVectorSetY(vBodyLook, 0.f);
	vBodyLook = XMVector3Normalize(vBodyLook); //* vTurretLook;

	// 카메라의 LOOK 벡터는 View 행렬의 Z축
	_matrix matViewInv = XMMatrixInverse(nullptr, m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW));
	_vector vCamLook = matViewInv.r[2]; // Z축
	vCamLook = XMVectorSetY(vCamLook, 0.f);
	vCamLook = XMVector3Normalize(vCamLook);

	// Dot: 각도
	_float fDot_Body_Turret = XMVectorGetX(XMVector3Dot(vTurretLook, vBodyLook));

	// Dot: 각도
	_float fDot_Body_Cam = XMVectorGetX(XMVector3Dot(vCamLook, vBodyLook));
	//_float fAngle = acosf(fDot + fDot2); // 항상 0~π (0~180도)

	_float fAngleTurretToBody = acosf(clamp(fDot_Body_Turret, -1.f, 1.f));
	_float fAngleCamToBody = acosf(clamp(fDot_Body_Cam, -1.f, 1.f));

	// Cross: 방향성
	_vector vCross_BodyTurret = XMVector3Cross(vBodyLook, vTurretLook);
	_float fCrossY_BodyTurret = XMVectorGetY(vCross_BodyTurret);

	// Cross: 방향성
	_vector vCross_CamBody = XMVector3Cross(vCamLook, vBodyLook);
	_float fCrossY_CamBody = XMVectorGetY(vCross_CamBody);

	// 방향 처리
	if (fCrossY_BodyTurret < 0.f)
		fAngleTurretToBody = -fAngleTurretToBody; // 시계 방향으로 회전

	// 방향 처리
	if (fCrossY_CamBody < 0.f)
		fAngleCamToBody = -fAngleCamToBody; // 시계 방향으로 회전

	_float fFinalAngle = fAngleCamToBody + fAngleTurretToBody;

	// 회전 수행 (Y축 기준)
	m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), -fFinalAngle);

}

void CDirectionTurret::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);

}

HRESULT CDirectionTurret::Render()
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

HRESULT CDirectionTurret::Ready_Components()
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_DirectionTurret"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CDirectionTurret* CDirectionTurret::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDirectionTurret* pInstance = new CDirectionTurret(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDirectionTurret");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDirectionTurret::Clone(void* pArg)
{
	CDirectionTurret* pInstance = new CDirectionTurret(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDirectionTurret");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDirectionTurret::Free()
{
	__super::Free();

	for (auto& Pair : m_PartObjects)
		Safe_Release(Pair.second);
	m_PartObjects.clear();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
