#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)

class CMapObject final : public CGameObject
{
public:
	typedef struct tagMapObject : public GAMEOBJECT_DESC
	{
		wstring				wstrModelCom = {};
		D3D11_CULL_MODE		eCullMode = { D3D11_CULL_BACK };
	}MAPOBJECT_DESC;

private:
	CMapObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMapObject(const CMapObject& Prototype);
	virtual ~CMapObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	const wstring& Get_ModelCom() const { return m_wstrComModel; }

private:
	HRESULT Render_Cull_Front();
private:
	wstring				m_wstrComModel = {};
	D3D11_CULL_MODE		m_eCullMode = { D3D11_CULL_BACK };
private:
	CShader*	m_pShaderCom = { nullptr };
	CModel*		m_pModelCom = { nullptr };
private:
	HRESULT Ready_Components(wstring wstrComModel);
	HRESULT Bind_ShaderResources();
public:
	static CMapObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END