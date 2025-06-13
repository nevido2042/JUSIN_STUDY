#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CScore_Tank final : public CUIObject
{
public:
	typedef struct tagScoreTank : UIOBJECT_DESC
	{
		_tchar szTextureName[MAX_PATH] = {};
	}SCORE_TANK_DESC;
private:
	CScore_Tank(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CScore_Tank(const CScore_Tank& Prototype);
	virtual ~CScore_Tank() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Set_ColorDark();

private:
	_float4 m_vBaseColor = { 1.f, 1.f, 1.f, 1.f };

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Components(_tchar* szTextureName);
	HRESULT Ready_PartObjects();

public:
	static CScore_Tank* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END