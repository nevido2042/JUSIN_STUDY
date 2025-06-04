#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CIcon_Module final : public CUIObject
{
public:
	typedef struct tagIconModuleDesc : public UIOBJECT_DESC
	{
		wstring				strTextureName = {};
	}ICON_MODULE_DESC;

private:
	CIcon_Module(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CIcon_Module(const CIcon_Module& Prototype);
	virtual ~CIcon_Module() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void	Set_ModuleState(MODULE_STATE eModuleState) { m_eModuleState = eModuleState; }

private:
	MODULE_STATE		m_eModuleState = { MODULE_STATE::FUNCTIONAL };
	wstring				m_strTextureName = {};

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Components();

public:
	static CIcon_Module* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END