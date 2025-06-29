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
		return E_FAIL; // 최소 두 개 이상이어야 선을 그을 수 있음

	vector<_float4x4> Matrixs;
	Matrixs.reserve(iPointCount); // 폐구간이므로 최대 iPointCount개

	for (_uint i = 0; i < iPointCount; ++i)
	{
		// 마지막 점은 첫 번째 점과 이어짐 (i+1에서 % 사용)
		_vector vStart = XMVectorSetW(XMLoadFloat3(&m_BoundaryPoints[i]), 1.f);
		_vector vEnd = XMVectorSetW(XMLoadFloat3(&m_BoundaryPoints[(i + 1) % iPointCount]), 1.f);

		_vector vDir = vEnd - vStart;
		_float fLength = XMVectorGetX(XMVector3Length(vDir));

		// 방향 벡터 정규화 후, 길이만큼 스케일
		_vector vRight = XMVector3Normalize(-vDir) * fLength;

		// Up은 Y축으로 고정 (높이 강조 시 y=10)
		_vector vUp = XMVectorSet(0.f, 10.f, 0.f, 0.f);

		// Look (정확한 직교 확보용)
		_vector vLook = XMVector3Cross(vRight, vUp);
		vLook = XMVector3Normalize(vLook);

		// 중점 계산
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
#pragma region 빨간 선
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

#pragma region  빨간 장벽
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
//	_int iCount = 0; // 교차 횟수를 세는 카운터
//	size_t iNumPoints = m_BoundaryPoints.size(); // 폴리곤 꼭짓점의 개수
//
//	for (size_t i = 0; i < iNumPoints; ++i)
//	{
//		// 현재 꼭짓점과 다음 꼭짓점을 가져옴 (마지막은 첫 번째와 연결)
//		_float3 vPointA = m_BoundaryPoints[i];
//		_float3 vPointB = m_BoundaryPoints[(i + 1) % iNumPoints];
//
//		// XZ 평면에서 주어진 점(vPoint)의 z좌표가 두 꼭짓점 사이에 있는지 확인
//		if ((vPointA.z > vPoint.z) != (vPointB.z > vPoint.z))
//		{
//			// 해당 z 위치에서 선분 AB와 교차하는 X 좌표 계산
//			_float fAt_X = (vPointB.x - vPointA.x) * (vPoint.z - vPointA.z) / (vPointB.z - vPointA.z) + vPointA.x;
//
//			// 점의 x 좌표가 교차점의 x보다 작으면 교차한 것으로 판단
//			if (vPoint.x < fAt_X)
//			{
//				++iCount; // 교차 횟수 증가
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
//	//vPoint와 가장 가까운 vCrossPoints 를 저장한다.
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
//	// 교차 횟수가 홀수이면 내부, 짝수이면 외부에 위치한 것으로 판단
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

		// XZ 평면 기준으로 계산
		_vector vP = XMLoadFloat3(&vPos);
		_vector vA = XMLoadFloat3(&vStart);
		_vector vB = XMLoadFloat3(&vEnd);

		// 선분 AB와 점 P 사이의 투영 계산
		_vector vAB = vB - vA;
		_vector vAP = vP - vA;

		// AB의 선분에 수직인 선을 
		_float fT = XMVectorGetX(XMVector3Dot(vAP, vAB)) / XMVectorGetX(XMVector3Dot(vAB, vAB));
		fT = clamp(fT, 0.f, 1.f);

		_vector vProj = vA + vAB * fT;

		// 거리 제곱 계산
		_vector vDiff = vP - vProj;
		_float fDistSq = XMVectorGetX(XMVector3LengthSq(vDiff));

		if (fDistSq < fMinDistSq)
		{
			fMinDistSq = fDistSq;
			XMStoreFloat3(&vClosest, vProj);
		}
	}

	// Y값 유지
	vClosest.y = vPos.y;
	return vClosest;
}

_bool CBoundary::IsPointInBoundary(const _float3& vPoint)
{
	_int iCount = 0; // 교차 횟수를 세는 카운터
	size_t iNumPoints = m_BoundaryPoints.size(); // 폴리곤 꼭짓점의 개수

	for (size_t i = 0; i < iNumPoints; ++i)
	{
		// 현재 꼭짓점과 다음 꼭짓점을 가져옴 (마지막은 첫 번째와 연결)
		_float3 vPointA = m_BoundaryPoints[i];
		_float3 vPointB = m_BoundaryPoints[(i + 1) % iNumPoints];

		// XZ 평면에서 주어진 점(vPoint)의 z좌표가 두 꼭짓점 사이에 있는지 확인
		if ((vPointA.z > vPoint.z) != (vPointB.z > vPoint.z))
		{
			// 해당 z 위치에서 선분 AB와 교차하는 X 좌표 계산
			_float fAt_X = (vPointB.x - vPointA.x) * (vPoint.z - vPointA.z) / (vPointB.z - vPointA.z) + vPointA.x;

			// 점의 x 좌표가 교차점의 x보다 작으면 교차한 것으로 판단
			if (vPoint.x < fAt_X)
				++iCount; // 교차 횟수 증가
		}
	}

	// 교차 횟수가 홀수이면 내부, 짝수이면 외부에 위치한 것으로 판단
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

		// 더 이상 읽을게 없으면 종료
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
			XMVectorSetW(XMLoadFloat3(&vPointA), 1.f),     // 시작점 A
			XMVectorSubtract(XMLoadFloat3(&vPointB), XMLoadFloat3(&vPointA)), // 방향 벡터 (B - A)
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
