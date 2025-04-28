#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
private:
	CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Terrain(const CVIBuffer_Terrain& Prototype);
	virtual ~CVIBuffer_Terrain() = default;

public:
	virtual HRESULT Initialize_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ);
	virtual HRESULT Initialize_Prototype(const _tchar* pHeightMapFilePath);
	virtual HRESULT Initialize(void* pArg) override;

public:
	virtual _float3 Compute_HeightPosition(const _float3& vPosition) override;

private:
	_uint				m_iNumVerticesX = {};
	_uint				m_iNumVerticesZ = {};

public:
	static CVIBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumVerticesX, _uint iNumVerticesZ);
	static CVIBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pHeightMapFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END