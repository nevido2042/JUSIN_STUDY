#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)

class CFuryTurret final : public CGameObject
{
private:
	CFuryTurret(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFuryTurret(const CFuryTurret& Prototype);
	virtual ~CFuryTurret() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();
private:
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

private:
	ID3D11RasterizerState* m_pRasterState = { nullptr };
	ID3D11RasterizerState* m_pOldRasterState = { nullptr };
private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();

public:
	static CFuryTurret* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END