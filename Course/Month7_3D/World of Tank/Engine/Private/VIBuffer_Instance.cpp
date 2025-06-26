#include "VIBuffer_Instance.h"

CVIBuffer_Instance::CVIBuffer_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Instance::CVIBuffer_Instance(const CVIBuffer_Instance& Prototype)
	: CVIBuffer(Prototype)
	, m_pVBInstance{ Prototype.m_pVBInstance }
	, m_VBInstanceDesc{ Prototype.m_VBInstanceDesc }
	, m_VBInstanceSubresourceData{ Prototype.m_VBInstanceSubresourceData }
	, m_iNumInstance{ Prototype.m_iNumInstance }
	, m_iVertexInstanceStride{ Prototype.m_iVertexInstanceStride }
	, m_iNumIndexPerInstance{ Prototype.m_iNumIndexPerInstance }
	, m_eEmissionShape{ Prototype.m_eEmissionShape }
{
	Safe_AddRef(m_pVBInstance);
}

HRESULT CVIBuffer_Instance::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer_Instance::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Instance::Bind_Buffers()
{
	ID3D11Buffer* pVertexBuffers[] = {
		   m_pVB,
		   m_pVBInstance,
	};

	_uint		iVertexStrides[] = {
		m_iVertexStride,
		m_iVertexInstanceStride

	};

	_uint		iOffsets[] = {
		0,
		0
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iVertexStrides, iOffsets);
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_ePrimitiveTopology);

	return S_OK;
}

HRESULT CVIBuffer_Instance::Render()
{

	m_pContext->DrawIndexedInstanced(m_iNumIndexPerInstance, m_iNumInstance, 0, 0, 0);

	return S_OK;
}

void CVIBuffer_Instance::Emission(_float fTimeDelta)
{
	switch (m_eEmissionShape)
	{
	case Engine::EMISSION_SHAPE::DROP:
		Drop(fTimeDelta);
		break;
	case Engine::EMISSION_SHAPE::SPREAD:
		Spread(fTimeDelta);
		break;
	case Engine::EMISSION_SHAPE::END:
		break;
	default:
		break;
	}

}

void CVIBuffer_Instance::Drop(_float fTimeDelta)
{


}

void CVIBuffer_Instance::Spread(_float fTimeDelta)
{
}

void CVIBuffer_Instance::Change_NumInstance(_uint iNumInstance)
{
}

void CVIBuffer_Instance::Change_Range(_float3 vRange)
{
}

void CVIBuffer_Instance::Change_Size(_float2 vSize)
{
}

void CVIBuffer_Instance::Replay()
{
}

void CVIBuffer_Instance::Free()
{
	__super::Free();

	Safe_Release(m_pVBInstance);

}
