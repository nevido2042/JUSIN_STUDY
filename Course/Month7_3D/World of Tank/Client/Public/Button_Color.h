#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CButton_Color final : public CUIObject
{
public:
	typedef struct tagButtonColor : public UIOBJECT_DESC
	{
		wstring strTextureName = {};
		_float4 vBaseColor = {};
	}BUTTON_COLOR_DESC;

private:
	CButton_Color(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CButton_Color(const CButton_Color& Prototype);
	virtual ~CButton_Color() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	wstring m_strTextureName = {};
	_float4 m_vBaseColor = {};

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };


private:
	HRESULT Ready_Components();

public:
	static CButton_Color* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END