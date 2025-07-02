#include "Camera.h"
#include "GameInstance.h"

CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject { pDevice, pContext }
{
}

CCamera::CCamera(const CCamera& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CCamera::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CCamera::Initialize(void* pArg)
{
	CAMERA_DESC*		pDesc = static_cast<CAMERA_DESC*>(pArg);

	m_fFov = pDesc->fFov;
	m_fNear = pDesc->fNear;
	m_fFar = pDesc->fFar;

	D3D11_VIEWPORT			ViewportDesc{};
	_uint					iNumViewports = { 1 };
	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	m_fAspect = static_cast<_float>(ViewportDesc.Width) / ViewportDesc.Height;
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&pDesc->vEye), 1.f));
	m_pTransformCom->LookAt(XMVectorSetW(XMLoadFloat3(&pDesc->vAt), 1.f));

	return S_OK;
}

void CCamera::Priority_Update(_float fTimeDelta)
{
}

void CCamera::Update(_float fTimeDelta)
{
}

void CCamera::Late_Update(_float fTimeDelta)
{
}

HRESULT CCamera::Render()
{
	return S_OK;
}

void CCamera::Clip_Cursor()
{
	// 마우스 중앙으로 위치
	POINT ptCenter = { static_cast<LONG>(g_iWinSizeX / 2), static_cast<LONG>(g_iWinSizeY / 2) };
	ClientToScreen(g_hWnd, &ptCenter);
	SetCursorPos(ptCenter.x, ptCenter.y);

	// 마우스 이동 제한
	RECT rcClient;
	GetClientRect(g_hWnd, &rcClient);
	POINT ptLT = { rcClient.left, rcClient.top };
	POINT ptRB = { rcClient.right, rcClient.bottom };
	ClientToScreen(g_hWnd, &ptLT);
	ClientToScreen(g_hWnd, &ptRB);
	RECT rcClip = { ptLT.x, ptLT.y, ptRB.x, ptRB.y };
	ClipCursor(&rcClip);
}


void CCamera::Bind_Matrices()
{
	m_pGameInstance->Set_Transform(D3DTS::VIEW, m_pTransformCom->Get_WorldMatrix_Inverse());
	m_pGameInstance->Set_Transform(D3DTS::PROJ, XMMatrixPerspectiveFovLH(m_fFov, m_fAspect, m_fNear, m_fFar));
	m_pGameInstance->Set_Fov(m_fFov);
	//m_pGameInstance->Save_Near(m_fNear);
	//m_pGameInstance->Save_Far(m_fFar);
}

void CCamera::Free()
{
	__super::Free();
}
