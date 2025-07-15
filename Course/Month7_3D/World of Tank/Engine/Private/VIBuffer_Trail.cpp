#include "VIBuffer_Trail.h"

CVIBuffer_Trail::CVIBuffer_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer{ pDevice, pContext }
{
}

CVIBuffer_Trail::CVIBuffer_Trail(const CVIBuffer_Trail& Prototype)
	: CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_Trail::Initialize_Prototype()
{

	m_iNumVertexBuffers = 1;
	m_iNumVertices = 8;
	m_iVertexStride = sizeof(VTXPOSTEX);
	m_iNumIndices = 18;
	m_iIndexStride = sizeof(_ushort);
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_ePrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	D3D11_BUFFER_DESC			IBBufferDesc{};
	IBBufferDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
	IBBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IBBufferDesc.CPUAccessFlags = /*D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE*/0;
	IBBufferDesc.StructureByteStride = m_iIndexStride;
	IBBufferDesc.MiscFlags = 0;

	_ushort* pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

	//수직 트레일
	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;

	pIndices[3] = 0;
	pIndices[4] = 2;
	pIndices[5] = 3;

	//수평 트레일
	pIndices[6] = 4;
	pIndices[7] = 5;
	pIndices[8] = 6;

	pIndices[9] = 4;
	pIndices[10] = 6;
	pIndices[11] = 7;

	//뒷판
	pIndices[12] = 1;
	pIndices[13] = 5;
	pIndices[14] = 2;

	pIndices[15] = 1;
	pIndices[16] = 2;
	pIndices[17] = 6;

	D3D11_SUBRESOURCE_DATA		IBInitialData{};
	IBInitialData.pSysMem = pIndices;

	if (FAILED(m_pDevice->CreateBuffer(&IBBufferDesc, &IBInitialData, &m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CVIBuffer_Trail::Initialize(void* pArg)
{
	D3D11_BUFFER_DESC			VBBufferDesc{};
	VBBufferDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
	VBBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBBufferDesc.Usage = D3D11_USAGE_DYNAMIC;            // CPU에서 매 프레임 쓰기 가능하도록
	VBBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU 쓰기 권한 부여
	VBBufferDesc.StructureByteStride = m_iVertexStride;
	VBBufferDesc.MiscFlags = 0;

	// 초기 데이터 없이 생성 (DYNAMIC)
	if (FAILED(m_pDevice->CreateBuffer(&VBBufferDesc, nullptr, &m_pVB)))
		return E_FAIL;

	return S_OK;
}

void CVIBuffer_Trail::UpdateTrailBuffer(_fvector vPrevPos, _fvector vCurPos, const _matrix& WorldInverse, _float fTrailWidth)
{
	D3D11_MAPPED_SUBRESOURCE	SubResource{};

	m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXPOSTEX* pVertices = static_cast<VTXPOSTEX*>(SubResource.pData);

	_vector vCur = XMVector3TransformCoord(vCurPos, WorldInverse);
	_vector vPrev = XMVector3TransformCoord(vPrevPos, WorldInverse);

	// 방향
	_vector vDir = vCur - vPrev;
	vDir = XMVector3Normalize(vDir);

	//트레일 너비
	_vector vOffset = XMVectorSet(0.f, fTrailWidth, 0.f, 0.f);

	//수직 트레일
	XMStoreFloat3(&pVertices[0].vPosition, vCur + vOffset);
	pVertices[0].vTexcoord = { 0.f, 0.f };

	XMStoreFloat3(&pVertices[1].vPosition, vPrev + vOffset);
	pVertices[1].vTexcoord = { 1.f, 0.f };

	XMStoreFloat3(&pVertices[2].vPosition, vPrev - vOffset);
	pVertices[2].vTexcoord = { 1.f, 1.f };

	XMStoreFloat3(&pVertices[3].vPosition, vCur - vOffset);
	pVertices[3].vTexcoord = { 0.f, 1.f };

	//수평 트레일
	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector vRight = XMVector3Normalize(XMVector3Cross(vUp, vDir));

	// 트레일 너비만큼 오프셋
	vOffset = vRight * fTrailWidth * 0.5f;

	XMStoreFloat3(&pVertices[4].vPosition, vCur + vOffset);
	pVertices[4].vTexcoord = { 0.f, 0.f };

	XMStoreFloat3(&pVertices[5].vPosition, vPrev + vOffset);
	pVertices[5].vTexcoord = { 1.f, 0.f };

	XMStoreFloat3(&pVertices[6].vPosition, vPrev - vOffset);
	pVertices[6].vTexcoord = { 1.f, 1.f };

	XMStoreFloat3(&pVertices[7].vPosition, vCur - vOffset);
	pVertices[7].vTexcoord = { 0.f, 1.f };

	m_pContext->Unmap(m_pVB, 0);
}

CVIBuffer_Trail* CVIBuffer_Trail::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Trail* pInstance = new CVIBuffer_Trail(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Trail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Trail::Clone(void* pArg)
{
	CVIBuffer_Trail* pInstance = new CVIBuffer_Trail(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Trail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Trail::Free()
{
	__super::Free();


}
