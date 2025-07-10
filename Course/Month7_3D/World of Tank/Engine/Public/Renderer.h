#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CRenderer final : public CBase
{
private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	HRESULT Initialize();
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
	HRESULT Draw();

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	class CGameInstance*	m_pGameInstance = { nullptr };

private:
	class CVIBuffer_Rect*	m_pVIBuffer = { nullptr };
	class CShader*			m_pShader = { nullptr };

	_float4x4				m_WorldMatrix{}, m_ViewMatrix{}, m_ProjMatrix{};

	ID3D11DepthStencilView* m_pShadowDSV = { nullptr };
	//_uint					m_iOriginalViewportWidth{}, m_iOriginalViewportHeight{};

private:
	list<class CGameObject*>	m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_END)];

private:
	/*const _uint m_iMaxWidth = 16384;
	const _uint m_iMaxHeight = 9216;	*/

	const _uint m_iMaxWidth = 8192;
	const _uint m_iMaxHeight = 4608;

	/*const _uint m_iMaxWidth = 4096;
	const _uint m_iMaxHeight = 2304;*/

	//const _uint m_iMaxWidth = 1280;
	//const _uint m_iMaxHeight = 720;

	/*const _uint m_iMaxWidth = 2560;
	const _uint m_iMaxHeight = 1440;*/

#ifdef _DEBUG
private:
	list<class CComponent*>		m_DebugComponent;
#endif

#ifdef _DEBUG
public:
	HRESULT Add_DebugComponent(class CComponent* pDebugCom);
#endif

private:
	HRESULT Render_Priority();
	HRESULT Render_Outline();
	HRESULT Render_Shadow();
	HRESULT Render_NonBlend();
	HRESULT Render_Lights();
	HRESULT Render_BackBuffer();

	HRESULT Render_NonLight();
	HRESULT Render_Blend_First();
	HRESULT Render_Blend();
	HRESULT Render_UI();

private:
	HRESULT Ready_DepthStencilView(_uint iWidth, _uint iHeight);
	HRESULT Change_ViewportDesc(_uint iWidth, _uint iHeight);

#ifdef _DEBUG
private:
	HRESULT Render_Debug();
#endif

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

NS_END