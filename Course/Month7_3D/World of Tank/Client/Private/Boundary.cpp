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

	if (FAILED(Load_BoundaryPoints()))
		return E_FAIL;

	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
	m_pEffect = new BasicEffect(m_pDevice);

	const void* pShaderByteCode = { nullptr };
	size_t		iShaderByteCodeLength = {  };

	m_pEffect->SetVertexColorEnabled(true);

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount,
		pShaderByteCode, iShaderByteCodeLength, &m_pInputLayout)))
		return E_FAIL;

	return S_OK;
}


void CBoundary::Priority_Update(_float fTimeDelta)
{

}

void CBoundary::Update(_float fTimeDelta)
{


}

void CBoundary::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CBoundary::Render()
{
	m_pEffect->SetWorld(XMMatrixIdentity());
	m_pEffect->SetView(m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW));
	m_pEffect->SetProjection(m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ));

	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pEffect->Apply(m_pContext);

	m_pBatch->Begin();

	Draw_Boundary();

	m_pBatch->End();

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

	Safe_Release(m_pInputLayout);
	Safe_Delete(m_pBatch);
	Safe_Delete(m_pEffect);
}
