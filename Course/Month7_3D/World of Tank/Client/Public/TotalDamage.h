#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CTotalDamage final : public CUIObject
{
private:
	CTotalDamage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTotalDamage(const CTotalDamage& Prototype);
	virtual ~CTotalDamage() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void AddDamage(_float fDamage) {
		m_fTotalDamage += fDamage;
	}
	_float Get_TotalDamage() const {
		return m_fTotalDamage;
	}

private:
	_float m_fTotalDamage = { 0.f };

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Components();

public:
	static CTotalDamage* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END