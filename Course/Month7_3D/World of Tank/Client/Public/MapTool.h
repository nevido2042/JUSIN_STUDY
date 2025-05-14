#pragma once

#include "Client_Defines.h"
#include "Tool.h"

NS_BEGIN(Client)

class CMapTool final : public CTool
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
	CGameObject* Get_Selected_GameObject(vector<string>& HierarchyNames);
	CGameObject* Get_GameObject(string HierarchyName);

private:
	class CTerrain* m_pTerrain = { nullptr };

private:
	vector<wstring> m_AssetNames = {};
	_int m_iSelectedAssetIndex = -1; // 선택된 항목 인덱스

	//vector<wstring> m_HierarchyNames = {};
	_int m_iSelectedHierarchyIndex = -1; // 선택된 항목 인덱스

private:
	HRESULT Delete_All();

	HRESULT Save_Map(vector<string>& HierarchyNames);
	HRESULT Load_Map();

	HRESULT Get_Assets();
	HRESULT Ready_Components();

public:
	static string WStringToString(const wstring& wstr);

public:
	static CMapTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END