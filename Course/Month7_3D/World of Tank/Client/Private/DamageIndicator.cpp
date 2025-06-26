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
	// 카메라가 보는 방향 정보(뷰 행렬)랑
	// 화면에 어떻게 보일지 계산하는 정보(투영 행렬)를 가져온다
	_matrix matView = m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW);
	_matrix matProj = m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ);

	// 총알이나 공격이 날아온 위치를 불러온다 (게임 세상 안의 좌표)
	_vector vWorldPos = XMLoadFloat3(&m_vFirePos);

	// 이 위치를 카메라 기준으로 바꾼다 (카메라가 중심인 좌표로)
	_vector vViewPos = XMVector3TransformCoord(vWorldPos, matView);

	// 이걸 다시 화면 기준으로 바꾼다 ([-1, 1] 사이 값으로 변환됨)
	_vector vProjPos = XMVector3TransformCoord(vViewPos, matProj);

	// 이제 화면 픽셀 좌표로 바꿔준다
	_float3 vScreenPos;
	vScreenPos.x = (XMVectorGetX(vProjPos) + 1.f) * 0.5f * g_iWinSizeX; // X 좌표 0~화면 너비
	vScreenPos.y = (1.f - XMVectorGetY(vProjPos)) * 0.5f * g_iWinSizeY; // Y 좌표 위아래 반전
	vScreenPos.z = XMVectorGetZ(vProjPos); // Z값은 안 써도 된다

	// 카메라 위치를 가져온다
	_vector vCamPos = XMLoadFloat4(m_pGameInstance->Get_CamPosition());

	// 공격 위치에서 카메라까지 방향 벡터를 구한다
	_vector vDirWorld = vWorldPos - vCamPos;

	// 이 방향도 카메라 기준으로 바꾼다
	_vector vDirView = XMVector3TransformNormal(vDirWorld, matView);

	// XZ 평면 기준으로 각도를 구한다
	_float fRotation = atan2f(XMVectorGetX(vDirView), XMVectorGetZ(vDirView));

	// 만약 공격이 카메라 뒤쪽에서 왔다면 180도 돌려준다
	if (XMVectorGetZ(vProjPos) < 0.f)
		fRotation += XM_PI;

	// 구한 각도로 UI를 돌린다
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
