#include "VIBuffer.h"
#include "GameInstance.h"

CVIBuffer::CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{ pDevice, pContext }
{
}

CVIBuffer::CVIBuffer(const CVIBuffer& Prototype)
	: CComponent(Prototype)
	, m_pVB { Prototype.m_pVB }
	, m_pIB { Prototype.m_pIB }
	, m_iNumVertices { Prototype.m_iNumVertices }
	, m_iVertexStride { Prototype.m_iVertexStride }
	, m_iNumIndices { Prototype.m_iNumIndices }
	, m_iIndexStride { Prototype.m_iIndexStride }
	, m_pVertexPositions { Prototype.m_pVertexPositions }
	, m_iNumVertexBuffers { Prototype.m_iNumVertexBuffers }
	, m_eIndexFormat{ Prototype.m_eIndexFormat }
	, m_ePrimitiveTopology{ Prototype.m_ePrimitiveTopology }
	, m_iNumPritimive{ Prototype.m_iNumPritimive }
	, m_pIndices{ Prototype.m_pIndices }
{
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

HRESULT CVIBuffer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer::Initialize(void* pArg)
{
	return S_OK;
}


HRESULT CVIBuffer::Bind_Buffers()
{
	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB, 
		/*m_pVB1,
		m_pVB2,*/
	};

	_uint		iVertexStrides[] = {
		m_iVertexStride, 
	};

	_uint		iOffsets[] = {
		0
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iVertexStrides, iOffsets);	
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_ePrimitiveTopology);

	/*
	LPCSTR SemanticName;
    UINT SemanticIndex;
    DXGI_FORMAT Format;
    UINT InputSlot;
    UINT AlignedByteOffset;
    D3D11_INPUT_CLASSIFICATION InputSlotClass;
    UINT InstanceDataStepRate;
	*/
	/*D3D11_INPUT_ELEMENT_DESC		Elements[6] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	_uint		iNumElements = 2;*/

	//m_pDevice->CreateInputLayout(Elements, iNumElements, );

	//m_pContext->IASetInputLayout();

	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	m_pContext->DrawIndexed(m_iNumIndices, 0, 0);

	//// ===== 선 렌더링 추가 =====
	//if (m_pVertexPositions && m_iNumVertices >= 2)
	//{
	//	struct LineVertex
	//	{
	//		DirectX::XMFLOAT3 vPos;
	//		DirectX::XMFLOAT4 vColor;
	//	};

	//	// 첫 번째와 마지막 정점을 사용
	//	LineVertex lineVertices[2] =
	//	{
	//		{ m_pVertexPositions[0], {1.f, 0.f, 0.f, 1.f} }, // 빨간색
	//		{ m_pVertexPositions[m_iNumVertices - 1], {1.f, 0.f, 0.f, 1.f} }
	//	};

	//	// 버텍스 버퍼 생성
	//	D3D11_BUFFER_DESC bufferDesc = {};
	//	bufferDesc.ByteWidth = sizeof(lineVertices);
	//	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	//	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	//	D3D11_SUBRESOURCE_DATA initData = {};
	//	initData.pSysMem = lineVertices;

	//	ID3D11Buffer* pLineVB = nullptr;
	//	if (FAILED(m_pDevice->CreateBuffer(&bufferDesc, &initData, &pLineVB)))
	//		return E_FAIL;

	//	UINT stride = sizeof(LineVertex);
	//	UINT offset = 0;

	//	// IA 설정
	//	m_pContext->IASetVertexBuffers(0, 1, &pLineVB, &stride, &offset);
	//	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	//	// 셰이더 및 입력 레이아웃은 따로 설정되어 있다고 가정
	//	m_pContext->Draw(2, 0); // 선 1개: 정점 2개

	//	Safe_Release(pLineVB);
	//}
	//// ==========================

	return S_OK;
}

_float3 CVIBuffer::Compute_PickedPosition(const _matrix& pWorldMatrixInverse)
{
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

void CVIBuffer::Free()
{
	__super::Free();

	if (false == m_isCloned)
	{
		Safe_Delete_Array(m_pVertexPositions);
		Safe_Delete_Array(m_pIndices);
	}

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}
