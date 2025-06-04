#pragma once

#include "Client_Defines.h"
#include "Module.h"

NS_BEGIN(Engine)
class CSoundController;
NS_END

NS_BEGIN(Client)

//장전 속도, 장전 사운드 관리

class CAmmoBay final : public CModule
{
private:
	CAmmoBay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAmmoBay(const CAmmoBay& Prototype);
	virtual ~CAmmoBay() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Start_Load();
public:
	HRESULT is_Load_Complete();

private:
	_bool m_bIsPlayLoadSound = { false }; // 장전 사운드 재생 여부
private:
	_bool m_bIsLoading = { false }; // 장전 중인지 여부
private:
	chrono::duration<_float>	m_Remaining = {}; // 남은 시간
	_float						m_fDisplaySeconds = {}; // 남은 시간 표시용
private:
	const _float m_fLoadTime = { 3.5f }; // 장전 시간
private:
	chrono::time_point < chrono::steady_clock, chrono::duration<_float>> m_TimeLoadComplete = {};
private:
	CSoundController* m_pSoundCom{ nullptr };
private:
	HRESULT Ready_Components();

public:
	static CAmmoBay* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END