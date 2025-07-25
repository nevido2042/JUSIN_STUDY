#pragma once

#include "Client_Defines.h"
#include "Level.h"

NS_BEGIN(Client)

class CLevel_MapTool final : public CLevel
{
private:
	CLevel_MapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_MapTool() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_Tool_Base(const _wstring strLayerTag);
	HRESULT Ready_Layer_MapTool(const _wstring strLayerTag);

	HRESULT Ready_Layer_Skydome(const _wstring strLayerTag);
	HRESULT Ready_Layer_Terrain(const _wstring strLayerTag);
	HRESULT Ready_Layer_Camera(const _wstring strLayerTag);

	HRESULT Ready_Layer_Fury(const _wstring strLayerTag);

	HRESULT Ready_Layer_Decal(const _wstring strLayerTag);

public:
	static CLevel_MapTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

NS_END