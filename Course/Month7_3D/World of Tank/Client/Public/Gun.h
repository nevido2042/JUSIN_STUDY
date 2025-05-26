#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

NS_BEGIN(Engine)
class CSoundController;
NS_END

NS_BEGIN(Client)

class CGun abstract : public CGameObject
{
protected:
	CGun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGun(const CGun& Prototype);
	virtual ~CGun() = default;

public:
	virtual HRESULT Initialize_Prototype() { return S_OK; };
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta) {}
	virtual HRESULT Render() { return S_OK; }

public:
	void	Set_Up(_bool bUp) { m_bUp = bUp; }
	void	Set_Down(_bool bDown) { m_bDown = bDown; }
public:
	HRESULT	Fire();

protected:
	const _float	m_fSyncInterval = { 0.5f };
	_float			m_fTimeAcc = { 0 };

protected:
	_bool	m_bUp = { false };
	_bool	m_bDown = { false };

protected:
	CSoundController* m_pSoundCom{ nullptr };

	virtual void Free() override;
};

NS_END
