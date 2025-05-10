#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Client)

class CMapTool final : public CUIObject
{
private:
	CMapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMapTool(const CMapTool& Prototype);
	virtual ~CMapTool() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	class CTerrain* m_pTerrain = { nullptr };

private:
	HRESULT Ready_Components();

private:
	_bool m_bShowDemoWindow = true;
	ImVec4 m_ClearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

public:
	static CMapTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END