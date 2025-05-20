#pragma once

#include "Client_Defines.h"
#include "LandObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)

class CTiger final : public CLandObject
{
private:
	CTiger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTiger(const CTiger& Prototype);
	virtual ~CTiger() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	void Destroyed();

private:
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

private:
	_bool	m_bDestroyed = { false };
private:
	ID3D11RasterizerState* m_pRasterState = { nullptr };
	ID3D11RasterizerState* m_pOldRasterState = { nullptr };
private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CModel* m_pModelCom_Destroyed = { nullptr };

	HRESULT Ready_Components();

public:
	static CTiger* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END