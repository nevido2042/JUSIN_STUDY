#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CMediumTank_Enemy_Red final : public CUIObject
{
public:
	typedef struct tagMediumTank_Enemy_Red : public UIOBJECT_DESC
	{
		CGameObject* pTarget = { nullptr };
	}MEDIUM_TANK_ENEMY_RED_DESC;
private:
	CMediumTank_Enemy_Red(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMediumTank_Enemy_Red(const CMediumTank_Enemy_Red& Prototype);
	virtual ~CMediumTank_Enemy_Red() = default;

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
	static CMediumTank_Enemy_Red* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END