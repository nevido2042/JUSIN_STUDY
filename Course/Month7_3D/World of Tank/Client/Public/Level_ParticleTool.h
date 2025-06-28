#pragma once

#include "Client_Defines.h"
#include "Level.h"

NS_BEGIN(Client)

class CLevel_ParticleTool final : public CLevel
{
private:
	CLevel_ParticleTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_ParticleTool() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_Terrain(const _wstring strLayerTag);
	HRESULT Ready_Layer_Camera_Free(const _wstring strLayerTag);
	HRESULT Load_Map();
	HRESULT Ready_Layer_Skydome(const _wstring strLayerTag);
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_Effect(const _wstring strLayerTag);
	HRESULT Ready_Layer_ParticleTool(const _wstring strLayerTag);
	HRESULT Ready_Layer_DummyTank(const _wstring strLayerTag);

public:
	static CLevel_ParticleTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

NS_END