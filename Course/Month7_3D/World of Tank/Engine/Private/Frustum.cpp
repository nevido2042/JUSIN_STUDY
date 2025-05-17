#include "Frustum.h"
#include "GameInstance.h"

CFrustum::CFrustum()
	: m_pGameInstance{CGameInstance::Get_Instance()}
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CFrustum::Initialize()
{
	return S_OK;
}

HRESULT CFrustum::Update()
{
    _matrix matView = m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW);
    _matrix matProj = m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ);

    _matrix matViewProj = XMMatrixMultiply(matView, matProj); // View * Projection

    // Right
    m_FrustumPlane[0] = XMPlaneNormalize(
        matViewProj.r[3] - matViewProj.r[0]);

    // Left
    m_FrustumPlane[1] = XMPlaneNormalize(
        matViewProj.r[3] + matViewProj.r[0]);

    // Top
    m_FrustumPlane[2] = XMPlaneNormalize(
        matViewProj.r[3] - matViewProj.r[1]);

    // Bottom
    m_FrustumPlane[3] = XMPlaneNormalize(
        matViewProj.r[3] + matViewProj.r[1]);

    // Far
    m_FrustumPlane[4] = XMPlaneNormalize(
        matViewProj.r[3] - matViewProj.r[2]);

    // Near
    m_FrustumPlane[5] = XMPlaneNormalize(
        matViewProj.r[3] + matViewProj.r[2]);

    return S_OK;
}


_bool CFrustum::Is_In_Frustum(_fvector vPos)
{

    for (int i = 0; i < 6; ++i)
    {
        // ���� �� ���� �Ÿ�
        _float fDistance = XMVectorGetX(XMPlaneDotCoord(m_FrustumPlane[i], vPos));

        // ���� ���� ��� �ٱ���(=����)�̶�� �������� �ۿ� ����
        if (fDistance > 0.f)
            return false;
    }

    return true;
}

_bool CFrustum::Is_In_Frustum(_fvector vPos, _float fRadius)
{
    for (int i = 0; i < 6; ++i)
    {
        // ��� ������: Ax + By + Cz + D
        // XMPlaneDotCoord�� ���� �� ������ signed �Ÿ� ���� ��ȯ
        _float fDistance = XMVectorGetX(XMPlaneDotCoord(m_FrustumPlane[i], vPos));

        // ���� ���������� �� �ָ� ������ ������ �������� ��
        if (fDistance > -fRadius)
            return false;
    }

    return true;
}

CFrustum* CFrustum::Create()
{
    CFrustum* pInstance = new CFrustum();

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : CFrustum");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CFrustum::Free()
{
	Safe_Release(m_pGameInstance);
}
