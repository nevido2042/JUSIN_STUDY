#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
private:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider(const CCollider& Prototype);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype(COLLIDER eType);
	virtual HRESULT Initialize(void* pArg) override;
	void Update(_fmatrix WorldMatrix);
	_bool Intersect(CCollider* pTargetCollider, _vector* pOutNormal);
	_bool Intersect_Ray(_fvector vOrigin, _fvector vDir, _float& fDist);
#ifdef _DEBUG
	virtual HRESULT Render() override;
#endif

public:
	_bool Get_IsColl() const { return m_isColl; }
	_vector Get_Normal() const { return XMLoadFloat3(&m_vNormal); }
private:
	void Reset_Collision() { m_isColl = false; m_isRaycastHit = false; } friend class CCollider_Manager;

private:
	COLLIDER			m_eType = { COLLIDER::END };
	class CBounding*	m_pBounding = { nullptr };

	_bool				m_isColl = { false };
	_bool				m_isRaycastHit = { false };
	_float3				m_vNormal = {};

#ifdef _DEBUG
private:
	PrimitiveBatch<VertexPositionColor>* m_pBatch = { nullptr };
	BasicEffect* m_pEffect = { nullptr };
	ID3D11InputLayout* m_pInputLayout = { nullptr };
#endif

public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, COLLIDER eType);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

NS_END