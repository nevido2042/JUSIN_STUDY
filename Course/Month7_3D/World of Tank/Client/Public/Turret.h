#pragma once
#include "Module.h"
#include "Client_Defines.h"

NS_BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
class CSoundController;
NS_END

NS_BEGIN(Client)

class CTurret abstract : public CModule
{
public:
	typedef struct tagTurretDesc : public MODULE_DESC
	{
		_float4 vBaseColor =	{ 1.f, 1.f, 1.f, 1.f };
		_float4 vGunColor =		{ 1.f, 1.f, 1.f, 1.f };
		CUSTOM3D e3DCustom = { CUSTOM3D::END };
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
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_Outline() override;
	virtual HRESULT Render_Shadow() override;
	virtual void On_RaycastHit(CGameObject* pOther) override;

public:
	virtual void	Set_ModuleState(MODULE_STATE eState) override;


public:
	void	Set_Left(_bool bLeft) { m_bLeft = bLeft; }
	void	Set_Right(_bool bRight) { m_bRight = bRight; }

private:
	void Input(_float fTimeDelta);

private:
	_float m_fTimeAccSameDir = { 0.f };//한 방향으로 계속 이동하는시간, 움직이는 소리 때문에

private:
	_float4 m_vBaseColor = { 1.f, 1.f, 1.f, 1.f };
private:
	CUSTOM3D m_eCustom3D = { CUSTOM3D::END };

protected:
	const _float	m_fSyncInterval = { 0.5f };
	_float			m_fTimeAcc = { 0 };

protected:
	_bool	m_bLeft = { false };
	_bool	m_bRight = { false };
	_float	m_fRotateSpeed = 1.f;

protected:
	CCollider* m_pColliderCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CSoundController* m_pSoundCom = { nullptr };

protected:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Ready_PartObjects();

public:
	virtual void Free() override;
};

NS_END
