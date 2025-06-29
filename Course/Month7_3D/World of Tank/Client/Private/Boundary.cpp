#include "Boundary.h"

#include "GameInstance.h"

CBoundary::CBoundary(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CBoundary::CBoundary(const CBoundary& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CBoundary::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBoundary::Initialize(void* pArg)
{
	GAMEOBJECT_DESC			Desc{};

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	
	if (FAILED(Load_BoundaryPoints()))
		return E_FAIL;

#ifdef _DEBUG
	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
	m_pEffect = new BasicEffect(m_pDevice);

	const void* pShaderByteCode = { nullptr };
	size_t		iShaderByteCodeLength = {  };

	m_pEffect->SetVertexColorEnabled(true);

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount,
		pShaderByteCode, iShaderByteCodeLength, &m_pInputLayout)))
		return E_FAIL;

#endif // _DEBUG

	//vector<_float3> Temp{};
	//Temp.resize(500);
	//for (_float3& T : Temp)
	//{
	//	T = { 300.f, 90.f, 300.f };
	//}

	const size_t iPointCount = m_BoundaryPoints.size();
	if (iPointCount < 2)
		return E_FAIL; // �ּ� �� �� �̻��̾�� ���� ���� �� ����

	vector<_float4x4> Matrixs;
	Matrixs.reserve(iPointCount); // �󱸰��̹Ƿ� �ִ� iPointCount��

	for (_uint i = 0; i < iPointCount; ++i)
	{
		// ������ ���� ù ��° ���� �̾��� (i+1���� % ���)
		_vector vStart = XMVectorSetW(XMLoadFloat3(&m_BoundaryPoints[i]), 1.f);
		_vector vEnd = XMVectorSetW(XMLoadFloat3(&m_BoundaryPoints[(i + 1) % iPointCount]), 1.f);

		_vector vDir = vEnd - vStart;
		_float fLength = XMVectorGetX(XMVector3Length(vDir));

		// ���� ���� ����ȭ ��, ���̸�ŭ ������
		_vector vRight = XMVector3Normalize(-vDir) * fLength;

		// Up�� Y������ ���� (���� ���� �� y=10)
		_vector vUp = XMVectorSet(0.f, 10.f, 0.f, 0.f);

		// Look (��Ȯ�� ���� Ȯ����)
		_vector vLook = XMVector3Cross(vRight, vUp);
		vLook = XMVector3Normalize(vLook);

		// ���� ���
		_vector vMid = (vStart + vEnd) * 0.5f;

		_matrix matWorld = {
			XMVectorSet(vRight.m128_f32[0], vRight.m128_f32[1], vRight.m128_f32[2], 0.f),
			XMVectorSet(vUp.m128_f32[0], vUp.m128_f32[1], vUp.m128_f32[2], 0.f),
			XMVectorSet(vLook.m128_f32[0], vLook.m128_f32[1], vLook.m128_f32[2], 0.f),
			XMVectorSet(vMid.m128_f32[0], vMid.m128_f32[1], vMid.m128_f32[2], 1.f)
		};

		_float4x4 mat{};
		XMStoreFloat4x4(&mat, matWorld);
		Matrixs.push_back(mat);
	}

	m_pVIBufferCom->Change_Matrix(Matrixs);


	return S_OK;
}


void CBoundary::Priority_Update(_float fTimeDelta)
{

}

void CBoundary::Update(_float fTimeDelta)
{
	m_pVIBufferCom->Emission(fTimeDelta);

}

void CBoundary::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CBoundary::Render()
{
#ifdef _DEBUG
#pragma region ���� ��
	m_pEffect->SetWorld(XMMatrixIdentity());
	m_pEffect->SetView(m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW));
	m_pEffect->SetProjection(m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ));

	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pEffect->Apply(m_pContext);

	m_pBatch->Begin();

	Draw_Boundary();

	m_pBatch->End();
#pragma endregion
#endif // _DEBUG

#pragma region  ���� �庮
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
	//	return E_FAIL;

	//if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpah", &m_pVIBufferCom->Get_Desc().fAlpha, sizeof(_float))))
	//	return E_FAIL;

	//if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
	//	return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(2)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

//_bool CBoundary::IsPointInBoundary(const _float3& vPoint, _float3& vOutClosestPos)
//{
//	vector<_float3> vCrossPoints;
//
//	_int iCount = 0; // ���� Ƚ���� ���� ī����
//	size_t iNumPoints = m_BoundaryPoints.size(); // ������ �������� ����
//
//	for (size_t i = 0; i < iNumPoints; ++i)
//	{
//		// ���� �������� ���� �������� ������ (�������� ù ��°�� ����)
//		_float3 vPointA = m_BoundaryPoints[i];
//		_float3 vPointB = m_BoundaryPoints[(i + 1) % iNumPoints];
//
//		// XZ ��鿡�� �־��� ��(vPoint)�� z��ǥ�� �� ������ ���̿� �ִ��� Ȯ��
//		if ((vPointA.z > vPoint.z) != (vPointB.z > vPoint.z))
//		{
//			// �ش� z ��ġ���� ���� AB�� �����ϴ� X ��ǥ ���
//			_float fAt_X = (vPointB.x - vPointA.x) * (vPoint.z - vPointA.z) / (vPointB.z - vPointA.z) + vPointA.x;
//
//			// ���� x ��ǥ�� �������� x���� ������ ������ ������ �Ǵ�
//			if (vPoint.x < fAt_X)
//			{
//				++iCount; // ���� Ƚ�� ����
//
//				_float t = (vPoint.z - vPointA.z) / (vPointB.z - vPointA.z);
//				_float3 vIntersect;
//				vIntersect.x = vPointA.x + (vPointB.x - vPointA.x) * t;
//				vIntersect.z = vPoint.z;
//				vIntersect.y = vPointA.y + (vPointB.y - vPointA.y) * t;
//
//				vCrossPoints.push_back(vIntersect);
//			}
//
//		}
//	}
//
//	_float fMinDist = FLT_MAX;
//
//	//vPoint�� ���� ����� vCrossPoints �� �����Ѵ�.
//	for (_float3 vCrossPoint : vCrossPoints)
//	{
//		_float fDist = XMVectorGetX(XMVector3Length(XMLoadFloat3(&vCrossPoint) - XMLoadFloat3(&vPoint)));
//
//		if (fMinDist > fDist)
//		{
//			fMinDist = fDist;
//			vOutClosestPos = vCrossPoint;
//		}
//	}
//
//	// ���� Ƚ���� Ȧ���̸� ����, ¦���̸� �ܺο� ��ġ�� ������ �Ǵ�
//	return (iCount % 2 == 1);
//}

_float3 CBoundary::ClosestPointOnBoundary(const _float3& vPos)
{
	_float3 vClosest = vPos;
	_float fMinDistSq = FLT_MAX;

	for (size_t i = 0; i < m_BoundaryPoints.size(); ++i)
	{
		const _float3& vStart = m_BoundaryPoints[i];
		const _float3& vEnd = m_BoundaryPoints[(i + 1) % m_BoundaryPoints.size()];

		// XZ ��� �������� ���
		_vector vP = XMLoadFloat3(&vPos);
		_vector vA = XMLoadFloat3(&vStart);
		_vector vB = XMLoadFloat3(&vEnd);

		// ���� AB�� �� P ������ ���� ���
		_vector vAB = vB - vA;
		_vector vAP = vP - vA;

		// AB�� ���п� ������ ���� 
		_float fT = XMVectorGetX(XMVector3Dot(vAP, vAB)) / XMVectorGetX(XMVector3Dot(vAB, vAB));
		fT = clamp(fT, 0.f, 1.f);

		_vector vProj = vA + vAB * fT;

		// �Ÿ� ���� ���
		_vector vDiff = vP - vProj;
		_float fDistSq = XMVectorGetX(XMVector3LengthSq(vDiff));

		if (fDistSq < fMinDistSq)
		{
			fMinDistSq = fDistSq;
			XMStoreFloat3(&vClosest, vProj);
		}
	}

	// Y�� ����
	vClosest.y = vPos.y;
	return vClosest;
}

_bool CBoundary::IsPointInBoundary(const _float3& vPoint)
{
	_int iCount = 0; // ���� Ƚ���� ���� ī����
	size_t iNumPoints = m_BoundaryPoints.size(); // ������ �������� ����

	for (size_t i = 0; i < iNumPoints; ++i)
	{
		// ���� �������� ���� �������� ������ (�������� ù ��°�� ����)
		_float3 vPointA = m_BoundaryPoints[i];
		_float3 vPointB = m_BoundaryPoints[(i + 1) % iNumPoints];

		// XZ ��鿡�� �־��� ��(vPoint)�� z��ǥ�� �� ������ ���̿� �ִ��� Ȯ��
		if ((vPointA.z > vPoint.z) != (vPointB.z > vPoint.z))
		{
			// �ش� z ��ġ���� ���� AB�� �����ϴ� X ��ǥ ���
			_float fAt_X = (vPointB.x - vPointA.x) * (vPoint.z - vPointA.z) / (vPointB.z - vPointA.z) + vPointA.x;

			// ���� x ��ǥ�� �������� x���� ������ ������ ������ �Ǵ�
			if (vPoint.x < fAt_X)
				++iCount; // ���� Ƚ�� ����
		}
	}

	// ���� Ƚ���� Ȧ���̸� ����, ¦���̸� �ܺο� ��ġ�� ������ �Ǵ�
	return (iCount % 2 == 1);

}

_float3 CBoundary::SafeInsideBoundary(const _float3& vBoundaryPoint, const _float3& vCenter, _float fOffset)
{
	_vector vDir = XMLoadFloat3(&vCenter) - XMLoadFloat3(&vBoundaryPoint);
	
	vDir = XMVectorSetY(vDir, 0.f);
	vDir = XMVector3Normalize(vDir);
	_float3 vSafePos;
	XMStoreFloat3(&vSafePos, XMLoadFloat3(&vBoundaryPoint) + vDir * fOffset);
	vSafePos.y = vBoundaryPoint.y;
	return vSafePos;
}

HRESULT CBoundary::Load_BoundaryPoints()
{
	m_BoundaryPoints.clear();

	filesystem::path Path = "../Bin/Map/BoundaryPoints.bin";

	FILE* fp = nullptr;
	fopen_s(&fp, Path.string().c_str(), "rb");
	if (!fp)
		return S_OK;

	while (true)
	{

		_float3 vPos = {};
		size_t readCount = fread(&vPos, sizeof(_float3), 1, fp);

		// �� �̻� ������ ������ ����
		if (readCount != 1)
			break;

		m_BoundaryPoints.push_back(vPos);

	}

	fclose(fp);

	return S_OK;
}

#ifdef _DEBUG
void CBoundary::Draw_Boundary()
{
	if (m_BoundaryPoints.size() < 2)
		return;

	_float3 vPointA = {};
	_float3 vPointB = {};

	for (_uint i = 0; i < m_BoundaryPoints.size(); ++i)
	{
		if (i == m_BoundaryPoints.size() - 1)
		{
			vPointA = m_BoundaryPoints.at(i);
			vPointB = m_BoundaryPoints.at(0);
		}
		else
		{
			vPointA = m_BoundaryPoints.at(i);
			vPointB = m_BoundaryPoints.at(i + 1);
		}

		DX::DrawRay(
			m_pBatch,
			XMVectorSetW(XMLoadFloat3(&vPointA), 1.f),     // ������ A
			XMVectorSubtract(XMLoadFloat3(&vPointB), XMLoadFloat3(&vPointA)), // ���� ���� (B - A)
			false,
			DirectX::Colors::Red
		);

	}
}
#endif // _DEBUG

HRESULT CBoundary::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosInstance"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Boundary"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Smoke"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CBoundary* CBoundary::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBoundary* pInstance = new CBoundary(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBoundary");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoundary::Clone(void* pArg)
{
	CBoundary* pInstance = new CBoundary(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBoundary");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoundary::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);

#ifdef _DEBUG
	Safe_Release(m_pInputLayout);
	Safe_Delete(m_pBatch);
	Safe_Delete(m_pEffect);
#endif // _DEBUG
}
