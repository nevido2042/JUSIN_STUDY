#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CScore final : public CUIObject
{
private:
	CScore(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CScore(const CScore& Prototype);
	virtual ~CScore() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	_uint m_iNumTeamA = {};
	_uint m_iNumTeamB = {};

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();

public:
	static CScore* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END