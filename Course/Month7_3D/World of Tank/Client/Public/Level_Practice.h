#pragma once

#include "Client_Defines.h"
#include "Level.h"

#include "Engin_Sound_Tool.h"

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
	CEngin_Sound_Tool* m_pEngine_Sound_Tool = { nullptr };

private:
	HRESULT Ready_Layer_BackGround(const _wstring strLayerTag);
	HRESULT Ready_Layer_Camera(const _wstring strLayerTag);
	HRESULT Ready_Layer_Engine(const _wstring strLayerTag);
	HRESULT Ready_Layer_Engine_Sound_Tool();

public:
	static CLevel_Practice* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

NS_END