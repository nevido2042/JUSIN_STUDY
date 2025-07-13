#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CCountDamageModule final : public CUIObject
{
private:
	CCountDamageModule(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCountDamageModule(const CCountDamageModule& Prototype);
	virtual ~CCountDamageModule() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void AddCountDamageModule() {
		++m_iCountDamageModule;
	}
	_uint Get_CountDamageModule() const {
		return m_iCountDamageModule;
	}

private:
	_uint m_iCountDamageModule = { 0 };

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Components();

public:
	static CCountDamageModule* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END