#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CDamageBar_World final : public CUIObject
{
public:
	typedef struct tagDamageBar_World_Desc : public UIOBJECT_DESC
	{
		TEAM eTeam;
	}DAMAGEBAR_WORLD_DESC;
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
	TEAM m_eTeam = { TEAM::END };

private:
	//피 딜레이 애니메이션 시작시간 체크하는 함수
	_float	m_fTimeAcc = { 0.f };

	_float	m_fFillAmount = { 1.f };
	_float	m_fFillDelay = { 1.f };

private:
	const _float m_fBaseScale = 0.03f;

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	//CTexture* m_pTextureCom_Background = { nullptr };
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