#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CTool final : public CGameObject
{
private:
	CTool(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTool(const CTool& Prototype);
	virtual ~CTool() = default;

public:	
	virtual HRESULT Initialize_Prototype()override;	
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	// 현재 상태 변수
	bool show_demo_window = true;
	bool show_another_window = true;

private:
	HRESULT Ready_Components();

public:
	static CTool* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END