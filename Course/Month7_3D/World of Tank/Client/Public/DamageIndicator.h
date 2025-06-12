#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CDamageIndicator final : public CUIObject
{
public:
	typedef struct tagDamageIndicator : public UIOBJECT_DESC
	{
		_float3 vFirePos = {};
	}DAMAGE_INDICATOR_DESC;

private:
	CDamageIndicator(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDamageIndicator(const CDamageIndicator& Prototype);
	virtual ~CDamageIndicator() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	void Rotate_FirePos();

private:
	_float3 m_vFirePos = {};
	_float	m_fTimeAcc = {};

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Components();

public:
	static CDamageIndicator* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END