#pragma once

#include "GameObject.h"

NS_BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagCameraDesc : public CGameObject::GAMEOBJECT_DESC
	{
		_float3			vEye;
		_float3			vAt;
		_float			fFov, fNear, fFar;
	}CAMERA_DESC;
protected:
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera(const CCamera& Prototype);
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	_float	Get_Yaw() const { return m_fYaw; }
	void	Set_Yaw(_float fYaw) { m_fYaw = fYaw; }

	_float	Get_Pitch() const { return m_fPitch; }
	void	Set_Pitch(_float fPitch) { m_fPitch = fPitch; }

protected:
	void Clip_Cursor();

protected:
	_float				m_fFov = { };
	_float				m_fNear = { };
	_float				m_fFar = { };
	_float				m_fAspect = {};

	_float				m_fYaw = {};
	_float				m_fPitch = {};

protected:
	void Bind_Matrices();

public:
	virtual CGameObject* Clone(void* pArg) = 0;	
	virtual void Free() override;

};

NS_END