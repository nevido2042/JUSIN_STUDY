#pragma once

#include "Client_Defines.h"
#include "Tool.h"

NS_BEGIN(Client)

class CTool_Engine_Sound final : public CTool
{
private:
	CTool_Engine_Sound(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTool_Engine_Sound(const CTool_Engine_Sound& Prototype);
	virtual ~CTool_Engine_Sound() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();


private:
	class CEngine* m_pEngine = { nullptr };

private:
	HRESULT Ready_Components();

public:
	static CTool_Engine_Sound* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END