#pragma once

#include "Client_Defines.h"
#include "Tank.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class CSoundController;
NS_END

NS_BEGIN(Client)

class CFury final : public CTank
{
private:
	CFury(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFury(const CFury& Prototype);
	virtual ~CFury() = default;

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
	static CFury* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END