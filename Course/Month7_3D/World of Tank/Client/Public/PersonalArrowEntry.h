#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CPersonalArrowEntry final : public CUIObject
{
public:
	typedef struct tagPersonalArrowEntry : public UIOBJECT_DESC
	{
		CGameObject* pTarget = { nullptr };
	}PERSONAL_ARROW_ENTRY_DESC;

private:
	CPersonalArrowEntry(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPersonalArrowEntry(const CPersonalArrowEntry& Prototype);
	virtual ~CPersonalArrowEntry() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CGameObject* m_pTarget = { nullptr };

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Components();

public:
	static CPersonalArrowEntry* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END