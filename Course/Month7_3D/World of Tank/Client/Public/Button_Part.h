#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CButton_Part final : public CUIObject
{
public:
	typedef struct tagButtonPart : public UIOBJECT_DESC
	{
		wstring			strTextureName = {};
		PART_REPAINT	ePartRepaint = { PART_REPAINT::END };
	}BUTTON_PART_DESC;

private:
	CButton_Part(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CButton_Part(const CButton_Part& Prototype);
	virtual ~CButton_Part() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	wstring			m_strTextureName = {};
	PART_REPAINT	m_ePartRepaint = {};

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };


private:
	HRESULT Ready_Components();

public:
	static CButton_Part* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END