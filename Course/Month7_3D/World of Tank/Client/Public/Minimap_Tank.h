#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CMinimap_Tank final : public CUIObject
{
public:
	typedef struct tagMinimapTankDesc : public UIOBJECT_DESC
	{
		CGameObject* pTarget = { nullptr };
		_tchar szTextureName[MAX_PATH] = {};
	}MINIMAP_TANK_DESC;
private:
	CMinimap_Tank(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMinimap_Tank(const CMinimap_Tank& Prototype);
	virtual ~CMinimap_Tank() = default;

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
	HRESULT Ready_Components(_tchar* szTextureName);

public:
	static CMinimap_Tank* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END