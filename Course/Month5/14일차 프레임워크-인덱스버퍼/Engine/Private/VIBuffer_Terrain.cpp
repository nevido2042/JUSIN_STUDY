#include "VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer { pGraphic_Device }
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain& Prototype)
	: CVIBuffer{ Prototype }
	, m_iNumVerticesX { Prototype.m_iNumVerticesX }
	, m_iNumVerticesZ { Prototype.m_iNumVerticesZ }
{
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ)
{
	//ü���������� Ŭ���� �Լ��� ����� ��
	m_iNumVerticesX = iNumVerticesX;
	m_iNumVerticesZ = iNumVerticesZ;
	m_iNumVertices = iNumVerticesX * iNumVerticesZ;
	m_iVertexStride = sizeof(VTXPOSTEX);
	m_iFVF = D3DFVF_XYZ | D3DFVF_TEX1/* | D3DFVF_TEXCOORDSIZE2(0)*/;
	m_iNumPritimive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;
	m_iNumIndices = m_iNumPritimive * 3;
	m_iIndexStride = 4;
	m_eIndexFormat = D3DFMT_INDEX32;

#pragma region VERTEX_BUFFER
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXPOSTEX* pVertices = { nullptr };


	m_pVB->Lock(0, /*m_iNumVertices * m_iVertexStride*/0, reinterpret_cast<void**>(&pVertices), 0);

	for (size_t i = 0; i < m_iNumVerticesZ; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX; j++)
		{
			_uint			iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _float3(j, 0.f, i);
			pVertices[iIndex].vTexcoord = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
		}
	}	

	m_pVB->Unlock();
#pragma endregion

#pragma region INDEX_BUFFER
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	_uint* pIndices = { nullptr };

 	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);
		
	/*for (_uint i = 0; i < m_iNumIndices / 6; ++i)
	{
		pIndices[6 * i] = i + i / (m_iNumVerticesX - 1) + m_iNumVerticesX;
		pIndices[6 * i + 1] = i + i / (m_iNumVerticesX - 1) + m_iNumVerticesX + 1;
		pIndices[6 * i + 2] = i + i / (m_iNumVerticesX - 1) + 1;

		pIndices[6 * i + 3] = i + i / (m_iNumVerticesX - 1) + m_iNumVerticesX;
		pIndices[6 * i + 4] = i + i / (m_iNumVerticesX - 1) + 1;
		pIndices[6 * i + 5] = i + i / (m_iNumVerticesX - 1);
	}*/

	for (_uint i = 0; i < m_iNumIndices / 6; ++i)
	{
		_uint iRowOffset = i / (m_iNumVerticesX - 1); // ���� �ٿ��� �� ��°���� ����
		_uint iIndexBase = i + iRowOffset;             // ������ �⺻ �ε���

		pIndices[6 * i] = iIndexBase + m_iNumVerticesX;
		pIndices[6 * i + 1] = iIndexBase + m_iNumVerticesX + 1;
		pIndices[6 * i + 2] = iIndexBase + 1;

		pIndices[6 * i + 3] = iIndexBase + m_iNumVerticesX;
		pIndices[6 * i + 4] = iIndexBase + 1;
		pIndices[6 * i + 5] = iIndexBase;
	}

	m_pIB->Unlock();

#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize(void* pArg)
{
	return S_OK;
}

CVIBuffer_Terrain* CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumVerticesX, _uint iNumVerticesZ)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(iNumVerticesX, iNumVerticesZ)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Terrain::Clone(void* pArg)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();


}
