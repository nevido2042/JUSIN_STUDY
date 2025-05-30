#pragma once

#include "Client_Defines.h"
#include "LandObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class CSoundController;
NS_END

NS_BEGIN(Client)

class CTank abstract : public CLandObject
{
protected:
	CTank(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTank(const CTank& Prototype);
	virtual ~CTank() = default;

public:
	virtual HRESULT Initialize_Prototype() = 0;
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Try_Fire();
	HRESULT Take_Damage(_float fDamage);

private:
	void Input();
private:
	void Move(_float fTimeDelta);
private:
	void Destroyed();
private:
	HRESULT Set_State_Engine(MODULE_STATE eState);

private:
	void ApplyRecoil(_float fTimeDelta);

protected:
	const	_float m_fMaxHP = { 100.f };
	_float	m_fHP = { m_fMaxHP };

private:
	_float m_fRecoilTime = 0.f;
	const _float m_fMaxRecoilTime = 0.3f;

private:
	const _float	m_fSyncInterval = { 0.5f };
	_float			m_fTimeAcc = { 0 };

private:
	_bool	m_bDestroyed = { false };

protected:
	CShader*			m_pShaderCom = { nullptr };
	CModel*				m_pModelCom = { nullptr };
	CModel*				m_pModelCom_Destroyed = { nullptr };
	CSoundController*	m_pSoundCom = { nullptr };

private:
	HRESULT Bind_ShaderResources();

public:
	virtual void Free() override;

};

NS_END