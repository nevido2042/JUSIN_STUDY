#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CButton_3DCustom final : public CUIObject
{
public:
	typedef struct tagButton3DCustom : public UIOBJECT_DESC
	{
		wstring strTextureName = {};
		CUSTOM3D e3DCustom = {};
	}BUTTON_3DCUSTOM_DESC;

private:
	CButton_3DCustom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CButton_3DCustom(const CButton_3DCustom& Prototype);
	virtual ~CButton_3DCustom() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	wstring m_strTextureName = {};
	CUSTOM3D m_e3DCustom = { CUSTOM3D::END };

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };


private:
	HRESULT Ready_Components();

public:
	static CButton_3DCustom* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END