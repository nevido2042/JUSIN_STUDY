#pragma once

#include "Client_Defines.h"
#include "Level.h"

//#include "Engin_Sound_Tool.h"

NS_BEGIN(Client)

class CLevel_Practice final : public CLevel
{
private:
	CLevel_Practice(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Practice() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


private:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_Terrain(const _wstring strLayerTag);
	HRESULT Ready_Layer_Camera_Free(const _wstring strLayerTag);
	HRESULT Ready_Layer_Camera_TPS(const _wstring strLayerTag);
	HRESULT Ready_Layer_Camera_FPS(const _wstring strLayerTag);
	HRESULT Ready_Layer_Engine(const _wstring strLayerTag);
	HRESULT Ready_Layer_Skydome(const _wstring strLayerTag);
	HRESULT Ready_Layer_PlayerTank(const _wstring strLayerTag);
	HRESULT Ready_Layer_DummyTank(const _wstring strLayerTag);
	HRESULT Ready_Layer_DamageColliders(const _wstring strLayerTag);
	HRESULT Ready_Layer_PickedManager(const _wstring strLayerTag);
	HRESULT Ready_Layer_Boundary(const _wstring strLayerTag);
	HRESULT Ready_Layer_InvisibleWalls(const _wstring strLayerTag);
	HRESULT Ready_Layer_MapVegetation(const _wstring strLayerTag);


	//3D UI
	HRESULT Ready_Layer_GunMarker(const _wstring strLayerTag);

	//UI
	HRESULT Ready_Layer_CountdownTimer(const _wstring strLayerTag);
	HRESULT Ready_Layer_Minimap(const _wstring strLayerTag);
	HRESULT Ready_Layer_DamagePanel(const _wstring strLayerTag);
	HRESULT Ready_Layer_PersonalArrowEntry(const _wstring strLayerTag);
	HRESULT Ready_Layer_Crosshair(const _wstring strLayerTag);
	HRESULT Ready_Layer_DirectionBody(const _wstring strLayerTag);
	HRESULT Ready_Layer_Icon_Consumables(const _wstring strLayerTag);
	HRESULT Ready_Layer_Score(const _wstring strLayerTag);
	HRESULT Ready_Layer_AimCircle(const _wstring strLayerTag);
private:
	HRESULT Load_Map();

public:
	static CLevel_Practice* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

NS_END