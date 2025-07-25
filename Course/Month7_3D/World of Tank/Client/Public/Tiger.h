#pragma once

#include "Client_Defines.h"
#include "Tank.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class CSoundController;
NS_END

NS_BEGIN(Client)

class CTiger final : public CTank
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
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects(TANK_DESC* pDesc);

public:
	static CTiger* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END