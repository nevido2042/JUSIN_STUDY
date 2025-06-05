#pragma once
#include "Module.h"
#include "Client_Defines.h"

NS_BEGIN(Engine)
class CSoundController;
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)

class CGun abstract : public CModule
{
public:
	typedef struct tagGunDesc : public MODULE_DESC
	{
		_float4 vBaseColor = { 1.f, 1.f, 1.f, 1.f };
	}GUN_DESC;
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
	virtual HRESULT Render();

public:
	void	Set_Up(_bool bUp) { m_bUp = bUp; }
	void	Set_Down(_bool bDown) { m_bDown = bDown; }

public:
	HRESULT	Fire();

private:
	void Input(_float fTimeDelta);

private:
	_float4	m_vBaseColor = { 1.f, 1.f, 1.f, 1.f };

protected:
	const _float	m_fSyncInterval = { 0.5f };
	_float			m_fTimeAcc = { 0 };

protected:
	_bool	m_bUp = { false };
	_bool	m_bDown = { false };

protected:
	// 허용 각도 (예: 상향 30도, 하향 10도)
	const _float m_fMaxPitch = sinf(XMConvertToRadians(30.f));  // +Y
	const _float m_fMinPitch = sinf(XMConvertToRadians(-10.f)); // -Y

protected:
	CShader*			m_pShaderCom = { nullptr };
	CModel*				m_pModelCom = { nullptr };
	CSoundController*	m_pSoundCom{ nullptr };

protected:
	HRESULT Bind_ShaderResources();

	virtual void Free() override;
};

NS_END
