#pragma once

#include "VIBuffer.h"

NS_BEGIN(Engine)

const int MAX_TRIANGLES_PER_NODE = 100;
const int MAX_QUADTREE_DEPTH = 8;

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
private:
	CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Terrain(const CVIBuffer_Terrain& Prototype);
	virtual ~CVIBuffer_Terrain() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pHeightMapFilePath, _float2 Offset);
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Render() override;

public:
	virtual _vector Compute_HeightPosition(const _vector& vPosition) override;
	virtual _vector Compute_NormalPosition(const _vector& vPosition) override;
	virtual _float3 Compute_PickedPosition(const _matrix& pWorldMatrixInverse) override;
	_bool PickQuadTreeNode(const _float3& rayOrigin, const _float3& rayDir, _float& outNearestDist, _uint& outPickedTriangleIndex);

private:
	void ComputeTriangleAABB(const _float3& v0, const _float3& v1, const _float3& v2, _float3& outMin, _float3& outMax);
	_bool AABBOverlap(const _float3& minA, const _float3& maxA, const _float3& minB, const _float3& maxB);
	void BuildQuadTree(QuadTreeNode* pNode, _int depth, const _float3* pPositions, const _uint* pIndices, _int numIndices);
	QuadTreeNode* CreateTerrainQuadTree(const _float3* pPositions, const _uint* pIndices, _int numVertices, _int numIndices);
	//_float3 Pick_Quad(_float3 vLB, _float3 vRB, _float3 vLT, _float3 vRT, const _matrix& worldInv);
	_bool RayIntersectAABB(const _float3& rayOrigin, const _float3& rayDir, const _float3& boxMin, const _float3& boxMax, float& outDistance);
	_bool RayIntersectTriangle(const _float3& rayOrigin, const _float3& rayDir, const _float3& v0, const _float3& v1, const _float3& v2, _float& outDistance);
	_bool PickQuadTreeNode(QuadTreeNode* pNode, const _float3& rayOrigin, const _float3& rayDir, const _float3* pPositions, const _uint* pIndices, _float& outNearestDist, _uint& outPickedTriangleIndex);

private:
	HRESULT Read_HeightMap_BMP(const _tchar* pHeightMapFilePath, _float2 Offset);
	HRESULT Read_HeightMap_PNG(const _tchar* pHeightMapFilePath, _float2 Offset);
private:
	_uint			m_iNumVerticesX = {};
	_uint			m_iNumVerticesZ = {};

private:
	_float2			m_Offset = {};

private:
	QuadTreeNode* m_pQuadTreeRoot = nullptr;


public:
	static CVIBuffer_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pHeightMapFilePath, _float2 Offset = {1.f, 1.f});
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END