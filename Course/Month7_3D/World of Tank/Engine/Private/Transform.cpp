#include "Transform.h"

#include "Shader.h"

CTransform::CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent { pDevice, pContext }
{
}

CTransform::CTransform(const CTransform& Prototype)
	: CComponent(Prototype)
	, m_WorldMatrix { Prototype.m_WorldMatrix }
{
}

_float3 CTransform::Get_Scaled()
{	
	return _float3(XMVectorGetX(XMVector3Length(Get_State(STATE::RIGHT))),
		XMVectorGetX(XMVector3Length(Get_State(STATE::UP))),
		XMVectorGetX(XMVector3Length(Get_State(STATE::LOOK))));		
}



_float3 CTransform::Get_RotationEuler() const
{
	_vector scale, rotQuat, translation;
	XMMatrixDecompose(&scale, &rotQuat, &translation, XMLoadFloat4x4(&m_WorldMatrix));

	_float4 q;
	XMStoreFloat4(&q, rotQuat);

	_float3 eulerAngles;

	// Roll (X축 회전)
	_float sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
	_float cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
	eulerAngles.x = atan2f(sinr_cosp, cosr_cosp);

	// Pitch (Y축 회전)
	_float sinp = 2 * (q.w * q.y - q.z * q.x);
	if (fabs(sinp) >= 1)
		eulerAngles.y = copysignf(XM_PIDIV2, sinp);
	else
		eulerAngles.y = asinf(sinp);

	// Yaw (Z축 회전)
	_float siny_cosp = 2 * (q.w * q.z + q.x * q.y);
	_float cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
	eulerAngles.z = atan2f(siny_cosp, cosy_cosp);

	return eulerAngles;
}

void CTransform::Set_RotationEuler(const _vector& vEulerRadian)
{
	// 오일러 각 → 회전행렬 (XYZ 순서 기준)
	_matrix matRotation =
		XMMatrixRotationX(XMVectorGetX(vEulerRadian)) *
		XMMatrixRotationY(XMVectorGetY(vEulerRadian)) *
		XMMatrixRotationZ(XMVectorGetZ(vEulerRadian));

	// 현재 Scale, Position 값은 유지해야 하므로 분해
	_vector vScale, vRotationQuat, vTranslation;
	XMMatrixDecompose(&vScale, &vRotationQuat, &vTranslation, XMLoadFloat4x4(&m_WorldMatrix));

	// 최종 월드행렬 = Scale * Rotation * Translation
	_matrix matScale = XMMatrixScalingFromVector(vScale);
	_matrix matTranslation = XMMatrixTranslationFromVector(vTranslation);

	_matrix matWorld = matScale * matRotation * matTranslation;

	XMStoreFloat4x4(&m_WorldMatrix, matWorld);
}


HRESULT CTransform::Initialize_Prototype()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CTransform::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return S_OK;

	TRANSFORM_DESC* pDesc = static_cast<TRANSFORM_DESC*>(pArg);

	m_fSpeedPerSec = pDesc->fSpeedPerSec;
	m_fRotationPerSec = pDesc->fRotationPerSec;

	if (pDesc->vInitPosition.x != 0.f || pDesc->vInitPosition.y != 0.f || pDesc->vInitPosition.z != 0.f)
		Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&pDesc->vInitPosition), 1.f));

	return S_OK;
}

void CTransform::Scaling(_float fX, _float fY, _float fZ)
{
	Set_State(STATE::RIGHT, XMVector3Normalize(Get_State(STATE::RIGHT)) * fX);
	Set_State(STATE::UP, XMVector3Normalize(Get_State(STATE::UP)) * fY);
	Set_State(STATE::LOOK, XMVector3Normalize(Get_State(STATE::LOOK)) * fZ);
}

void CTransform::Scaling(const _float3& vScale)
{
	Set_State(STATE::RIGHT, XMVector3Normalize(Get_State(STATE::RIGHT)) * vScale.x);
	Set_State(STATE::UP, XMVector3Normalize(Get_State(STATE::UP)) * vScale.y);
	Set_State(STATE::LOOK, XMVector3Normalize(Get_State(STATE::LOOK)) * vScale.z);
}

void CTransform::Go_Straight(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE::POSITION);
	_vector		vLook = Get_State(STATE::LOOK);
		
	vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Backward(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE::POSITION);
	_vector		vLook = Get_State(STATE::LOOK);

	vPosition -= XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Right(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE::POSITION);
	_vector		vRight = Get_State(STATE::RIGHT);

	vPosition += XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Left(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE::POSITION);
	_vector		vRight = Get_State(STATE::RIGHT);

	vPosition -= XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Target(_fvector vTarget, _float fTimeDelta, _float fMinDistance)
{
	_vector		vMoveDir = vTarget - Get_State(STATE::POSITION);

	if(fMinDistance <= XMVectorGetX(XMVector3Length(vMoveDir)))
		Set_State(STATE::POSITION, Get_State(STATE::POSITION) + XMVector3Normalize(vMoveDir) * m_fSpeedPerSec * fTimeDelta);
}

void CTransform::Turn(_fvector vAxis, _float fTimeDelta)
{
	_matrix			RotationMatrix = XMMatrixRotationAxis(vAxis, m_fRotationPerSec * fTimeDelta);

	Set_State(STATE::RIGHT, XMVector4Transform(Get_State(STATE::RIGHT), RotationMatrix));
	Set_State(STATE::UP, XMVector4Transform(Get_State(STATE::UP), RotationMatrix));
	Set_State(STATE::LOOK, XMVector4Transform(Get_State(STATE::LOOK), RotationMatrix));
}

void CTransform::LookAt(_fvector vAt)
{
	_float3		vScaled = Get_Scaled();

	_vector		vLook = vAt - Get_State(STATE::POSITION);
	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	_vector		vUp = XMVector3Cross(vLook, vRight);

	Set_State(STATE::RIGHT, XMVector3Normalize(vRight) * vScaled.x);
	Set_State(STATE::UP, XMVector3Normalize(vUp) * vScaled.y);
	Set_State(STATE::LOOK, XMVector3Normalize(vLook) * vScaled.z);
}

HRESULT CTransform::Bind_ShaderResource(CShader* pShader, const _char* pConstantName)
{
	return pShader->Bind_Matrix(pConstantName, &m_WorldMatrix);	
}

CTransform* CTransform::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTransform* pInstance = new CTransform(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent* CTransform::Clone(void* pArg)
{
	CTransform* pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;

}

void CTransform::Free()
{
	__super::Free();

}
