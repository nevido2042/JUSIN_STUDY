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

public:
	void Set_Destroy_Green();
	void Set_Destroy_Red();

private:
	void Check_Team_All_Dead();

private:
	void Draw_GameResult();

private:
	_float m_fAccTime = { 0.f };

private:
	_bool m_bIsGameEnd = { false };
	
	GAME_RESULT m_tGameResult = {};
private:
	_uint m_iGreenScore = {};
	_uint m_iRedScore = {};

private:
	_uint m_iNumGreen = 0;
	_uint m_iNumRed = 0;

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();

public:
	static CScore* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END