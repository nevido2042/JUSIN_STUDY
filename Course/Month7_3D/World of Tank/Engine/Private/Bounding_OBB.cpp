#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"

CBounding_OBB::CBounding_OBB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBounding{ pDevice, pContext }
{
}

HRESULT CBounding_OBB::Initialize(const CBounding::BOUNDING_DESC* pDesc)
{
	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;
	const OBB_DESC* pOBBDesc = static_cast<const OBB_DESC*>(pDesc);

	_float4		vQuaternion = {};

	XMStoreFloat4(&vQuaternion, XMQuaternionRotationRollPitchYaw(pOBBDesc->vRotation.x, pOBBDesc->vRotation.y, pOBBDesc->vRotation.z));


	m_pOriginalDesc = new BoundingOrientedBox(pOBBDesc->vCenter, pOBBDesc->vExtents, vQuaternion);
	m_pDesc = new BoundingOrientedBox(*m_pOriginalDesc);

	return S_OK;
}

void CBounding_OBB::Update(_fmatrix WorldMatrix)
{

	m_pOriginalDesc->Transform(*m_pDesc, WorldMatrix);
}

_bool CBounding_OBB::Intersect(CBounding* pTarget, _vector* pOutNormal)
{
	_bool		isColl = { false };

	switch (pTarget->Get_Type())
	{
	case COLLIDER::AABB:
		isColl = m_pDesc->Intersects(*static_cast<CBounding_AABB*>(pTarget)->Get_Desc());
		break;
	case COLLIDER::OBB:
		//isColl = m_pDesc->Intersects(*static_cast<CBounding_OBB*>(pTarget)->Get_Desc());
		isColl = Intersect_ToOBB(pTarget, pOutNormal);
		/*�浹 ���� ���� ��ȯ*/

		//����ġ, Ÿ�� ��ġ�� ��� ��鿡 �΋H������ �Ǵ��ϰ�
		//�׹��� ���͸� ��ȯ �ؾ���


		break;
	case COLLIDER::SPHERE:
		isColl = m_pDesc->Intersects(*static_cast<CBounding_Sphere*>(pTarget)->Get_Desc());
		break;

	}

	return isColl;
}

_bool CBounding_OBB::Intersect_Ray(_fvector vOrigin, _fvector vDir, _float& fDist)
{
	_bool		isColl = { false };

	isColl = m_pDesc->Intersects(vOrigin, vDir, fDist);

	return isColl;
}

#ifdef _DEBUG

HRESULT CBounding_OBB::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor)
{

	DX::Draw(pBatch, *m_pDesc, vColor);

	return S_OK;
}

#endif


_bool CBounding_OBB::Intersect_Ray_ToOBB(_fvector vOrigin, _fvector vDir, _float& fDist, _vector* pOutNormal)
{
	OBB_INFO OBBDesc = Compute_OBB();

	// ���� ���� ��ǥ��� ��ȯ
	_vector vDelta = XMLoadFloat3(&OBBDesc.vCenter) - vOrigin;

	_vector vAxis[3] =
	{
		XMLoadFloat3(&OBBDesc.vAxisDir[0]),
		XMLoadFloat3(&OBBDesc.vAxisDir[1]),
		XMLoadFloat3(&OBBDesc.vAxisDir[2])
	};

	_float fExtent[3] =
	{
		XMVectorGetX(XMVector3Length(XMLoadFloat3(&OBBDesc.vCenterDir[0]))),
		XMVectorGetX(XMVector3Length(XMLoadFloat3(&OBBDesc.vCenterDir[1]))),
		XMVectorGetX(XMVector3Length(XMLoadFloat3(&OBBDesc.vCenterDir[2])))
	};

	_float tMin = -FLT_MAX;
	_float tMax = FLT_MAX;
	_int iHitAxis = -1;

	for (int i = 0; i < 3; ++i)
	{
		_float e = XMVectorGetX(XMVector3Dot(vAxis[i], vDelta));
		_float f = XMVectorGetX(XMVector3Dot(vAxis[i], vDir));

		if (fabsf(f) > 1e-6f) // ���̿� ���� �������� ����
		{
			_float t1 = (e + fExtent[i]) / f;
			_float t2 = (e - fExtent[i]) / f;

			if (t1 > t2) std::swap(t1, t2);

			if (t1 > tMin)
			{
				tMin = t1;
				iHitAxis = i;
			}

			tMax = min(tMax, t2);

			if (tMin > tMax) return false;
			if (tMax < 0.f) return false;
		}
		else
		{
			// ���̰� ��� ���� ������ ��, OBB�� �ݴ��� ���� ���
			if (fabsf(e) > fExtent[i])
				return false;
		}
	}

	fDist = (tMin >= 0.f) ? tMin : tMax;

	if (pOutNormal != nullptr && iHitAxis >= 0)
	{
		_vector vNormal = vAxis[iHitAxis];

		// ���� ������ ������ �ݴ������ �������� ����
		if (XMVectorGetX(XMVector3Dot(vNormal, vDir)) > 0.f)
			vNormal = -vNormal;

		*pOutNormal = XMVector3Normalize(vNormal);
	}

	return true;
}

_bool CBounding_OBB::Intersect_ToOBB(CBounding* pTarget, _vector* pOutNormal)
{
	OBB_INFO	OBBDesc[2] = {
		Compute_OBB(),
		static_cast<CBounding_OBB*>(pTarget)->Compute_OBB()
	};

	_vector		vCenterDelta = XMLoadFloat3(&OBBDesc[1].vCenter) - XMLoadFloat3(&OBBDesc[0].vCenter);

	_vector		vAxisList[15];
	_int		iAxisIndex = 0;

	// 0~2: A�� ���� ��
	for (size_t i = 0; i < 3; ++i)
		vAxisList[iAxisIndex++] = XMLoadFloat3(&OBBDesc[0].vAxisDir[i]);

	// 3~5: B�� ���� ��
	for (size_t i = 0; i < 3; ++i)
		vAxisList[iAxisIndex++] = XMLoadFloat3(&OBBDesc[1].vAxisDir[i]);

	// 6~14: �� ���� ������
	for (size_t i = 0; i < 3; ++i)
	{
		_vector vA = XMLoadFloat3(&OBBDesc[0].vAxisDir[i]);

		for (size_t j = 0; j < 3; ++j)
		{
			_vector vB = XMLoadFloat3(&OBBDesc[1].vAxisDir[j]);
			_vector vCross = XMVector3Cross(vA, vB);

			if (XMVector3Equal(vCross, XMVectorZero())) continue;

			vAxisList[iAxisIndex++] = XMVector3Normalize(vCross);
		}
	}

	_float		fMinPenetration = FLT_MAX;
	_vector		vBestAxis = XMVectorZero();

	for (size_t i = 0; i < iAxisIndex; ++i)
	{
		_vector vAxis = XMVector3Normalize(vAxisList[i]);
		if (XMVector3Equal(vAxis, XMVectorZero())) continue;

		_float fProjCenter = fabs(XMVectorGetX(XMVector3Dot(vCenterDelta, vAxis)));

		_float fProjA =
			fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterDir[0]), vAxis))) +
			fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterDir[1]), vAxis))) +
			fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterDir[2]), vAxis)));

		_float fProjB =
			fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterDir[0]), vAxis))) +
			fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterDir[1]), vAxis))) +
			fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterDir[2]), vAxis)));


		if (fProjCenter > fProjA + fProjB)
			return false;

		if (pOutNormal != nullptr)
		{
			_float fPenetration = (fProjA + fProjB) - fProjCenter;
			if (fPenetration < fMinPenetration)
			{
				fMinPenetration = fPenetration;
				vBestAxis = vAxis;
			}
		}
	}

	if (pOutNormal != nullptr)
	{
		// ���� ����: vCenterDelta �������� �븻 ���� ����
		if (XMVectorGetX(XMVector3Dot(vBestAxis, vCenterDelta)) < 0.f)
			vBestAxis = -vBestAxis;

		*pOutNormal = vBestAxis;
	}

	return true;
}




CBounding_OBB::OBB_INFO CBounding_OBB::Compute_OBB()
{
	OBB_INFO		OBBDesc{};

	_float3			vPoints[8];

	m_pDesc->GetCorners(vPoints);

	OBBDesc.vCenter = m_pDesc->Center;

	XMStoreFloat3(&OBBDesc.vCenterDir[0], (XMLoadFloat3(&vPoints[5]) - XMLoadFloat3(&vPoints[4])) * 0.5f);
	XMStoreFloat3(&OBBDesc.vCenterDir[1], (XMLoadFloat3(&vPoints[7]) - XMLoadFloat3(&vPoints[4])) * 0.5f);
	XMStoreFloat3(&OBBDesc.vCenterDir[2], (XMLoadFloat3(&vPoints[0]) - XMLoadFloat3(&vPoints[4])) * 0.5f);

	for (size_t i = 0; i < 3; i++)
		XMStoreFloat3(&OBBDesc.vAxisDir[i], XMVector3Normalize(XMLoadFloat3(&OBBDesc.vCenterDir[i])));

	return OBBDesc;
}

CBounding_OBB* CBounding_OBB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CBounding::BOUNDING_DESC* pDesc)
{
	CBounding_OBB* pInstance = new CBounding_OBB(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pDesc)))
	{
		MSG_BOX("Failed to Created : CBounding_OBB");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CBounding_OBB::Free()
{
	__super::Free();

	Safe_Delete(m_pOriginalDesc);
	Safe_Delete(m_pDesc);
}
