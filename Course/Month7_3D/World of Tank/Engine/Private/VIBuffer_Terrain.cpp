#include "VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CVIBuffer { pDevice, pContext }
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain& Prototype)
    : CVIBuffer(Prototype)
	, m_iNumVerticesX { Prototype.m_iNumVerticesX }
	, m_iNumVerticesZ { Prototype.m_iNumVerticesZ }
{
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(const _tchar* pHeightMapFilePath)
{	
	HANDLE			hFile = CreateFile(pHeightMapFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_ulong			dwByte = {};

	BITMAPFILEHEADER			fh{};
	BITMAPINFOHEADER			ih{};

	_bool bResult = { false };

	bResult = ReadFile(hFile, &fh, sizeof fh, &dwByte, nullptr);
	if (bResult == false)
		return E_FAIL;
	bResult = ReadFile(hFile, &ih, sizeof ih, &dwByte, nullptr);
	if (bResult == false)
		return E_FAIL;

	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;

	_uint* pPixels = new _uint[m_iNumVertices];	
	bResult = ReadFile(hFile, pPixels, sizeof(_uint) * m_iNumVertices, &dwByte, nullptr);
	if (bResult == false)
		return E_FAIL;

	
	m_iNumVertexBuffers = 1;
	m_iVertexStride = sizeof(VTXNORTEX);
	m_iNumPritimive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;
	m_iNumIndices = m_iNumPritimive * 3;
	m_iIndexStride = sizeof(_uint);
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	D3D11_BUFFER_DESC			VBBufferDesc{};
	VBBufferDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
	VBBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VBBufferDesc.CPUAccessFlags = /*D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE*/0;
	VBBufferDesc.StructureByteStride = m_iVertexStride;
	VBBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA		VBInitialData{};

	VTXNORTEX* pVertices = new VTXNORTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertices);

	m_pVertexPositions = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertexPositions, sizeof(_float3) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVerticesZ; i++)
	{
		for (_uint j = 0; j < m_iNumVerticesX; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _float3(
				static_cast<_float>(j), 
				static_cast<_float>((pPixels[iIndex] & 0x000000ff) * 0.1f), 
				static_cast<_float>(i));

			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexcoord = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesX - 1.f));
		}
	}


	for (_uint i = 0; i < m_iNumVertices; ++i)
		m_pVertexPositions[i] = pVertices[i].vPosition;

	VBInitialData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&VBBufferDesc, &VBInitialData, &m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	D3D11_BUFFER_DESC			IBBufferDesc{};
	IBBufferDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
	IBBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IBBufferDesc.CPUAccessFlags = /*D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE*/0;
	IBBufferDesc.StructureByteStride = m_iIndexStride;
	IBBufferDesc.MiscFlags = 0;

	m_pIndices = new _uint[m_iNumIndices];
	ZeroMemory(m_pIndices, sizeof(_uint) * m_iNumIndices);

	_uint	iNumIndices = { 0 };

	for (_uint i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			_uint		iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			reinterpret_cast<_uint*>(m_pIndices)[iNumIndices++] = iIndices[0];
			reinterpret_cast<_uint*>(m_pIndices)[iNumIndices++] = iIndices[1];
			reinterpret_cast<_uint*>(m_pIndices)[iNumIndices++] = iIndices[2];

			reinterpret_cast<_uint*>(m_pIndices)[iNumIndices++] = iIndices[0];
			reinterpret_cast<_uint*>(m_pIndices)[iNumIndices++] = iIndices[2];
			reinterpret_cast<_uint*>(m_pIndices)[iNumIndices++] = iIndices[3];
		}
	}

	D3D11_SUBRESOURCE_DATA		IBInitialData{};
	IBInitialData.pSysMem = m_pIndices;

	if (FAILED(m_pDevice->CreateBuffer(&IBBufferDesc, &IBInitialData, &m_pIB)))
		return E_FAIL;

	CloseHandle(hFile);
	Safe_Delete_Array(pPixels);

    return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize(void* pArg)
{
    return S_OK;
}

_vector CVIBuffer_Terrain::Compute_HeightPosition(const _vector& vPosition)
{
	_uint iIndex = static_cast<_uint>(XMVectorGetZ(vPosition)) * m_iNumVerticesX + static_cast<_uint>(XMVectorGetX(vPosition));

	_uint iIndices[4] = {
		iIndex + m_iNumVerticesX,
		iIndex + m_iNumVerticesX + 1,
		iIndex + 1,
		iIndex
	};

	_float fWidth = XMVectorGetX(vPosition) - m_pVertexPositions[iIndices[0]].x;
	_float fDepth = m_pVertexPositions[iIndices[0]].z - XMVectorGetZ(vPosition);

	// Plane 계산용 Vector
	_vector v0 = XMLoadFloat3(&m_pVertexPositions[iIndices[0]]);
	_vector v1, v2;

	// 오른쪽 위 삼각형
	if (fWidth > fDepth)
	{
		v1 = XMLoadFloat3(&m_pVertexPositions[iIndices[1]]);
		v2 = XMLoadFloat3(&m_pVertexPositions[iIndices[2]]);
	}
	// 왼쪽 아래 삼각형
	else
	{
		v1 = XMLoadFloat3(&m_pVertexPositions[iIndices[2]]);
		v2 = XMLoadFloat3(&m_pVertexPositions[iIndices[3]]);
	}

	// 평면 정의 (직선적 변환)
	_vector vEdge1 = v1 - v0;
	_vector vEdge2 = v2 - v0;

	// 법선 벡터 구하기 (Cross Product)
	_vector vNormal = XMVector3Normalize(XMVector3Cross(vEdge1, vEdge2));

	// 평면 방정식의 d값 계산
	_float d = -XMVectorGetX(XMVector3Dot(vNormal, v0));

	// 높이(y) 계산 : y = (-a*x - c*z - d) / b
	_float a = XMVectorGetX(vNormal);
	_float b = XMVectorGetY(vNormal);
	_float c = XMVectorGetZ(vNormal);

	_float x = XMVectorGetX(vPosition);
	_float z = XMVectorGetZ(vPosition);

	_float y = (-a * x - c * z - d) / b;

	return XMVectorSet(x, y, z, 1.f);
}


CVIBuffer_Terrain* CVIBuffer_Terrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pHeightMapFilePath)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pHeightMapFilePath)))
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
		MSG_BOX("Failed to Cloned : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
    __super::Free();


}
