#include "VIBuffer_Point_Instance.h"

#include "GameInstance.h"

CVIBuffer_Point_Instance::CVIBuffer_Point_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer_Instance(pDevice, pContext)
{
}

CVIBuffer_Point_Instance::CVIBuffer_Point_Instance(const CVIBuffer_Point_Instance& Prototype)
	: CVIBuffer_Instance(Prototype)
	, m_pVertexInstances(Prototype.m_pVertexInstances)
	//, m_vPivot{ Prototype.m_vPivot }
	, m_pSpeeds{ Prototype.m_pSpeeds }
	//, m_isLoop{ Prototype.m_isLoop }
	, m_tPointInstanceDesc{ Prototype.m_tPointInstanceDesc }
{

}

HRESULT CVIBuffer_Point_Instance::Initialize_Prototype(const INSTANCE_DESC* pArg)
{
	const POINT_INSTANCE_DESC* pDesc = static_cast<const POINT_INSTANCE_DESC*>(pArg);

	m_tPointInstanceDesc = *pDesc;

	m_eEmissionShape = pDesc->eEmissionShape;
	//m_vPivot = pDesc->vPivot;
	//m_isLoop = pDesc->isLoop;
	m_iNumIndexPerInstance = 1;
	m_iVertexInstanceStride = sizeof(VTXPOS_PARTICLE_INSTANCE);
	m_iNumInstance = pDesc->iNumInstance;

	m_iNumVertexBuffers = 2;
	m_iNumVertices = 1;
	m_iVertexStride = sizeof(VTXPOS);
	m_iNumIndices = m_iNumIndexPerInstance;

	m_iIndexStride = sizeof(_ushort);
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_ePrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;


#pragma region VERTEXBUFFER
	D3D11_BUFFER_DESC			VBBufferDesc{};
	VBBufferDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
	VBBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VBBufferDesc.CPUAccessFlags = /*D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE*/0;
	VBBufferDesc.StructureByteStride = m_iVertexStride;
	VBBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA		VBInitialData{};

	VTXPOS* pVertices = new VTXPOS[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOS) * m_iNumVertices);

	m_pVertexPositions = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertexPositions, sizeof(_float3) * m_iNumVertices);

	pVertices[0].vPosition = _float3(0.0f, 0.0f, 0.f);

	for (_uint i = 0; i < m_iNumVertices; ++i)
		m_pVertexPositions[i] = pVertices[i].vPosition;

	VBInitialData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&VBBufferDesc, &VBInitialData, &m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion 

#pragma region INDEXBUFFER

	D3D11_BUFFER_DESC			IBBufferDesc{};
	IBBufferDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
	IBBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IBBufferDesc.CPUAccessFlags = /*D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE*/0;
	IBBufferDesc.StructureByteStride = m_iIndexStride;
	IBBufferDesc.MiscFlags = 0;

	_ushort* pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

	D3D11_SUBRESOURCE_DATA		IBInitialData{};
	IBInitialData.pSysMem = pIndices;

	if (FAILED(m_pDevice->CreateBuffer(&IBBufferDesc, &IBInitialData, &m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
#pragma endregion 

#pragma region INSTANCEBUFFER
	//m_VBInstanceDesc.ByteWidth = m_iNumInstance * m_iVertexInstanceStride;
	//m_VBInstanceDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//m_VBInstanceDesc.Usage = D3D11_USAGE_DYNAMIC;
	//m_VBInstanceDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//m_VBInstanceDesc.StructureByteStride = m_iVertexInstanceStride;
	//m_VBInstanceDesc.MiscFlags = 0;

	//m_pVertexInstances = new VTXPOS_PARTICLE_INSTANCE[m_iNumInstance];
	//m_pSpeeds = new _float[m_iNumInstance];

	//for (size_t i = 0; i < m_iNumInstance; i++)
	//{
	//	m_pSpeeds[i] = m_pGameInstance->Compute_Random(pDesc->vSpeed.x, pDesc->vSpeed.y);
	//	_float	fSize = m_pGameInstance->Compute_Random(pDesc->vSize.x, pDesc->vSize.y);

	//	m_pVertexInstances[i].vRight = _float4(fSize, 0.f, 0.f, 0.f);
	//	m_pVertexInstances[i].vUp = _float4(0.f, fSize, 0.f, 0.f);
	//	m_pVertexInstances[i].vLook = _float4(0.f, 0.f, fSize, 0.f);

	//	m_pVertexInstances[i].vTranslation = _float4(
	//		m_pGameInstance->Compute_Random(pDesc->vCenter.x - pDesc->vRange.x * 0.5f, pDesc->vCenter.x + pDesc->vRange.x * 0.5f),
	//		m_pGameInstance->Compute_Random(pDesc->vCenter.y - pDesc->vRange.y * 0.5f, pDesc->vCenter.y + pDesc->vRange.y * 0.5f),
	//		m_pGameInstance->Compute_Random(pDesc->vCenter.z - pDesc->vRange.z * 0.5f, pDesc->vCenter.z + pDesc->vRange.z * 0.5f),
	//		1.f
	//	);

	//	m_pVertexInstances[i].vLifeTime = _float2(
	//		m_pGameInstance->Compute_Random(pDesc->vLifeTime.x, pDesc->vLifeTime.y),
	//		0.f
	//	);
	//}

	//m_VBInstanceSubresourceData.pSysMem = m_pVertexInstances;

#pragma endregion 

	return S_OK;
}

HRESULT CVIBuffer_Point_Instance::Initialize(void* pArg)
{
#pragma region INSTANCEBUFFER
	m_VBInstanceDesc.ByteWidth = m_iNumInstance * m_iVertexInstanceStride;
	m_VBInstanceDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBInstanceDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_VBInstanceDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_VBInstanceDesc.StructureByteStride = m_iVertexInstanceStride;
	m_VBInstanceDesc.MiscFlags = 0;

	m_pVertexInstances = new VTXPOS_PARTICLE_INSTANCE[m_iNumInstance];
	m_pSpeeds = new _float[m_iNumInstance];

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		m_pSpeeds[i] = m_pGameInstance->Compute_Random(m_tPointInstanceDesc.vSpeed.x, m_tPointInstanceDesc.vSpeed.y);
		_float	fSize = m_pGameInstance->Compute_Random(m_tPointInstanceDesc.vSize.x, m_tPointInstanceDesc.vSize.y);

		m_pVertexInstances[i].vRight = _float4(fSize, 0.f, 0.f, 0.f);
		m_pVertexInstances[i].vUp = _float4(0.f, fSize, 0.f, 0.f);
		m_pVertexInstances[i].vLook = _float4(0.f, 0.f, fSize, 0.f);

		m_pVertexInstances[i].vTranslation = _float4(
			m_pGameInstance->Compute_Random(m_tPointInstanceDesc.vCenter.x - m_tPointInstanceDesc.vRange.x * 0.5f, m_tPointInstanceDesc.vCenter.x + m_tPointInstanceDesc.vRange.x * 0.5f),
			m_pGameInstance->Compute_Random(m_tPointInstanceDesc.vCenter.y - m_tPointInstanceDesc.vRange.y * 0.5f, m_tPointInstanceDesc.vCenter.y + m_tPointInstanceDesc.vRange.y * 0.5f),
			m_pGameInstance->Compute_Random(m_tPointInstanceDesc.vCenter.z - m_tPointInstanceDesc.vRange.z * 0.5f, m_tPointInstanceDesc.vCenter.z + m_tPointInstanceDesc.vRange.z * 0.5f),
			1.f
		);

		m_pVertexInstances[i].vLifeTime = _float2(
			m_pGameInstance->Compute_Random(m_tPointInstanceDesc.vLifeTime.x, m_tPointInstanceDesc.vLifeTime.y),
			0.f
		);
	}

	m_VBInstanceSubresourceData.pSysMem = m_pVertexInstances;

#pragma endregion 

	if (FAILED(m_pDevice->CreateBuffer(&m_VBInstanceDesc, &m_VBInstanceSubresourceData, &m_pVBInstance)))
		return E_FAIL;

	return S_OK;
}



void CVIBuffer_Point_Instance::Drop(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE	SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXPOS_PARTICLE_INSTANCE* pVertices = static_cast<VTXPOS_PARTICLE_INSTANCE*>(SubResource.pData);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pVertices[i].vLifeTime.y += fTimeDelta;

		pVertices[i].vTranslation.y -= m_pSpeeds[i] * fTimeDelta;

		if (true == m_tPointInstanceDesc.isLoop &&
			pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
		{
			pVertices[i].vLifeTime.y = 0.f;
			pVertices[i].vTranslation.y = m_pVertexInstances[i].vTranslation.y;
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Point_Instance::Spread(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE	SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXPOS_PARTICLE_INSTANCE* pVertices = static_cast<VTXPOS_PARTICLE_INSTANCE*>(SubResource.pData);

	_vector vDir = {};

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pVertices[i].vLifeTime.y += fTimeDelta;

		vDir = XMVectorSetW(XMLoadFloat4(&m_pVertexInstances[i].vTranslation) - XMVector3Normalize(XMLoadFloat3(&m_tPointInstanceDesc.vPivot)), 0.f);

		XMStoreFloat4(&pVertices[i].vTranslation,
			XMLoadFloat4(&pVertices[i].vTranslation) + (vDir * m_pSpeeds[i] * fTimeDelta));

		if (true == m_tPointInstanceDesc.isLoop &&
			pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
		{
			pVertices[i].vLifeTime.y = 0.f;
			pVertices[i].vTranslation = m_pVertexInstances[i].vTranslation;
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Point_Instance::Change_NumInstance(_uint iNumInstance)
{
	//부모의 인스턴스 갯수 바꾸고
	m_iNumInstance = iNumInstance;

	//리소스 제거 후, 새로운 인스턴스 버퍼 할당. (지금 이거 얕복 되네)
	Safe_Delete_Array(m_pVertexInstances);
	Safe_Delete_Array(m_pSpeeds);

	Safe_Release(m_pVBInstance);

#pragma region INSTANCEBUFFER
	m_VBInstanceDesc.ByteWidth = m_iNumInstance * m_iVertexInstanceStride;
	m_VBInstanceDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBInstanceDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_VBInstanceDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_VBInstanceDesc.StructureByteStride = m_iVertexInstanceStride;
	m_VBInstanceDesc.MiscFlags = 0;

	m_pVertexInstances = new VTXPOS_PARTICLE_INSTANCE[m_iNumInstance];
	m_pSpeeds = new _float[m_iNumInstance];

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		m_pSpeeds[i] = m_pGameInstance->Compute_Random(m_tPointInstanceDesc.vSpeed.x, m_tPointInstanceDesc.vSpeed.y);
		_float	fSize = m_pGameInstance->Compute_Random(m_tPointInstanceDesc.vSize.x, m_tPointInstanceDesc.vSize.y);

		m_pVertexInstances[i].vRight = _float4(fSize, 0.f, 0.f, 0.f);
		m_pVertexInstances[i].vUp = _float4(0.f, fSize, 0.f, 0.f);
		m_pVertexInstances[i].vLook = _float4(0.f, 0.f, fSize, 0.f);

		m_pVertexInstances[i].vTranslation = _float4(
			m_pGameInstance->Compute_Random(m_tPointInstanceDesc.vCenter.x - m_tPointInstanceDesc.vRange.x * 0.5f, m_tPointInstanceDesc.vCenter.x + m_tPointInstanceDesc.vRange.x * 0.5f),
			m_pGameInstance->Compute_Random(m_tPointInstanceDesc.vCenter.y - m_tPointInstanceDesc.vRange.y * 0.5f, m_tPointInstanceDesc.vCenter.y + m_tPointInstanceDesc.vRange.y * 0.5f),
			m_pGameInstance->Compute_Random(m_tPointInstanceDesc.vCenter.z - m_tPointInstanceDesc.vRange.z * 0.5f, m_tPointInstanceDesc.vCenter.z + m_tPointInstanceDesc.vRange.z * 0.5f),
			1.f
		);

		m_pVertexInstances[i].vLifeTime = _float2(
			m_pGameInstance->Compute_Random(m_tPointInstanceDesc.vLifeTime.x, m_tPointInstanceDesc.vLifeTime.y),
			0.f
		);
	}

	m_VBInstanceSubresourceData.pSysMem = m_pVertexInstances;

	if (FAILED(m_pDevice->CreateBuffer(&m_VBInstanceDesc, &m_VBInstanceSubresourceData, &m_pVBInstance)))
		return;
#pragma endregion 
}

void CVIBuffer_Point_Instance::Change_Range(_float3 vRange)
{
	m_tPointInstanceDesc.vRange = vRange;

	D3D11_MAPPED_SUBRESOURCE	SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXPOS_PARTICLE_INSTANCE* pVertices = static_cast<VTXPOS_PARTICLE_INSTANCE*>(SubResource.pData);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{

		pVertices[i].vTranslation = _float4(
			m_pGameInstance->Compute_Random(m_tPointInstanceDesc.vCenter.x - m_tPointInstanceDesc.vRange.x * 0.5f, m_tPointInstanceDesc.vCenter.x + m_tPointInstanceDesc.vRange.x * 0.5f),
			m_pGameInstance->Compute_Random(m_tPointInstanceDesc.vCenter.y - m_tPointInstanceDesc.vRange.y * 0.5f, m_tPointInstanceDesc.vCenter.y + m_tPointInstanceDesc.vRange.y * 0.5f),
			m_pGameInstance->Compute_Random(m_tPointInstanceDesc.vCenter.z - m_tPointInstanceDesc.vRange.z * 0.5f, m_tPointInstanceDesc.vCenter.z + m_tPointInstanceDesc.vRange.z * 0.5f),
			1.f
		);

		m_pVertexInstances[i].vTranslation = pVertices[i].vTranslation;
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Point_Instance::Change_Size(_float2 vSize)
{
	m_tPointInstanceDesc.vSize = vSize;

	D3D11_MAPPED_SUBRESOURCE	SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXPOS_PARTICLE_INSTANCE* pVertices = static_cast<VTXPOS_PARTICLE_INSTANCE*>(SubResource.pData);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		_float	fSize = m_pGameInstance->Compute_Random(m_tPointInstanceDesc.vSize.x, m_tPointInstanceDesc.vSize.y);

		pVertices[i].vRight = _float4(fSize, 0.f, 0.f, 0.f);
		pVertices[i].vUp = _float4(0.f, fSize, 0.f, 0.f);
		pVertices[i].vLook = _float4(0.f, 0.f, fSize, 0.f);

		m_pVertexInstances[i].vRight = pVertices[i].vRight;
		m_pVertexInstances[i].vUp = pVertices[i].vUp;
		m_pVertexInstances[i].vLook = pVertices[i].vLook;
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Point_Instance::Change_Size_XY(_float2 vSizeXY)
{
	D3D11_MAPPED_SUBRESOURCE	SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXPOS_PARTICLE_INSTANCE* pVertices = static_cast<VTXPOS_PARTICLE_INSTANCE*>(SubResource.pData);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pVertices[i].vRight = _float4(vSizeXY.x, 0.f, 0.f, 0.f);
		pVertices[i].vUp = _float4(0.f, vSizeXY.y, 0.f, 0.f);
		pVertices[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);

		m_pVertexInstances[i].vRight = pVertices[i].vRight;
		m_pVertexInstances[i].vUp = pVertices[i].vUp;
		m_pVertexInstances[i].vLook = pVertices[i].vLook;
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Point_Instance::Change_Pivot(_float3 vPivot)
{
	m_tPointInstanceDesc.vPivot = vPivot;
}

void CVIBuffer_Point_Instance::Change_LifeTime(_float2 vLifeTime)
{
	m_tPointInstanceDesc.vLifeTime = vLifeTime;

	D3D11_MAPPED_SUBRESOURCE	SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXPOS_PARTICLE_INSTANCE* pVertices = static_cast<VTXPOS_PARTICLE_INSTANCE*>(SubResource.pData);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pVertices[i].vLifeTime = _float2(
			m_pGameInstance->Compute_Random(m_tPointInstanceDesc.vLifeTime.x, m_tPointInstanceDesc.vLifeTime.y),
			0.f
		);

		m_pVertexInstances[i].vLifeTime = pVertices[i].vLifeTime;
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Point_Instance::Change_Speed(_float2 vSpeed)
{
	m_tPointInstanceDesc.vSpeed = vSpeed;

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		m_pSpeeds[i] = m_pGameInstance->Compute_Random(m_tPointInstanceDesc.vSpeed.x, m_tPointInstanceDesc.vSpeed.y);
	}

}

void CVIBuffer_Point_Instance::Change_isLoop(_bool bLoop)
{
	m_tPointInstanceDesc.isLoop = bLoop;
}

void CVIBuffer_Point_Instance::Change_Alpha(_float fAlpha)
{
	m_tPointInstanceDesc.fAlpha = fAlpha;
}

void CVIBuffer_Point_Instance::Change_Translation(const vector<_float3>& Translations)
{
	D3D11_MAPPED_SUBRESOURCE	SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXPOS_PARTICLE_INSTANCE* pVertices = static_cast<VTXPOS_PARTICLE_INSTANCE*>(SubResource.pData);

	size_t iCount = min(Translations.size(), m_iNumInstance);

	for (size_t i = 0; i < iCount; i++)
	{
		pVertices[i].vTranslation = _float4(Translations[i].x, Translations[i].y, Translations[i].z, 1.f);

		m_pVertexInstances[i].vTranslation = pVertices[i].vTranslation;
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Point_Instance::Change_Matrix(const vector<_float4x4>& Matrixs)
{
	D3D11_MAPPED_SUBRESOURCE	SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXPOS_PARTICLE_INSTANCE* pVertices = static_cast<VTXPOS_PARTICLE_INSTANCE*>(SubResource.pData);

	size_t iCount = min(Matrixs.size(), m_iNumInstance);

	for (size_t i = 0; i < iCount; i++)
	{
		pVertices[i].vRight = {Matrixs[i]._11, Matrixs[i]._12, Matrixs[i]._13, 1.f};
		pVertices[i].vUp = { Matrixs[i]._21, Matrixs[i]._22, Matrixs[i]._23, 1.f };
		pVertices[i].vLook = { Matrixs[i]._31, Matrixs[i]._32, Matrixs[i]._33, 1.f };
		pVertices[i].vTranslation = { Matrixs[i]._41, Matrixs[i]._42, Matrixs[i]._43, 1.f };

		m_pVertexInstances[i].vTranslation = pVertices[i].vTranslation;
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}



void CVIBuffer_Point_Instance::Change_Desc(const POINT_INSTANCE_DESC& Desc)
{
	Change_NumInstance(Desc.iNumInstance);
	Change_Range(Desc.vRange) ;
	Change_Size(Desc.vSize);
	Change_Pivot(Desc.vPivot);
	Change_LifeTime(Desc.vLifeTime);
	Change_Speed(Desc.vSpeed);
	Change_isLoop(Desc.isLoop);
	Change_Alpha(Desc.fAlpha);
	Set_EmissionShape(Desc.eEmissionShape);
}

void CVIBuffer_Point_Instance::Replay()
{
	D3D11_MAPPED_SUBRESOURCE	SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXPOS_PARTICLE_INSTANCE* pVertices = static_cast<VTXPOS_PARTICLE_INSTANCE*>(SubResource.pData);

	_vector vDir = {};

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pVertices[i].vLifeTime.y = 0.f;
		pVertices[i].vTranslation = m_pVertexInstances[i].vTranslation;
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

CVIBuffer_Point_Instance* CVIBuffer_Point_Instance::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const INSTANCE_DESC* pArg)
{
	CVIBuffer_Point_Instance* pInstance = new CVIBuffer_Point_Instance(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Point_Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Point_Instance::Clone(void* pArg)
{
	CVIBuffer_Point_Instance* pInstance = new CVIBuffer_Point_Instance(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Point_Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Point_Instance::Free()
{
	__super::Free();

	if (true == m_isCloned)
	{
		Safe_Delete_Array(m_pVertexInstances);
		Safe_Delete_Array(m_pSpeeds);
	}
}
