#include "VIBuffer_Terrain.h"
#include "GameInstance.h"
#include "QuadTreeNode.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CVIBuffer { pDevice, pContext }
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain& Prototype)
    : CVIBuffer(Prototype)
	, m_iNumVerticesX { Prototype.m_iNumVerticesX }
	, m_iNumVerticesZ { Prototype.m_iNumVerticesZ }
	, m_Offset{ Prototype.m_Offset }
	, m_pQuadTreeRoot{ Prototype.m_pQuadTreeRoot }
{
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(const _tchar* pHeightMapFilePath, _float2 Offset)
{	
	m_Offset = Offset;

	filesystem::path path = pHeightMapFilePath;
	if (path.extension().wstring() == L".bmp")
		Read_HeightMap_BMP(pHeightMapFilePath, Offset);
	else if (path.extension().wstring() == L".png")
		Read_HeightMap_PNG(pHeightMapFilePath, Offset);
	else
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CVIBuffer_Terrain::Render()
{
	//터레인 꼭짓점이 카메라 내에 들어왔는가?
	//m_pVertexPositions[0], m_pVertexPositions[m_iNumVerticesX -1]
	//m_pVertexPositions[m_iNumVerticesX * (m_iNumVerticesZ - 1)], m_pVertexPositions[m_iNumVerticesX * m_iNumVerticesZ - 1]


	/*if (m_pGameInstance->Is_In_Frustum(XMLoadFloat3(&m_pVertexPositions[0])))
	{
		
	}*/
	m_pContext->DrawIndexed(m_iNumIndices, 0, 0);


	return S_OK;
}

_vector CVIBuffer_Terrain::Compute_HeightPosition(const _vector& vPosition)
{
	_uint iIndex = static_cast<_uint>(XMVectorGetZ(vPosition) / m_Offset.x) * m_iNumVerticesX + static_cast<_uint>(XMVectorGetX(vPosition) / m_Offset.x);

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

_vector CVIBuffer_Terrain::Compute_NormalPosition(const _vector& vPosition)
{
	// 기준 인덱스 계산
	_uint iIndex = static_cast<_uint>(XMVectorGetZ(vPosition) / m_Offset.x) * m_iNumVerticesX
		+ static_cast<_uint>(XMVectorGetX(vPosition) / m_Offset.x);

	// 주변 정점 인덱스 (사각형 4개)
	_uint iIndices[4] = {
		iIndex + m_iNumVerticesX,         // 0: 좌하단
		iIndex + m_iNumVerticesX + 1,     // 1: 우하단
		iIndex + 1,                       // 2: 우상단
		iIndex                             // 3: 좌상단
	};

	// 정점 위치에서 가로/세로 거리 측정
	_float fWidth = XMVectorGetX(vPosition) - m_pVertexPositions[iIndices[0]].x;
	_float fDepth = m_pVertexPositions[iIndices[0]].z - XMVectorGetZ(vPosition);

	// 기준 삼각형 선택
	_vector v0 = XMLoadFloat3(&m_pVertexPositions[iIndices[0]]);
	_vector v1, v2;

	if (fWidth > fDepth) // 오른쪽 위 삼각형 (0, 1, 2)
	{
		v1 = XMLoadFloat3(&m_pVertexPositions[iIndices[1]]);
		v2 = XMLoadFloat3(&m_pVertexPositions[iIndices[2]]);
	}
	else // 왼쪽 아래 삼각형 (0, 2, 3)
	{
		v1 = XMLoadFloat3(&m_pVertexPositions[iIndices[2]]);
		v2 = XMLoadFloat3(&m_pVertexPositions[iIndices[3]]);
	}

	// 두 벡터로부터 노말 계산
	_vector vEdge1 = v1 - v0;
	_vector vEdge2 = v2 - v0;

	_vector vNormal = XMVector3Normalize(XMVector3Cross(vEdge1, vEdge2));
	return vNormal; // w값은 안 씀
}

_float3 CVIBuffer_Terrain::Compute_PickedPosition(const _matrix& pWorldMatrixInverse)
{
	
	//터레인 꼭짓점이 카메라 내에 들어왔는가?
	//m_pVertexPositions[0], m_pVertexPositions[m_iNumVerticesX -1]
	//m_pVertexPositions[m_iNumVerticesX * (m_iNumVerticesZ - 1)], m_pVertexPositions[m_iNumVerticesX * m_iNumVerticesZ - 1]

	//return Pick_Quad(m_pVertexPositions[0], m_pVertexPositions[m_iNumVerticesX - 1], m_pVertexPositions[m_iNumVerticesX * (m_iNumVerticesZ - 1)], m_pVertexPositions[m_iNumVerticesX * m_iNumVerticesZ - 1], pWorldMatrixInverse);

	_uint   iIndices[3] = {};
	_float3 vPickedPos = {};

	/* 컴퓨트 포지션을 호출 할 때 객체의 월드매트릭스의 역행렬을 받도록 해서, */
	/* 마우스 좌표를 로컬로 바꿔준 후에 피킹함수를 호출 해 준다 */
	m_pGameInstance->Transform_Picking_ToLocalSpace(pWorldMatrixInverse);

	for (size_t i = 0; i < m_iNumPritimive; i++)
	{
		_byte* pIndices = static_cast<_byte*>(m_pIndices) + m_iIndexStride * i * 3;

		memcpy(&iIndices[0], pIndices, m_iIndexStride);
		memcpy(&iIndices[1], pIndices + m_iIndexStride, m_iIndexStride);
		memcpy(&iIndices[2], pIndices + m_iIndexStride * 2, m_iIndexStride);

		if (true == m_pGameInstance->Picking_InLocal(vPickedPos, m_pVertexPositions[iIndices[0]], m_pVertexPositions[iIndices[1]], m_pVertexPositions[iIndices[2]]))
			break;
	}

	return vPickedPos;
}

_bool CVIBuffer_Terrain::PickQuadTreeNode(_float3& vOutPos, _float& vOutNearestDist, _uint& iOutPickedTriangleIndex)
{
	_float fNearestDist = FLT_MAX;

	_float fDistAABB;
	if (!RayIntersectAABB(m_pQuadTreeRoot->Get_Min(), m_pQuadTreeRoot->Get_Max(), fDistAABB))
		return false;

	_bool bHit = false;

	if (m_pQuadTreeRoot->Is_Leaf())
	{
		// 삼각형 하나씩 검사
		for (auto triIdx : m_pQuadTreeRoot->Get_Indices())
		{
			const _float3& v0 = m_pVertexPositions[reinterpret_cast<_uint*>(m_pIndices)[triIdx * 3 + 0]];
			const _float3& v1 = m_pVertexPositions[reinterpret_cast<_uint*>(m_pIndices)[triIdx * 3 + 1]];
			const _float3& v2 = m_pVertexPositions[reinterpret_cast<_uint*>(m_pIndices)[triIdx * 3 + 2]];

			_float distTri;
			if (RayIntersectTriangle(v0, v1, v2, distTri))
			{
				if (distTri < fNearestDist)
				{
					fNearestDist = distTri;
					iOutPickedTriangleIndex = triIdx;
					bHit = true;
				}
			}
		}
	}
	else
	{
		// 자식 노드 재귀 검사
		for (_int i = 0; i < 4; ++i)
		{
			if (m_pQuadTreeRoot->Get_Children()[i])
			{
				if (PickQuadTreeNode(m_pQuadTreeRoot->Get_Children()[i], m_pVertexPositions, reinterpret_cast<_uint*>(m_pIndices), fNearestDist, iOutPickedTriangleIndex))
				{
					bHit = true;
					vOutNearestDist = fNearestDist;
					XMStoreFloat3(&vOutPos, XMLoadFloat3(&m_pGameInstance->Get_MousePos()) + XMLoadFloat3(&m_pGameInstance->Get_MouseRay()) * fNearestDist);
				}

			}
		}
	}
	return bHit;
}

void CVIBuffer_Terrain::ComputeTriangleAABB(const _float3& v0, const _float3& v1, const _float3& v2, _float3& outMin, _float3& outMax)
{
	// 최소값 구하기
	_float minX = (v0.x < v1.x) ? ((v0.x < v2.x) ? v0.x : v2.x) : ((v1.x < v2.x) ? v1.x : v2.x);
	_float minY = (v0.y < v1.y) ? ((v0.y < v2.y) ? v0.y : v2.y) : ((v1.y < v2.y) ? v1.y : v2.y);
	_float minZ = (v0.z < v1.z) ? ((v0.z < v2.z) ? v0.z : v2.z) : ((v1.z < v2.z) ? v1.z : v2.z);

	// 최대값 구하기
	_float maxX = (v0.x > v1.x) ? ((v0.x > v2.x) ? v0.x : v2.x) : ((v1.x > v2.x) ? v1.x : v2.x);
	_float maxY = (v0.y > v1.y) ? ((v0.y > v2.y) ? v0.y : v2.y) : ((v1.y > v2.y) ? v1.y : v2.y);
	_float maxZ = (v0.z > v1.z) ? ((v0.z > v2.z) ? v0.z : v2.z) : ((v1.z > v2.z) ? v1.z : v2.z);

	outMin = _float3(minX, minY, minZ);
	outMax = _float3(maxX, maxY, maxZ);

}

_bool CVIBuffer_Terrain::AABBOverlap(const _float3& vMinA, const _float3& vMaxA, const _float3& vMinB, const _float3& vMaxB)
{
	return !(vMinA.x > vMaxB.x || vMaxA.x < vMinB.x ||
		vMinA.y > vMaxB.y || vMaxA.y < vMinB.y ||
		vMinA.z > vMaxB.z || vMaxA.z < vMinB.z);
}

void CVIBuffer_Terrain::BuildQuadTree(CQuadTreeNode* pNode, _uint iDepth, const _float3* pPositions, const _uint* pIndices, _uint iNumIndices)
{
	if (iDepth >= iMax_QuadTree_Depth || pNode->Get_Indices().size() <= iMax_Triangles_Per_Node)
	{
		pNode->Set_Leaf();
		return;
	}

	

	_float3 center = {};
	XMStoreFloat3(&center, XMLoadFloat3(&pNode->Get_Min()) + XMLoadFloat3(&pNode->Get_Max()) * 0.5f);

	// 자식 영역 AABB 설정
	_float3 min = pNode->Get_Min();
	_float3 max = pNode->Get_Max();

	_float3 childMins[4] = {
		{min.x, min.y, min.z},            // 좌상
		{center.x, min.y, min.z},         // 우상
		{min.x, min.y, center.z},         // 좌하
		{center.x, min.y, center.z},      // 우하
	};

	_float3 childMaxs[4] = {
		{center.x, max.y, center.z},
		{max.x, max.y, center.z},
		{center.x, max.y, max.z},
		{max.x, max.y, max.z},
	};

	// 자식 노드 생성
	for (int i = 0; i < 4; ++i)
	{
		CQuadTreeNode* pChild = CQuadTreeNode::Create();
		pChild->Get_Min() = childMins[i];
		pChild->Get_Max() = childMaxs[i];

		for (auto triIdx : pNode->Get_Indices())
		{
			_float3 v0 = pPositions[pIndices[triIdx * 3 + 0]];
			_float3 v1 = pPositions[pIndices[triIdx * 3 + 1]];
			_float3 v2 = pPositions[pIndices[triIdx * 3 + 2]];

			_float3 triMin, triMax;
			ComputeTriangleAABB(v0, v1, v2, triMin, triMax);

			if (AABBOverlap(pChild->Get_Min(), pChild->Get_Max(), triMin, triMax))
				pChild->Get_Indices().push_back(triIdx);
		}

		if (!pChild->Get_Indices().empty())
		{
			BuildQuadTree(pChild, iDepth + 1, pPositions, pIndices, iNumIndices);
			pNode->Get_Children()[i] = pChild;
		}
		else
		{
			Safe_Release(pChild);
		}
	}

	pNode->Get_Indices().clear(); // 자식으로 분산 후 비움
}

CQuadTreeNode* CVIBuffer_Terrain::CreateTerrainQuadTree(const _float3* pPositions, const _uint* pIndices, _uint iNumVertices, _uint iNumIndices)
{
	// 전체 지형 AABB 계산
	_float3 vMinPos = pPositions[0];
	_float3 vMaxPos = pPositions[0];

	for (_uint i = 1; i < iNumVertices; ++i)
	{
		const _float3& vPos = pPositions[i];

		// minPos 업데이트
		if (vPos.x < vMinPos.x) vMinPos.x = vPos.x;
		if (vPos.y < vMinPos.y) vMinPos.y = vPos.y;
		if (vPos.z < vMinPos.z) vMinPos.z = vPos.z;

		// maxPos 업데이트
		if (vPos.x > vMaxPos.x) vMaxPos.x = vPos.x;
		if (vPos.y > vMaxPos.y) vMaxPos.y = vPos.y;
		if (vPos.z > vMaxPos.z) vMaxPos.z = vPos.z;

	}

	CQuadTreeNode* pRoot = CQuadTreeNode::Create();
	pRoot->Get_Min() = vMinPos;
	pRoot->Get_Max() = vMaxPos;

	_uint iNumTriangles = iNumIndices / 3;
	pRoot->Get_Indices().resize(iNumTriangles);
	for (_uint i = 0; i < iNumTriangles; ++i)
		pRoot->Get_Indices()[i] = i;

	BuildQuadTree(pRoot, 0, pPositions, pIndices, iNumIndices);
	return pRoot;
}

_bool CVIBuffer_Terrain::RayIntersectAABB(const _float3& vBoxMin, const _float3& vBoxMax, _float& vOutDistance)
{
	// 레이 시작점과 방향 벡터를 가져옴
	const _float3 vRayOrigin = m_pGameInstance->Get_MousePos();
	const _float3 vRayDir = m_pGameInstance->Get_MouseRay();

	// X축에서의 진입(fTMin)과 이탈(fTMax) 거리 계산
	_float fTMin = (vBoxMin.x - vRayOrigin.x) / vRayDir.x;
	_float fTMax = (vBoxMax.x - vRayOrigin.x) / vRayDir.x;
	if (fTMin > fTMax) swap(fTMin, fTMax); // 방향이 반대일 수 있으므로 정렬

	// Y축에서의 진입과 이탈 거리 계산
	_float tymin = (vBoxMin.y - vRayOrigin.y) / vRayDir.y;
	_float tymax = (vBoxMax.y - vRayOrigin.y) / vRayDir.y;
	if (tymin > tymax) swap(tymin, tymax); // 정렬

	// X축과 Y축의 간격이 겹치지 않으면 AABB와 충돌 안 함
	if ((fTMin > tymax) || (tymin > fTMax))
		return false;

	// X, Y축 결과 통합
	if (tymin > fTMin)
		fTMin = tymin;
	if (tymax < fTMax)
		fTMax = tymax;

	// Z축에서의 진입과 이탈 거리 계산
	_float fTZMin = (vBoxMin.z - vRayOrigin.z) / vRayDir.z;
	_float fTZMax = (vBoxMax.z - vRayOrigin.z) / vRayDir.z;
	if (fTZMin > fTZMax) swap(fTZMin, fTZMax); // 정렬

	// X, Y, Z축의 간격이 겹치지 않으면 충돌 안 함
	if ((fTMin > fTZMax) || (fTZMin > fTMax))
		return false;

	// X, Y, Z 결과 통합
	if (fTZMin > fTMin)
		fTMin = fTZMin;
	if (fTZMax < fTMax)
		fTMax = fTZMax;

	// AABB가 레이 뒤쪽에 있음
	if (fTMax < 0)
		return false;

	// 최종 충돌 거리 저장 (앞쪽이면 tmin, 내부에서 시작하면 tmax)
	vOutDistance = (fTMin >= 0) ? fTMin : fTMax;

	return true; // 충돌 있음
}

_bool CVIBuffer_Terrain::RayIntersectTriangle(const _float3& v0, const _float3& v1, const _float3& v2, _float& vOutDistance)
{
	const _float3& rayOrigin = m_pGameInstance->Get_MousePos();
	const _float3& rayDir = m_pGameInstance->Get_MouseRay();

	const _float fEPSILON = numeric_limits<_float>::epsilon();

	// 벡터 로딩
	_vector vecOrigin = XMLoadFloat3(&rayOrigin);
	_vector vecDir = XMLoadFloat3(&rayDir);
	_vector vecV0 = XMLoadFloat3(&v0);
	_vector vecV1 = XMLoadFloat3(&v1);
	_vector vecV2 = XMLoadFloat3(&v2);

	// 에지 벡터 계산
	_vector edge1 = vecV1 - vecV0;
	_vector edge2 = vecV2 - vecV0;

	// 평면과의 교차 검사
	_vector h = XMVector3Cross(vecDir, edge2);
	_float a = XMVectorGetX(XMVector3Dot(edge1, h));

	if (fabs(a) < fEPSILON)
		return false;  // 레이와 삼각형이 평행

	_float f = 1.0f / a;
	_vector s = vecOrigin - vecV0;
	_float u = f * XMVectorGetX(XMVector3Dot(s, h));

	if (u < 0.0f || u > 1.0f)
		return false;

	_vector q = XMVector3Cross(s, edge1);
	_float v = f * XMVectorGetX(XMVector3Dot(vecDir, q));

	if (v < 0.0f || u + v > 1.0f)
		return false;

	_float t = f * XMVectorGetX(XMVector3Dot(edge2, q));

	if (t > fEPSILON)
	{
		vOutDistance = t;
		return true;
	}

	return false;
}

_bool CVIBuffer_Terrain::PickQuadTreeNode(CQuadTreeNode* pNode, const _float3* pPositions, const _uint* pIndices, _float& vOutNearestDist, _uint& iOutPickedTriangleIndex)
{
	const _float3& vRayOrigin = m_pGameInstance->Get_MousePos();
	const _float3& vRayDir = m_pGameInstance->Get_MouseRay();

	_float fDistAABB;
	if (!RayIntersectAABB(pNode->Get_Min(), pNode->Get_Max(), fDistAABB))
		return false;

	_bool bHit = false;

	if (pNode->Is_Leaf())
	{
		// 삼각형 하나씩 검사
		for (auto iTri_Index : pNode->Get_Indices())
		{
			const _float3& v0 = pPositions[pIndices[iTri_Index * 3 + 0]];
			const _float3& v1 = pPositions[pIndices[iTri_Index * 3 + 1]];
			const _float3& v2 = pPositions[pIndices[iTri_Index * 3 + 2]];

			_float fDistTri;
			if (RayIntersectTriangle(v0, v1, v2, fDistTri))
			{
				if (fDistTri < vOutNearestDist)
				{
					vOutNearestDist = fDistTri;
					iOutPickedTriangleIndex = iTri_Index;
					bHit = true;
				}
			}
		}
	}
	else
	{
		// 자식 노드 재귀 검사
		for (_uint i = 0; i < 4; ++i)
		{
			if (pNode->Get_Children()[i])
			{
				if (PickQuadTreeNode(pNode->Get_Children()[i], pPositions, pIndices, vOutNearestDist, iOutPickedTriangleIndex))
					bHit = true;
			}
		}
	}
	return bHit;
}

HRESULT CVIBuffer_Terrain::Read_HeightMap_BMP(const _tchar* pHeightMapFilePath, _float2 Offset)
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
				static_cast<_float>(j * Offset.x),
				static_cast<_float>((pPixels[iIndex] & 0x000000ff) * Offset.y),
				static_cast<_float>(i * Offset.x));

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

	m_pQuadTreeRoot = CreateTerrainQuadTree(m_pVertexPositions, reinterpret_cast<_uint*>(m_pIndices), m_iNumVertices, m_iNumIndices);

	CloseHandle(hFile);
	Safe_Delete_Array(pPixels);

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Read_HeightMap_PNG(const _tchar* pHeightMapFilePath, _float2 Offset)
{
    IWICImagingFactory* pWICFactory = nullptr;
    IWICBitmapDecoder* pDecoder = nullptr;
    IWICBitmapFrameDecode* pFrame = nullptr;
    IWICFormatConverter* pConverter = nullptr;

    // WIC 팩토리 생성
    HRESULT hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&pWICFactory));
    if (FAILED(hr)) return E_FAIL;

    // PNG 파일 디코더 생성
    hr = pWICFactory->CreateDecoderFromFilename(
        pHeightMapFilePath,
        nullptr,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        &pDecoder);
    if (FAILED(hr)) return E_FAIL;

    // 첫 번째 프레임 가져오기
    hr = pDecoder->GetFrame(0, &pFrame);
    if (FAILED(hr)) return E_FAIL;

    // 포맷 변환 (32bppRGBA로)
    hr = pWICFactory->CreateFormatConverter(&pConverter);
    if (FAILED(hr)) return E_FAIL;

    hr = pConverter->Initialize(
        pFrame,
        GUID_WICPixelFormat32bppRGBA,
        WICBitmapDitherTypeNone,
        nullptr,
        0.0,
        WICBitmapPaletteTypeCustom);
    if (FAILED(hr)) return E_FAIL;

    // 이미지 크기 가져오기
    _uint width = 0, height = 0;
    pFrame->GetSize(&width, &height);

    m_iNumVerticesX = width;
    m_iNumVerticesZ = height;
    m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;

    // 픽셀 데이터 읽기 (RGBA 4바이트 * 전체 픽셀 수)
	_uint bufferSize = width * height * 4;
    _ubyte* pPixels = new _ubyte[bufferSize];
    hr = pConverter->CopyPixels(
        nullptr,
        width * 4,    // stride
        bufferSize,
        pPixels);
    if (FAILED(hr)) return E_FAIL;

    // 버텍스 버퍼 세팅
    m_iNumVertexBuffers = 1;
    m_iVertexStride = sizeof(VTXNORTEX);
    m_iNumPritimive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;
    m_iNumIndices = m_iNumPritimive * 3;
    m_iIndexStride = sizeof(_uint);
    m_eIndexFormat = DXGI_FORMAT_R32_UINT;
    m_ePrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    // 버텍스 데이터 생성
    VTXNORTEX* pVertices = new VTXNORTEX[m_iNumVertices];
    ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertices);

    m_pVertexPositions = new _float3[m_iNumVertices];
    ZeroMemory(m_pVertexPositions, sizeof(_float3) * m_iNumVertices);

    for (_uint i = 0; i < m_iNumVerticesZ; ++i)
    {
        for (_uint j = 0; j < m_iNumVerticesX; ++j)
        {
			_uint index = i * m_iNumVerticesX + j;
			_uint pixelIndex = index * 4; // RGBA

            _ubyte r = pPixels[pixelIndex];  // R 값 사용 (높이)
            _float heightValue = static_cast<_float>(r);

            pVertices[index].vPosition = _float3(
                static_cast<_float>(j * Offset.x),
                heightValue * Offset.y,
                static_cast<_float>(i * Offset.x));

            pVertices[index].vNormal = _float3(0.f, 0.f, 0.f);
            pVertices[index].vTexcoord = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
        }
    }

    for (_uint i = 0; i < m_iNumVertices; ++i)
        m_pVertexPositions[i] = pVertices[i].vPosition;

    // Vertex Buffer 생성
    D3D11_BUFFER_DESC VBBufferDesc{};
    VBBufferDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
    VBBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    VBBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    VBBufferDesc.CPUAccessFlags = 0;
    VBBufferDesc.StructureByteStride = m_iVertexStride;

    D3D11_SUBRESOURCE_DATA VBInitialData{};
    VBInitialData.pSysMem = pVertices;

    if (FAILED(m_pDevice->CreateBuffer(&VBBufferDesc, &VBInitialData, &m_pVB)))
        return E_FAIL;

    Safe_Delete_Array(pVertices);

    // Index Buffer 생성 (원래랑 동일)
    D3D11_BUFFER_DESC IBBufferDesc{};
    IBBufferDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
    IBBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    IBBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    IBBufferDesc.CPUAccessFlags = 0;
    IBBufferDesc.StructureByteStride = m_iIndexStride;

    m_pIndices = new _uint[m_iNumIndices];
    ZeroMemory(m_pIndices, sizeof(_uint) * m_iNumIndices);

	_uint iNumIndices = 0;
    for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
    {
        for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
        {
			_uint iIndex = i * m_iNumVerticesX + j;

			_uint indices[4] = {
                iIndex + m_iNumVerticesX,
                iIndex + m_iNumVerticesX + 1,
                iIndex + 1,
                iIndex
            };

			reinterpret_cast<_uint*>(m_pIndices)[iNumIndices++] = indices[0];
			reinterpret_cast<_uint*>(m_pIndices)[iNumIndices++] = indices[1];
			reinterpret_cast<_uint*>(m_pIndices)[iNumIndices++] = indices[2];

			reinterpret_cast<_uint*>(m_pIndices)[iNumIndices++] = indices[0];
			reinterpret_cast<_uint*>(m_pIndices)[iNumIndices++] = indices[2];
			reinterpret_cast<_uint*>(m_pIndices)[iNumIndices++] = indices[3];
        }
    }

    D3D11_SUBRESOURCE_DATA IBInitialData{};
    IBInitialData.pSysMem = m_pIndices;

    if (FAILED(m_pDevice->CreateBuffer(&IBBufferDesc, &IBInitialData, &m_pIB)))
        return E_FAIL;

	m_pQuadTreeRoot = CreateTerrainQuadTree(m_pVertexPositions, reinterpret_cast<_uint*>(m_pIndices), m_iNumVertices, m_iNumIndices);


    // 리소스 해제
    Safe_Delete_Array(pPixels);
    if (pConverter) pConverter->Release();
    if (pFrame) pFrame->Release();
    if (pDecoder) pDecoder->Release();
    if (pWICFactory) pWICFactory->Release();

    return S_OK;
}



CVIBuffer_Terrain* CVIBuffer_Terrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pHeightMapFilePath, _float2 fOffset)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pHeightMapFilePath, fOffset)))
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

	if(!m_isCloned)
		Safe_Release(m_pQuadTreeRoot);
}
