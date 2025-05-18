#pragma once

#include "VIBuffer.h"

NS_BEGIN(Engine)

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
	_bool			PickQuadTreeNode(_float3& vOutPos, _float& vOutNearestDist, _uint& iOutPickedTriangleIndex);

private:
	_bool					PickQuadTreeNode(class CQuadTreeNode* pNode, const _float3* pPositions, const _uint* pIndices, _float& vOutNearestDist, _uint& iOutPickedTriangleIndex);
	_bool					RayIntersectAABB(const _float3& vBoxMin, const _float3& vBoxMax, _float& vOutDistance);
	_bool					RayIntersectTriangle(const _float3& v0, const _float3& v1, const _float3& v2, _float& vOutDistance);

private:
	class CQuadTreeNode*	CreateTerrainQuadTree(const _float3* pPositions, const _uint* pIndices, _uint iNumVertices, _uint iNumIndices);
	void					BuildQuadTree(class CQuadTreeNode* pNode, _uint iDepth, const _float3* pPositions, const _uint* pIndices, _uint iNumIndices);
	void					ComputeTriangleAABB(const _float3& v0, const _float3& v1, const _float3& v2, _float3& outMin, _float3& outMax);
	_bool					AABBOverlap(const _float3& minA, const _float3& maxA, const _float3& minB, const _float3& maxB);

private:
	HRESULT Read_HeightMap_BMP(const _tchar* pHeightMapFilePath, _float2 Offset);
	HRESULT Read_HeightMap_PNG(const _tchar* pHeightMapFilePath, _float2 Offset);
private:
	_uint			m_iNumVerticesX = {};
	_uint			m_iNumVerticesZ = {};

private:
	_float2			m_Offset = {};

private:
	const _uint		iMax_Triangles_Per_Node = { 100 };
	const _uint		iMax_QuadTree_Depth = { 8 };

private:
	class CQuadTreeNode* m_pQuadTreeRoot = { nullptr };


public:
	static CVIBuffer_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pHeightMapFilePath, _float2 Offset = {1.f, 1.f});
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END