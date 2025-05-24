#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

NS_BEGIN(Client)

class CTurret abstract : public CGameObject
{
protected:
	CTurret(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTurret(const CTurret& Prototype);
	virtual ~CTurret() = default;

public:
	virtual HRESULT Initialize_Prototype() { return S_OK; };
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta) {}
	virtual HRESULT Render() { return S_OK; }

public:
	void	Set_Left(_bool bLeft) { m_bLeft = bLeft; }
	void	Set_Right(_bool bRight) { m_bRight = bRight; }

protected:
	const _float	m_fSyncInterval = { 0.5f };
	_float			m_fTimeAcc = { 0 };

protected:
	_bool	m_bLeft = { false };
	_bool	m_bRight = { false };

	virtual void Free() override;
};

NS_END
