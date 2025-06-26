#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Point_Instance;
NS_END

NS_BEGIN(Client)

class CBaseParticle final : public CGameObject
{
private:
	CBaseParticle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBaseParticle(const CBaseParticle& Prototype);
	virtual ~CBaseParticle() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Change_Texture(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag);
	void	Change_ShaderPass(_uint iShaderPass) {
		m_iShaderPass = iShaderPass;
	}
private:
	_uint m_iShaderPass = {0};

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Point_Instance* m_pVIBufferCom = { nullptr };
private:
	const _float4x4* m_pSocketMatrix = { nullptr };
	const _uint* m_pParentState = { nullptr };

private:
	HRESULT Ready_Components();

public:
	static CBaseParticle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END