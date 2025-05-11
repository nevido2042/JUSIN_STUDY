#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CSoundController;
NS_END

#define RPM_MAX 600
#define RPM_MIN 0

NS_BEGIN(Client)

class CEngine final : public CGameObject
{
private:
	CEngine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEngine(const CEngine& Prototype);
	virtual ~CEngine() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Start_Engine();
	void Press_Accelerator(_float fTimeDelta);
private:
	void Set_RPM(_float _fValue);
private:
	_bool	m_bIsEngineOn = { false };
	_float	m_fRPM = { 0.f };
private:
	CSoundController* m_pSoundCom{ nullptr };
private:
	HRESULT Ready_Components();

public:
	static CEngine* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END