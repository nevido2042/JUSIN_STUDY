#pragma once
#include "Module.h"
#include "Client_Defines.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)

class CTurret abstract : public CModule
{
public:
	typedef struct tagTurretDesc : public GAMEOBJECT_DESC
	{
		_float4 vBaseColor =	{ 1.f, 1.f, 1.f, 1.f };
		_float4 vGunColor =		{ 1.f, 1.f, 1.f, 1.f };
	}TURRET_DESC;

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
	virtual HRESULT Render();

public:
	void	Set_Left(_bool bLeft) { m_bLeft = bLeft; }
	void	Set_Right(_bool bRight) { m_bRight = bRight; }

private:
	void Input(_float fTimeDelta);

private:
	_float4 m_vBaseColor = { 1.f, 1.f, 1.f, 1.f };

protected:
	const _float	m_fSyncInterval = { 0.5f };
	_float			m_fTimeAcc = { 0 };

protected:
	_bool	m_bLeft = { false };
	_bool	m_bRight = { false };


protected:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

protected:
	HRESULT Bind_ShaderResources();

	virtual void Free() override;
};

NS_END
