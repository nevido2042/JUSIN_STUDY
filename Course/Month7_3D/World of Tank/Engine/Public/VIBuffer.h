#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	CVIBuffer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	CVIBuffer(const CVIBuffer& Prototype);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

	virtual HRESULT Bind_Buffers();
	virtual HRESULT Render();

public:
	virtual _vector Compute_HeightPosition(const _vector& vPosition) { return vPosition; }
	virtual _vector Compute_NormalPosition(const _vector& vPosition) { return vPosition; }
	virtual _float3 Compute_PickedPosition(const _matrix& pWorldMatrixInverse);

protected:
	ID3D11Buffer* m_pVB = { nullptr };
	ID3D11Buffer* m_pIB = { nullptr };

protected:
	_float3*					m_pVertexPositions = {};

protected:
	_uint						m_iNumVertices = {};
	_uint						m_iVertexStride = {};
	_uint						m_iNumIndices = {};
	_uint						m_iIndexStride = {};
	_uint						m_iNumVertexBuffers = {};
	_uint						m_iNumPritimive = {};

	void*						m_pIndices = {};

	DXGI_FORMAT					m_eIndexFormat = {};
	D3D11_PRIMITIVE_TOPOLOGY	m_ePrimitiveTopology = {};

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;

};

NS_END