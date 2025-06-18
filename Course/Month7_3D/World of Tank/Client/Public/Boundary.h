#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Client)

class CBoundary final : public CGameObject
{
private:
	CBoundary(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoundary(const CBoundary& Prototype);
	virtual ~CBoundary() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

//public:
//	_bool IsPointInBoundary(const _float3& vPoint, _float3& vOutClosestPos);

public:
	_bool IsPointInBoundary(const _float3& vPoint);
	_float3 ClosestPointOnBoundary(const _float3& vPos);

private:
	_bool IsPointOnSegmentXZ(const _float3& p, const _float3& a, const _float3& b, _float fTolerance);

private:
	HRESULT Load_BoundaryPoints();
	void Draw_Boundary();
private:
	vector<_float3> m_BoundaryPoints;

	PrimitiveBatch<VertexPositionColor>* m_pBatch = { nullptr };
	BasicEffect* m_pEffect = { nullptr };
	ID3D11InputLayout* m_pInputLayout = { nullptr };

public:
	static CBoundary* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END