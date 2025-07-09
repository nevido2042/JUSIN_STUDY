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
	HRESULT		Create_My_Tank(SPAWN_TANK_DESC* pDesc);
	HRESULT		Ready_Layer_PlayerTank(const _wstring strLayerTag, SPAWN_TANK_DESC* pDesc);
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
public:
	HRESULT		PlaySound_Repair();
	HRESULT		PlaySound_Medkit();
	HRESULT		PlaySound_Extinguisher();

public:
	const _float4&  Get_BodyColor() const { return m_vBodyColor; }
	void			Set_BodyColor(const _float4& vColor) { m_vBodyColor = vColor; }

	const _float4&	Get_TurretColor() const { return m_vTurretColor; }
	void 			Set_TurretColor(const _float4& vColor) { m_vTurretColor = vColor; }

	const _float4&	Get_GunColor() const { return m_vGunColor; }
	void 			Set_GunColor(const _float4& vColor) { m_vGunColor = vColor; }

	const PART_REPAINT& Get_PartRepaint() const { return m_ePartRepaint; }
	void				Set_PartRepaint(const PART_REPAINT& ePartRepaint);

	const CUSTOM3D	Get_3DCustom() const { return m_e3DCustom; }
	void			Set_3DCustom(CUSTOM3D e3DCustom) { m_e3DCustom = e3DCustom; }
private:
	PART_REPAINT	m_ePartRepaint = { PART_REPAINT::BODY };
private:
	TANK		m_eSelectTank = { TANK::FURY };
	_float4		m_vBodyColor =		{ 1.f, 1.f, 1.f, 1.f };
	_float4		m_vTurretColor =	{ 1.f, 1.f, 1.f, 1.f };
	_float4		m_vGunColor =		{ 1.f, 1.f, 1.f, 1.f };
	CUSTOM3D	m_e3DCustom = { CUSTOM3D::END };
private:
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