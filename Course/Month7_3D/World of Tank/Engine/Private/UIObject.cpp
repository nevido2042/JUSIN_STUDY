#include "UIObject.h"

CUIObject::CUIObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject { pDevice, pContext }
{
}

CUIObject::CUIObject(const CUIObject& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CUIObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIObject::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return S_OK;

	UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

	m_fX = pDesc->fX;
	m_fY = pDesc->fY;
	m_fDepth = pDesc->fDepth;
	m_fSizeX = pDesc->fSizeX;
	m_fSizeY = pDesc->fSizeY;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	D3D11_VIEWPORT			ViewportDesc{};
	_uint					iNumViewports = { 1 };

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);
	
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.0f, 1.f));

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY);

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_fX - ViewportDesc.Width * 0.5f, -m_fY + ViewportDesc.Height * 0.5f, m_fDepth, 1.f));


	m_fXRatio = m_fX / ViewportDesc.Width;
	m_fYRatio = m_fY / ViewportDesc.Height;
	m_fSizeXRatio = m_fSizeX / ViewportDesc.Width;
	m_fSizeYRatio = m_fSizeY / ViewportDesc.Height;

	return S_OK;
}

void CUIObject::Priority_Update(_float fTimeDelta)
{
	if (g_bWindowResizeRequired)
		Resize(g_iWinSizeX, g_iWinSizeY);

}

void CUIObject::Update(_float fTimeDelta)
{
}

void CUIObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CUIObject::Render()
{

	return S_OK;
}

HRESULT CUIObject::Resize(_uint iNewWidth, _uint iNewHeight)
{
	_float fWidth = static_cast<_float>(iNewWidth);
	_float fHeight = static_cast<_float>(iNewHeight);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(fWidth, fHeight, 0.f, 1.f));

	// 비율을 기반으로 새 위치 및 크기 계산
	m_fX = m_fXRatio * fWidth;
	m_fY = m_fYRatio * fHeight;
	m_fSizeX = m_fSizeXRatio * fWidth;
	m_fSizeY = m_fSizeYRatio * fHeight;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY);
	m_pTransformCom->Set_State(
		STATE::POSITION,
		XMVectorSet(
			m_fX - fWidth * 0.5f,
			-m_fY + fHeight * 0.5f,
			m_fDepth,
			1.f
		)
	);

	return S_OK;
}

_bool CUIObject::isPick(HWND hWnd)
{
	POINT			ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	RECT			rcUI = {
		static_cast<LONG>(m_fX - m_fSizeX * 0.5f),
		static_cast<LONG>(m_fY - m_fSizeY * 0.5f),
		static_cast<LONG>(m_fX + m_fSizeX * 0.5f),
		static_cast<LONG>(m_fY + m_fSizeY * 0.5f) };

	return PtInRect(&rcUI, ptMouse);
}

void CUIObject::Free()
{
	__super::Free();


}
