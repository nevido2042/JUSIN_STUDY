#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)

class CTigerTrackLeft final : public CGameObject
{
private:
	CTigerTrackLeft(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTigerTrackLeft(const CTigerTrackLeft& Prototype);
	virtual ~CTigerTrackLeft() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Set_Speed(_float fSpeed) { m_fSpeed = fSpeed; }

private:
	_float			m_fSpeed = {};
	_float			m_fUVScrollY = {};

private:
	ID3D11RasterizerState* m_pRasterState = { nullptr };
	ID3D11RasterizerState* m_pOldRasterState = { nullptr };
private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

private:
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
	HRESULT Bind_ShaderResources();

private:
	HRESULT Ready_Components();

public:
	static CTigerTrackLeft* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END