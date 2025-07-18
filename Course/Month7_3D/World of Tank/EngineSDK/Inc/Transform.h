#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	typedef struct tagTransformDesc
	{
		_float			fSpeedPerSec;
		_float			fRotationPerSec;
		_float3			vInitPosition = { 0.f, 0.f, 0.f };
		_float3			vVelocity = { 0.f, 0.f, 0.f };
	}TRANSFORM_DESC;

protected:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& Prototype);
	virtual ~CTransform() = default;

public:
	_vector Get_State(STATE eState) const {
		return XMLoadFloat4x4(&m_WorldMatrix).r[ENUM_CLASS(eState)];
	}

	_float3 Get_Scaled();

	_matrix Get_WorldMatrix_Inverse() {
		return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));
	}

	_matrix Get_WorldMatrix() {
		return XMLoadFloat4x4(&m_WorldMatrix);
	}

	const _float4x4* Get_WorldMatrix_Ptr() const {
		return &m_WorldMatrix;
	}

	void Set_WorldMatrix(_float4x4 mWorldMatrix) {
		m_WorldMatrix = mWorldMatrix;
	}

	void Set_State(STATE eState, _fvector vState)
	{
		XMStoreFloat4(reinterpret_cast<_float4*>(&m_WorldMatrix.m[ENUM_CLASS(eState)]), vState);
	}

	_float3 Get_RotationEuler() const;

	void Set_RotationEuler(const _vector& vEulerRadian);


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);


public:
	void Scaling(_float fX = 1.f, _float fY = 1.f, _float fZ = 1.f);
	void Scaling(const _float3& vScale);

public:
	void Go_Straight(_float fTimeDelta, class CCollider* pCollider = nullptr, _float fFriction = 0.f);
	void Go_Backward(_float fTimeDelta, class CCollider* pCollider = nullptr, _float fFriction = 0.f);
	void Go_Right(_float fTimeDelta);
	void Go_Left(_float fTimeDelta);
	void Go_Target(_fvector vTarget, _float fTimeDelta, _float fMinDistance);

	void Turn(_fvector vAxis, _float fTimeDelta, CCollider* pCollider = nullptr, _float fPushPower = 0.f);
	void Rotation(_fvector vAxis, _float fRadian);
	_float3 Get_Rotation() const;
	void Rotation(_float fX, _float fY, _float fZ);
public:
	void LookAt(_fvector vAt);
	void LookAt_Reverse(_fvector vAt);

public:
	void Add_Velocity(_fvector vVelocity);
	void Set_Velocity(_float3 vVelocity) { m_vVelocity = vVelocity; }
	void Move_Velocity(_float fTimeDelta);
	void Apply_Gravity(_float fGravity, _float fTimeDelta);

public:
	_float Get_RotationPerSec() const { return m_fRotationPerSec; }
	_float Get_SpeedPerSec() const { return m_fSpeedPerSec; }

	void Set_RotationPerSec(_float fRotatePerSec) { m_fRotationPerSec = fRotatePerSec; }
	void Set_SpeedPerSec(_float fSpeedPerSec) { m_fSpeedPerSec = fSpeedPerSec; }

public:
	HRESULT Bind_ShaderResource(class CShader* pShader, const _char* pConstantName);

private:
	_float3		m_vVelocity = {};

private:
	_float4x4				m_WorldMatrix = {};
	_float					m_fSpeedPerSec = {};
	_float					m_fRotationPerSec = {};


public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END