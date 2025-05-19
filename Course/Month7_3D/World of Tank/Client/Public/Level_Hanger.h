#pragma once

#include "Client_Defines.h"
#include "Level.h"

NS_BEGIN(Client)

class CLevel_Hanger final : public CLevel
{
private:
	CLevel_Hanger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Hanger() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_Terrain(const _wstring strLayerTag);
	HRESULT Ready_Layer_Camera(const _wstring strLayerTag);

private:
	HRESULT Load_Map();

public:
	static CLevel_Hanger* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

NS_END