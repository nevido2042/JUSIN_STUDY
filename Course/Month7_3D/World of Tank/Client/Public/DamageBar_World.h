#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CDamageBar_World final : public CGameObject
{
private:
	CDamageBar_World(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDamageBar_World(const CDamageBar_World& Prototype);
	virtual ~CDamageBar_World() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void	Fill(_float fFillAmount);

private:
	_float	m_fFillAmount = { 1.f };

private:
	const _float m_fBaseScale = 0.03f;

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pTextureCom_Background = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CDamageBar_World* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END