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

public:
	virtual _vector Compute_HeightPosition(const _vector& vPosition) override;
	virtual _vector Compute_NormalPosition(const _vector& vPosition) override;
	virtual _float3 Compute_PickedPosition(const _matrix& pWorldMatrixInverse) override;

private:
	_float3 Pick_Quad(_float3 vLB, _float3 vRB, _float3 vLT, _float3 vRT, const _matrix& worldInv);

private:
	HRESULT Read_HeightMap_BMP(const _tchar* pHeightMapFilePath, _float2 Offset);
	HRESULT Read_HeightMap_PNG(const _tchar* pHeightMapFilePath, _float2 Offset);
private:
	_uint			m_iNumVerticesX = {};
	_uint			m_iNumVerticesZ = {};

	_float2			m_Offset = {};

public:
	static CVIBuffer_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pHeightMapFilePath, _float2 Offset = {1.f, 1.f});
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END