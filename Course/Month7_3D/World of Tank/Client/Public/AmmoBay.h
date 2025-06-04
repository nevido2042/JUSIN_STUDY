#pragma once

#include "Client_Defines.h"
#include "Module.h"

NS_BEGIN(Engine)
class CSoundController;
NS_END

NS_BEGIN(Client)

//���� �ӵ�, ���� ���� ����

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
	_bool m_bIsPlayLoadSound = { false }; // ���� ���� ��� ����
private:
	_bool m_bIsLoading = { false }; // ���� ������ ����
private:
	chrono::duration<_float>	m_Remaining = {}; // ���� �ð�
	_float						m_fDisplaySeconds = {}; // ���� �ð� ǥ�ÿ�
private:
	const _float m_fLoadTime = { 3.5f }; // ���� �ð�
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