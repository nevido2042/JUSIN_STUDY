#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
class CSoundController;
NS_END

NS_BEGIN(Client)

class CGameTimer final : public CUIObject
{
private:
	CGameTimer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameTimer(const CGameTimer& Prototype);
	virtual ~CGameTimer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();


private:
	chrono::high_resolution_clock::time_point m_GameEndTime = {};
	chrono::high_resolution_clock::time_point m_GameStartTime = {};
	chrono::seconds m_Remaining = {};
private:
	_bool m_bIsSpawnMissile = { false };
	_bool m_bIsPlaySiren = { false };
private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CSoundController* m_pSoundCom = { nullptr };

private:
	HRESULT Ready_Components();

public:
	static CGameTimer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END