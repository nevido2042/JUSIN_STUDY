#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CScoreBoard_TotalDamage final : public CUIObject
{
public:
	typedef struct TotalDamageDesc : public tagUIObjectDesc
	{
		_float fTotalDamage = { 0.f };
	}TOTALDAMAGE_DESC;

private:
	CScoreBoard_TotalDamage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CScoreBoard_TotalDamage(const CScoreBoard_TotalDamage& Prototype);
	virtual ~CScoreBoard_TotalDamage() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();
private:
	_float m_fTotalDamage = { 0.f };

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Components();

public:
	static CScoreBoard_TotalDamage* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END