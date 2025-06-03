#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CIcon_Consumables final : public CUIObject
{
public:
	typedef struct tagIcon_Consumables_Desc : public UIOBJECT_DESC
	{
		wstring			strTextureName = {};
		_uint			iKeyNumber = { 0 };
	}ICON_CONSUMABLES_DESC;

private:
	CIcon_Consumables(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CIcon_Consumables(const CIcon_Consumables& Prototype);
	virtual ~CIcon_Consumables() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	wstring			m_strTextureName = {};
	_uint			m_iKeyNumber = { 0 };

private:
	chrono::high_resolution_clock::time_point m_CanUseTime = {};

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pTextureCom_Background = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Components();

public:
	static CIcon_Consumables* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END