#include "VIBuffer_Terrain.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
	//체인지레벨에 클리어 함수들 만드셈 ㅋ
	m_iNumVerticesX = iNumVerticesX;
	m_iNumVerticesZ = iNumVerticesZ;
	m_iNumVertices = iNumVerticesX * iNumVerticesZ;
	m_iVertexStride = sizeof(VTXNORTEX);
	m_iFVF = D3DFVF_NORMAL | D3DFVF_XYZ | D3DFVF_TEX1/* | D3DFVF_TEXCOORDSIZE2(0)*/;
	m_iNumPritimive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;
	m_iNumIndices = m_iNumPritimive * 3;
	m_iIndexStride = 4;
	m_eIndexFormat = D3DFMT_INDEX32;

#pragma region VERTEX_BUFFER
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXNORTEX* pVertices = { nullptr };


	m_pVB->Lock(0, /*m_iNumVertices * m_iVertexStride*/0, reinterpret_cast<void**>(&pVertices), 0);

	for (size_t i = 0; i < m_iNumVerticesZ; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX; j++)
		{
			_uint			iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _float3(j, 0.f, i);
			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexcoord = _float2(j / (m_iNumVerticesX - 1.f) * 50.f, i / (m_iNumVerticesZ - 1.f) * 50.f);
		}
	}	


#pragma endregion

#pragma region INDEX_BUFFER
	m_pIndices = new _uint[m_iNumIndices];

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	_uint* pIndices = { nullptr };

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

	_uint		iNumIndices = { 0 };

	for (size_t i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint			iIndex = i * m_iNumVerticesX + j;

			_uint			iIndices[4] = {
				iIndex + m_iNumVerticesX, 
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1, 
				iIndex
			};

			_float3		vSourDir, vDestDir, vNormal;

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[1];
			pIndices[iNumIndices++] = iIndices[2];

			vSourDir = pVertices[iIndices[1]].vPosition - pVertices[iIndices[0]].vPosition;
			vDestDir = pVertices[iIndices[2]].vPosition - pVertices[iIndices[1]].vPosition;

			D3DXVec3Normalize(&vNormal, D3DXVec3Cross(&vNormal, &vSourDir, &vDestDir));

			pVertices[iIndices[0]].vNormal += vNormal;
			pVertices[iIndices[1]].vNormal += vNormal;
			pVertices[iIndices[2]].vNormal += vNormal;



			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[2];
			pIndices[iNumIndices++] = iIndices[3];


			vSourDir = pVertices[iIndices[2]].vPosition - pVertices[iIndices[0]].vPosition;
			vDestDir = pVertices[iIndices[3]].vPosition - pVertices[iIndices[2]].vPosition;

			D3DXVec3Normalize(&vNormal, D3DXVec3Cross(&vNormal, &vSourDir, &vDestDir));

			pVertices[iIndices[0]].vNormal += vNormal;
			pVertices[iIndices[2]].vNormal += vNormal;
			pVertices[iIndices[3]].vNormal += vNormal;
		}
	}

	memcpy(m_pIndices, pIndices, m_iIndexStride * m_iNumIndices);

	for (size_t i = 0; i < m_iNumVertices; i++)	
		D3DXVec3Normalize(&pVertices[i].vNormal, &pVertices[i].vNormal);

	m_pVB->Unlock();
	m_pIB->Unlock();

#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(const _tchar* pHeightMapFilePath)
{
	int width = 0, height = 0, channels = 0;

	// PNG는 UTF-8 경로만 받으므로 TCHAR → multibyte 변환
	char szPath[MAX_PATH]{};
	WideCharToMultiByte(CP_ACP, 0, pHeightMapFilePath, -1, szPath, MAX_PATH, 0, 0);

	// 3채널로 강제 (R, G, B)
	stbi_uc* pImage = stbi_load(szPath, &width, &height, &channels, 3);
	if (!pImage)
		return E_FAIL;

#pragma region VERTEX_BUFFER
	m_iNumVerticesX = width;
	m_iNumVerticesZ = height;
	m_iNumVertices = width * height;
	m_iVertexStride = sizeof(VTXNORTEX);
	m_iFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	m_iNumPritimive = (width - 1) * (height - 1) * 2;
	m_iNumIndices = m_iNumPritimive * 3;
	m_iIndexStride = 4;
	m_eIndexFormat = D3DFMT_INDEX32;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	m_pVertexPositions = new _float3[m_iNumVertices];

	VTXNORTEX* pVertices = nullptr;
	m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);

	for (int z = 0; z < height; ++z)
	{
		for (int x = 0; x < width; ++x)
		{
			int iIndex = z * width + x;
			int pixelIndex = iIndex * 3; // 3 channels (R, G, B)

			BYTE r = pImage[pixelIndex];
			float fHeight = r * 0.1f; // 밝기 → 높이 변환

			m_pVertexPositions[iIndex] = pVertices[iIndex].vPosition = _float3(x, fHeight , z);
			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexcoord = _float2(x / (width - 1.f) * 1.f, z / (height - 1.f) * 1.f);
		}
	}

	m_pVB->Unlock();
	stbi_image_free(pImage); // 메모리 해제
#pragma endregion

#pragma region INDEX_BUFFER
	m_pIndices = new _uint[m_iNumIndices];

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	_uint* pIndices = { nullptr };

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

	_uint		iNumIndices = { 0 };

	for (size_t i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint			iIndex = i * m_iNumVerticesX + j;

			_uint			iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			_float3		vSourDir, vDestDir, vNormal;

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[1];
			pIndices[iNumIndices++] = iIndices[2];

			vSourDir = pVertices[iIndices[1]].vPosition - pVertices[iIndices[0]].vPosition;
			vDestDir = pVertices[iIndices[2]].vPosition - pVertices[iIndices[1]].vPosition;

			D3DXVec3Normalize(&vNormal, D3DXVec3Cross(&vNormal, &vSourDir, &vDestDir));

			pVertices[iIndices[0]].vNormal += vNormal;
			pVertices[iIndices[1]].vNormal += vNormal;
			pVertices[iIndices[2]].vNormal += vNormal;



			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[2];
			pIndices[iNumIndices++] = iIndices[3];


			vSourDir = pVertices[iIndices[2]].vPosition - pVertices[iIndices[0]].vPosition;
			vDestDir = pVertices[iIndices[3]].vPosition - pVertices[iIndices[2]].vPosition;

			D3DXVec3Normalize(&vNormal, D3DXVec3Cross(&vNormal, &vSourDir, &vDestDir));

			pVertices[iIndices[0]].vNormal += vNormal;
			pVertices[iIndices[2]].vNormal += vNormal;
			pVertices[iIndices[3]].vNormal += vNormal;
		}
	}

	for (size_t i = 0; i < m_iNumVertices; i++)
		D3DXVec3Normalize(&pVertices[i].vNormal, &pVertices[i].vNormal);
	memcpy(m_pIndices, pIndices, m_iIndexStride * m_iNumIndices);
	m_pVB->Unlock();
	m_pIB->Unlock();

#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize(void* pArg)
{
	return S_OK;
}

_float3 CVIBuffer_Terrain::Compute_HeightPosition(const _float3& vPosition)
{
	_uint	iIndex = static_cast<_uint>(vPosition.z) * m_iNumVerticesX + static_cast<_uint>(vPosition.x);

	_uint	iIndices[4] = {
		iIndex + m_iNumVerticesX, 
		iIndex + m_iNumVerticesX + 1,
		iIndex + 1,
		iIndex
	};

	_float		fWidth = vPosition.x - m_pVertexPositions[iIndices[0]].x;
	_float		fDepth = m_pVertexPositions[iIndices[0]].z - vPosition.z;

	D3DXPLANE		Plane = {};

	/*
	직선의 방정식 : ax + by + c = 0
	y = 기울기x + y절편;

	평면의 방정식 : ax + by + cz + d = 0
	*/

	/* 
	y = (-ax - cz - d) / b
	*/

	/* 오른쪽 위 삼각형 안에 있다. */
	if (fWidth > fDepth)
	{
		// D3DXPlaneFromPointNormal(&Plane, );
		D3DXPlaneFromPoints(&Plane, &m_pVertexPositions[iIndices[0]], &m_pVertexPositions[iIndices[1]], &m_pVertexPositions[iIndices[2]]);	 
		
	}

	/*왼쪽 하단 삼각형에 있다. */
	else
	{
		D3DXPlaneFromPoints(&Plane, &m_pVertexPositions[iIndices[0]], &m_pVertexPositions[iIndices[2]], &m_pVertexPositions[iIndices[3]]);
	}

	return _float3(vPosition.x, (-Plane.a * vPosition.x - Plane.c * vPosition.z - Plane.d) / Plane.b, vPosition.z);
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

CVIBuffer_Terrain* CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pHeightMapFilePath)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pGraphic_Device);

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
		MSG_BOX("Failed to Created : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();


}
