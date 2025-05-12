#include "Mesh.h"

CMesh::CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer{ pDevice, pContext }
{
}

CMesh::CMesh(const CMesh& Prototype)
	: CVIBuffer(Prototype)
{
}

HRESULT CMesh::Initialize_Prototype(vector<_float3>& positions,
	vector<_float3>& normals,
	vector<_float3>& tangents,
	vector<_float2>& uvs,
	vector<_uint>& indices,
	_uint materialIndex)
{
	m_iMaterialIndex = materialIndex;
	m_iNumVertexBuffers = 1;
	m_iNumVertices = static_cast<_uint>(positions.size());

	m_iVertexStride = sizeof(VTXMESH);
	m_iNumIndices = static_cast<_uint>(indices.size());
	m_iIndexStride = sizeof(_uint);
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// Vertex Buffer
	D3D11_BUFFER_DESC VBBufferDesc = {};
	VBBufferDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
	VBBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VBBufferDesc.CPUAccessFlags = 0;
	VBBufferDesc.StructureByteStride = m_iVertexStride;

	std::vector<VTXMESH> vertices(m_iNumVertices);
	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		vertices[i].vPosition = positions[i];
		vertices[i].vNormal = normals[i];
		vertices[i].vTangent = tangents[i];
		vertices[i].vTexcoord = uvs[i];
	}

	// Vertex Positions 따로 저장
	m_pVertexPositions = new _float3[m_iNumVertices];
	memcpy(m_pVertexPositions, positions.data(), sizeof(_float3) * m_iNumVertices);

	D3D11_SUBRESOURCE_DATA VBInitialData = {};
	VBInitialData.pSysMem = vertices.data();

	if (FAILED(m_pDevice->CreateBuffer(&VBBufferDesc, &VBInitialData, &m_pVB)))
		return E_FAIL;

	// Index Buffer
	D3D11_BUFFER_DESC IBBufferDesc = {};
	IBBufferDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
	IBBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IBBufferDesc.CPUAccessFlags = 0;
	IBBufferDesc.StructureByteStride = m_iIndexStride;

	D3D11_SUBRESOURCE_DATA IBInitialData = {};
	IBInitialData.pSysMem = indices.data();

	if (FAILED(m_pDevice->CreateBuffer(&IBBufferDesc, &IBInitialData, &m_pIB)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMesh::Initialize(void* pArg)
{
	return S_OK;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	vector<_float3>& positions, vector<_float3>& normals, vector<_float3>& tangents, vector<_float2>& uvs, vector<_uint>& indices, _uint materialIndex)
{
	CMesh* pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(positions, normals, tangents, uvs, indices, materialIndex)))
	{
		MSG_BOX("Failed to Create : CMesh (Binary)");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent* CMesh::Clone(void* pArg)
{
	CMesh* pInstance = new CMesh(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMesh::Free()
{
	__super::Free();


}
