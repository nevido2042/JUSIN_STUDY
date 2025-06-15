#pragma once

#include "Client_Defines.h"
#include "GameObject.h"


NS_BEGIN(Client)

#pragma message ("이 클래스 필요 없을 수도 있는데 일단 냅둠.")

class CPickedManager final : public CGameObject
{
private:
	CPickedManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPickedManager(const CPickedManager& Prototype);
	virtual ~CPickedManager() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	_float3 Get_ScreenCenterPickedPos() const { return m_vScreenCenterPickedPos; }
	void	Add_ScreenCenterPickedPos(_float3 vPickedPos);

public:
	_float3 Get_GunPickedPos() const { return m_vGunPickedPos; }
	void	Add_GunPickedPos(_float3 vPickedPos);

private:
	void Update_ScreenCenterPickedPos();
	void Update_GunPickedPos();

private:
	list<_float3> m_ScreenCenterPickedPoses = {};
	list<_float3> m_GunPickedPoses = {};

	_float3		m_vScreenCenterPickedPos = {};
	_float3		m_vGunPickedPos = {};

public:
	static CPickedManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END