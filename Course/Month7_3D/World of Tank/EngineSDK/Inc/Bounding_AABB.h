#pragma once

#include "Bounding.h"

NS_BEGIN(Engine)

class CBounding_AABB final : public CBounding
{
public:
	typedef struct tagBoundingBoxDesc : public CBounding::BOUNDING_DESC
	{
		_float3		vExtents;
	}AABB_DESC;
private:
	CBounding_AABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding_AABB() = default;

public:
	const BoundingBox* Get_Desc() const {
		return m_pDesc;
	}

public:
	HRESULT Initialize(const CBounding::BOUNDING_DESC* pDesc);
	virtual void Update(_fmatrix WorldMatrix) override;
	virtual _bool Intersect(CBounding* pTarget) override;
	virtual _bool Intersect_Ray(_fvector vOrigin, _fvector vDir, _float& fDist) override;


#ifdef _DEBUG
public:
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor) override;
#endif



private:
	/* ä������ �ʱ������� ���û� ���´�� �����Ѵ�. */
	BoundingBox* m_pOriginalDesc = { nullptr };
	BoundingBox* m_pDesc = { nullptr };

private:
	_bool Intersect_ToAABB(CBounding* pTarget);
	_float3 Compute_Min();
	_float3 Compute_Max();

public:
	static CBounding_AABB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CBounding::BOUNDING_DESC* pDesc);
	virtual void Free() override;

};

NS_END