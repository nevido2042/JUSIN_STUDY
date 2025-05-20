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
	HRESULT Ready_Layer_Terrain(const _wstring strLayerTag);
	HRESULT Ready_Layer_Camera_Free(const _wstring strLayerTag);
	HRESULT Ready_Layer_Camera_TPS(const _wstring strLayerTag);
	HRESULT Ready_Layer_Engine(const _wstring strLayerTag);
	HRESULT Ready_Layer_Skydome(const _wstring strLayerTag);
	HRESULT Ready_Layer_PlayerTank(const _wstring strLayerTag);

	//UI
	HRESULT Ready_Layer_Minimap(const _wstring strLayerTag);
	HRESULT Ready_Layer_DamagePanel(const _wstring strLayerTag);

	//TOOL
	HRESULT Ready_Layer_Tool_EngineSound(const _wstring strLayerTag);

private:
	HRESULT Load_Map();

public:
	static CLevel_Practice* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

NS_END