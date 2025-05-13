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

	/*1. ����Ʈ ��(���������)�� ���콺 ��ġ�� ������. */
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);

	/*2. ���������̽��� �ű���. ������ġ * ������� * ����� * ������� * 1/w */
	_float4			vPosition = { };

	vPosition.x = ptMouse.x / (iWinSizeX * 0.5f) - 1.f;
	vPosition.y = ptMouse.y / (iWinSizeY * -0.5f) + 1.f;
	vPosition.z = 0.0f;
	vPosition.w = 1.f;

	/*3. �佺���̽��� �ű���. ������ġ * ������� * ����� */
	// 1. �������� ����� ��������
	_matrix matProjInv = XMMatrixInverse(nullptr,
		XMLoadFloat4x4(m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ)));

	// vPosition�� ��ũ�� ������ NDC ��ǥ��� �����մϴ�
	_vector vPos = XMLoadFloat4(&vPosition); // _float4 ���¶�� ����

	// 2. NDC -> View Space
	vPos = XMVector4Transform(vPos, matProjInv);

	// 3. View -> World Space
	_matrix matView = XMLoadFloat4x4(m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW));
	_matrix matViewInv = XMMatrixInverse(nullptr, matView);

	_vector vMouseOrigin = XMVector3TransformCoord(XMVectorSet(0.f, 0.f, 0.f, 1.f), matViewInv);
	_vector vMouseDir = XMVector3TransformNormal(vPos, matViewInv);
	vMouseDir = XMVector3Normalize(vMouseDir);

	// ��� ����
	XMStoreFloat3(&m_vMousePos, vMouseOrigin);
	XMStoreFloat3(&m_vMouseRay, vMouseDir);
}

_bool CPicking::Picking_InWorld(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC)
{
	_vector v0 = XMLoadFloat3(&vPointA);
	_vector v1 = XMLoadFloat3(&vPointB);
	_vector v2 = XMLoadFloat3(&vPointC);

	_vector rayOrigin = XMLoadFloat3(&m_vMousePos);
	_vector rayDir = XMVector3Normalize(XMLoadFloat3(&m_vMouseRay));  // ����ȭ �ʼ�

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
	_vector rayDir = XMVector3Normalize(XMLoadFloat3(&m_vLocalMouseRay));  // �� ����ȭ!

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
		XMStoreFloat3(&vPickedPos, vPicked);  // vPickedPos�� _float3
	}

	return isPicked;

}

void CPicking::Transform_ToLocalSpace(const _matrix& WorldMatrixInverse)
{
	/* �����Ʈ������ ������ ��ü�� ��� �ְ�, */
	/* �츰 �����Ʈ������ ������� �����ִ� �Լ��� ���� �ִ�. */
	/* �����Ʈ������ ������� �����ͼ� */
	/* ����� �ִ� ���콺 ��ġ, ������ ���� ����İ� �����شٸ� */
	/* �� ��ü�� ������ǥ�� ���콺 ��ġ�� ������ �̵��� ���̴�. */
	//D3DXVec3TransformCoord(&m_vLocalMousePos, &m_vMousePos, &WorldMatrixInverse);
	//D3DXVec3TransformNormal(&m_vLocalMouseRay, &m_vMouseRay, &WorldMatrixInverse);

	// ���콺 ��ġ (��ġ ��ǥ�� w=1.0)
	_vector vMousePosWS = XMLoadFloat3(&m_vMousePos);
	_vector vLocalMousePos = XMVector3TransformCoord(vMousePosWS, WorldMatrixInverse);
	XMStoreFloat3(&m_vLocalMousePos, vLocalMousePos);

	// ���콺 ���� (���� ���ʹ� w=0.0)
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
