#include "VIBuffer_Cube.h"

CVIBuffer_Cube::CVIBuffer_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer{ pGraphic_Device }
{
}

CVIBuffer_Cube::CVIBuffer_Cube(const CVIBuffer_Cube& Prototype)
	: CVIBuffer{ Prototype }
{
}

HRESULT CVIBuffer_Cube::Initialize_Prototype()
{
    m_iNumVertices = 24; // �� �鿡 ���� 4���� ������, �� 6���� �� -> 24���� ����
    m_iVertexStride = sizeof(VTXPOSTEX);
    m_iFVF = D3DFVF_XYZ | D3DFVF_TEX1;
    m_iNumPritimive = 12; // �� ���� 2���� �ﰢ������ �̷���� ����, 6�� * 2 = 12 �ﰢ��

    m_iIndexStride = 2;
    m_iNumIndices = 36; // 12���� �ﰢ��, 36���� �ε���
    m_eIndexFormat = D3DFMT_INDEX16;

#pragma region VERTEX_BUFFER
    if (FAILED(__super::Create_VertexBuffer()))
        return E_FAIL;

    // Vertices for a Cube (24 vertices, 6 faces)
    VTXPOSTEX* pVertices = { nullptr };

    m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);

    // ���� (Top face)
    pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.5f);  // ���� ���
    pVertices[0].vTexcoord = _float2(0.f, 0.f);

    pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.5f);   // ������ ���
    pVertices[1].vTexcoord = _float2(1.f, 0.f);

    pVertices[2].vPosition = _float3(0.5f, 0.5f, -0.5f);  // ������ �ϴ�
    pVertices[2].vTexcoord = _float2(1.f, 1.f);

    pVertices[3].vPosition = _float3(-0.5f, 0.5f, -0.5f); // ���� �ϴ�
    pVertices[3].vTexcoord = _float2(0.f, 1.f);

    // �Ʒ��� (Bottom face)
    pVertices[4].vPosition = _float3(-0.5f, -0.5f, 0.5f);  // ���� ���
    pVertices[4].vTexcoord = _float2(0.f, 0.f);

    pVertices[5].vPosition = _float3(0.5f, -0.5f, 0.5f);   // ������ ���
    pVertices[5].vTexcoord = _float2(1.f, 0.f);

    pVertices[6].vPosition = _float3(0.5f, -0.5f, -0.5f);  // ������ �ϴ�
    pVertices[6].vTexcoord = _float2(1.f, 1.f);

    pVertices[7].vPosition = _float3(-0.5f, -0.5f, -0.5f); // ���� �ϴ�
    pVertices[7].vTexcoord = _float2(0.f, 1.f);

    // �ո� (Front face)
    pVertices[8].vPosition = _float3(-0.5f, 0.5f, 0.5f);   // ���� ���
    pVertices[8].vTexcoord = _float2(0.f, 0.f);

    pVertices[9].vPosition = _float3(0.5f, 0.5f, 0.5f);    // ������ ���
    pVertices[9].vTexcoord = _float2(1.f, 0.f);

    pVertices[10].vPosition = _float3(0.5f, -0.5f, 0.5f);   // ������ �ϴ�
    pVertices[10].vTexcoord = _float2(1.f, 1.f);

    pVertices[11].vPosition = _float3(-0.5f, -0.5f, 0.5f);  // ���� �ϴ�
    pVertices[11].vTexcoord = _float2(0.f, 1.f);

    // �޸� (Back face)
    pVertices[12].vPosition = _float3(-0.5f, 0.5f, -0.5f);  // ���� ���
    pVertices[12].vTexcoord = _float2(0.f, 0.f);

    pVertices[13].vPosition = _float3(0.5f, 0.5f, -0.5f);   // ������ ���
    pVertices[13].vTexcoord = _float2(1.f, 0.f);

    pVertices[14].vPosition = _float3(0.5f, -0.5f, -0.5f);  // ������ �ϴ�
    pVertices[14].vTexcoord = _float2(1.f, 1.f);

    pVertices[15].vPosition = _float3(-0.5f, -0.5f, -0.5f); // ���� �ϴ�
    pVertices[15].vTexcoord = _float2(0.f, 1.f);

    // ���ʸ� (Left face)
    pVertices[16].vPosition = _float3(-0.5f, 0.5f, -0.5f);  // ���� ���
    pVertices[16].vTexcoord = _float2(0.f, 0.f);

    pVertices[17].vPosition = _float3(-0.5f, 0.5f, 0.5f);   // ������ ���
    pVertices[17].vTexcoord = _float2(1.f, 0.f);

    pVertices[18].vPosition = _float3(-0.5f, -0.5f, 0.5f);  // ������ �ϴ�
    pVertices[18].vTexcoord = _float2(1.f, 1.f);

    pVertices[19].vPosition = _float3(-0.5f, -0.5f, -0.5f); // ���� �ϴ�
    pVertices[19].vTexcoord = _float2(0.f, 1.f);

    // �����ʸ� (Right face)
    pVertices[20].vPosition = _float3(0.5f, 0.5f, -0.5f);   // ���� ���
    pVertices[20].vTexcoord = _float2(0.f, 0.f);

    pVertices[21].vPosition = _float3(0.5f, 0.5f, 0.5f);    // ������ ���
    pVertices[21].vTexcoord = _float2(1.f, 0.f);

    pVertices[22].vPosition = _float3(0.5f, -0.5f, 0.5f);   // ������ �ϴ�
    pVertices[22].vTexcoord = _float2(1.f, 1.f);

    pVertices[23].vPosition = _float3(0.5f, -0.5f, -0.5f);  // ���� �ϴ�
    pVertices[23].vTexcoord = _float2(0.f, 1.f);

    m_pVB->Unlock();
#pragma endregion

#pragma region INDEX_BUFFER
    if (FAILED(__super::Create_IndexBuffer()))
        return E_FAIL;

    // Index buffer for Cube (12 triangles, 36 indices)
    _ushort* pIndices = { nullptr };

    m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

    // �� �鿡 ���� �ε����� �����մϴ� (�ð����)

    // ����
    pIndices[0] = 0; pIndices[1] = 1; pIndices[2] = 3;
    pIndices[3] = 1; pIndices[4] = 2; pIndices[5] = 3;

    // �Ʒ���
    pIndices[6] = 7; pIndices[7] = 5; pIndices[8] = 4;
    pIndices[9] = 7; pIndices[10] = 6; pIndices[11] = 5;

    // �ո�
    pIndices[12] = 11; pIndices[13] = 9; pIndices[14] = 8;
    pIndices[15] = 11; pIndices[16] = 10; pIndices[17] = 9;

    // �޸�
    pIndices[18] = 12; pIndices[19] = 13; pIndices[20] = 15;
    pIndices[21] = 13; pIndices[22] = 14; pIndices[23] = 15;

    // ���ʸ�
    pIndices[24] = 19; pIndices[25] = 17; pIndices[26] = 16;
    pIndices[27] = 19; pIndices[28] = 18; pIndices[29] = 17;

    // �����ʸ�
    pIndices[30] = 20; pIndices[31] = 21; pIndices[32] = 23;
    pIndices[33] = 21; pIndices[34] = 22; pIndices[35] = 23;

    m_pIB->Unlock();
#pragma endregion

    return S_OK;
}



HRESULT CVIBuffer_Cube::Initialize(void* pArg)
{
	return S_OK;
}

CVIBuffer_Cube* CVIBuffer_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Cube* pInstance = new CVIBuffer_Cube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Cube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Cube::Clone(void* pArg)
{
	CVIBuffer_Cube* pInstance = new CVIBuffer_Cube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Cube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Cube::Free()
{
	__super::Free();


}
