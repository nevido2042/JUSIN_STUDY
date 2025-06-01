#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CSoundController;
NS_END

NS_BEGIN(Client)

class CGameManager final : public CGameObject
{
private:
	CGameManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameManager(const CGameManager& Prototype);
	virtual ~CGameManager() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void		Set_Select_Tank(TANK eTank) { m_eSelectTank = eTank; }
	TANK		Get_Select_Tank() { return m_eSelectTank; }

public:
	_bool		Get_isGameStart() const { return m_isGameStart; }
	void		Set_isGameStart(_bool isGameStart) { m_isGameStart = isGameStart; }

public:
	HRESULT		Create_My_Tank(_float3 vPos);
	HRESULT		Ready_Layer_PlayerTank(const _wstring strLayerTag, _float3 vPos);
	HRESULT		Ready_Layer_Camera_TPS(const _wstring strLayerTag);
	HRESULT		Ready_Layer_Camera_FPS(const _wstring strLayerTag);
public:
	HRESULT		PlayBGM_LoadingGame();
	HRESULT		StopBGM_LoadingGame();
public:
	HRESULT		PlayBGM_Game();
	HRESULT		StopBGM_Game();
public:
	HRESULT		PlaySound_Button();

private:
	TANK		m_eSelectTank = { TANK::FURY };
	_bool		m_isGameStart = { false };

private:
	CSoundController* m_pSoundCom_BGM = { nullptr };
	CSoundController* m_pSoundCom_GUI = { nullptr };

private:
	HRESULT Ready_Components();

public:
	static CGameManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END