#include "Picking.h"

#include "GameInstance.h"

CPicking::CPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	, m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CPicking::Initialize(HWND hWnd/*, _uint iWinSizeX, _uint iWinSizeY*/)
{

	m_hWnd = hWnd;
	//m_iWinSizeX = iWinSizeX;
	//m_iWinSizeY = iWinSizeY;

	return S_OK;
}

void CPicking::Update(_uint iWinSizeX, _uint iWinSizeY)
{
	POINT		ptMouse = {};

	/*1. 뷰포트 상(윈도우공간)의 마우스 위치를 구하자. */
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);

	/*2. 투영스페이스로 옮기자. 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬 * 1/w */
	_float4			vPosition = { };

	vPosition.x = ptMouse.x / (iWinSizeX * 0.5f) - 1.f;
	vPosition.y = ptMouse.y / (iWinSizeY * -0.5f) + 1.f;
	vPosition.z = 0.0f;
	vPosition.w = 1.f;

	/*3. 뷰스페이스로 옮기자. 로컬위치 * 월드행렬 * 뷰행렬 */
	// 1. 프로젝션 역행렬 가져오기
	_matrix matProjInv = XMMatrixInverse(nullptr,
		XMLoadFloat4x4(m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ)));

	// vPosition은 스크린 공간의 NDC 좌표라고 가정합니다
	_vector vPos = XMLoadFloat4(&vPosition); // _float4 형태라고 가정

	// 2. NDC -> View Space
	vPos = XMVector4Transform(vPos, matProjInv);

	// 3. View -> World Space
	_matrix matView = XMLoadFloat4x4(m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW));
	_matrix matViewInv = XMMatrixInverse(nullptr, matView);

	_vector vMouseOrigin = XMVector3TransformCoord(XMVectorSet(0.f, 0.f, 0.f, 1.f), matViewInv);
	_vector vMouseDir = XMVector3TransformNormal(vPos, matViewInv);
	vMouseDir = XMVector3Normalize(vMouseDir);

	// 결과 저장
	XMStoreFloat3(&m_vMousePos, vMouseOrigin);
	XMStoreFloat3(&m_vMouseRay, vMouseDir);
}

_bool CPicking::Picking_InWorld(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC)
{
	_vector v0 = XMLoadFloat3(&vPointA);
	_vector v1 = XMLoadFloat3(&vPointB);
	_vector v2 = XMLoadFloat3(&vPointC);

	_vector rayOrigin = XMLoadFloat3(&m_vMousePos);
	_vector rayDir = XMVector3Normalize(XMLoadFloat3(&m_vMouseRay));  // 정규화 필수

	_float fDist = 0.f;

	_bool isPicked = TriangleTests::Intersects(
		rayOrigin,
		rayDir,
		v0,
		v1,
		v2,
		fDist
	);

	if (isPicked)
	{
		_vector vPicked = rayOrigin + rayDir * fDist;
		XMStoreFloat3(&vPickedPos, vPicked);
	}

	return isPicked;


}

_bool CPicking::Picking_InLocal(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC)
{
	_vector v0 = XMLoadFloat3(&vPointA);
	_vector v1 = XMLoadFloat3(&vPointB);
	_vector v2 = XMLoadFloat3(&vPointC);

	_vector rayOrigin = XMLoadFloat3(&m_vLocalMousePos);
	_vector rayDir = XMVector3Normalize(XMLoadFloat3(&m_vLocalMouseRay));  // 꼭 정규화!

	_float	fDist = 0.f;

	_bool isPicked = TriangleTests::Intersects(
		rayOrigin,
		rayDir,
		v0,
		v1,
		v2,
		fDist
	);

	if (isPicked)
	{
		_vector vPicked = rayOrigin + rayDir * fDist;
		XMStoreFloat3(&vPickedPos, vPicked);  // vPickedPos는 _float3
	}

	return isPicked;

}

void CPicking::Transform_ToLocalSpace(const _matrix& WorldMatrixInverse)
{
	/* 월드매트릭스는 각각의 객체가 들고 있고, */
	/* 우린 월드매트릭스의 역행렬을 구해주는 함수를 갖고 있다. */
	/* 월드매트릭스의 역행렬을 가져와서 */
	/* 월드상에 있는 마우스 위치, 방향을 각각 역행렬과 곱해준다면 */
	/* 각 객체의 로컬좌표로 마우스 위치와 방향이 이동될 것이다. */
	//D3DXVec3TransformCoord(&m_vLocalMousePos, &m_vMousePos, &WorldMatrixInverse);
	//D3DXVec3TransformNormal(&m_vLocalMouseRay, &m_vMouseRay, &WorldMatrixInverse);

	// 마우스 위치 (위치 좌표는 w=1.0)
	_vector vMousePosWS = XMLoadFloat3(&m_vMousePos);
	_vector vLocalMousePos = XMVector3TransformCoord(vMousePosWS, WorldMatrixInverse);
	XMStoreFloat3(&m_vLocalMousePos, vLocalMousePos);

	// 마우스 방향 (방향 벡터는 w=0.0)
	_vector vMouseRayWS = XMLoadFloat3(&m_vMouseRay);
	_vector vLocalMouseRay = XMVector3TransformNormal(vMouseRayWS, WorldMatrixInverse);
	XMStoreFloat3(&m_vLocalMouseRay, vLocalMouseRay);

}

CPicking* CPicking::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd/*, _uint iWinSizeX, _uint iWinSizeY*/)
{
	CPicking* pInstance = new CPicking(pDevice, pContext);

	if (FAILED(pInstance->Initialize(hWnd/*, iWinSizeX, iWinSizeY*/)))
	{
		MSG_BOX("Failed to Created : CPicking");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPicking::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
}
