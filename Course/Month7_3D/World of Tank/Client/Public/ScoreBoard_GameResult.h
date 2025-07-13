#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CScoreBoard_GameResult final : public CUIObject
{
public:
	typedef struct tagGameResultDesc : public tagUIObjectDesc
	{
		_bool bIsVictory = { false };
	}GAMERESULT_DESC;

private:
	CScoreBoard_GameResult(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CScoreBoard_GameResult(const CScoreBoard_GameResult& Prototype);
	virtual ~CScoreBoard_GameResult() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	_bool m_bIsVictory = { false };

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Components();

public:
	static CScoreBoard_GameResult* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END