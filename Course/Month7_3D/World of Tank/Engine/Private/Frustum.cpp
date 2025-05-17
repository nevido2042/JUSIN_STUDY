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
        // 평면과 점 사이 거리
        _float fDistance = XMVectorGetX(XMPlaneDotCoord(m_FrustumPlane[i], vPos));

        // 만약 점이 평면 바깥쪽(=음수)이라면 프러스텀 밖에 있음
        if (fDistance > 0.f)
            return false;
    }

    return true;
}

_bool CFrustum::Is_In_Frustum(_fvector vPos, _float fRadius)
{
    for (int i = 0; i < 6; ++i)
    {
        // 평면 방정식: Ax + By + Cz + D
        // XMPlaneDotCoord는 평면과 점 사이의 signed 거리 값을 반환
        _float fDistance = XMVectorGetX(XMPlaneDotCoord(m_FrustumPlane[i], vPos));

        // 구의 반지름보다 더 멀리 떨어져 있으면 프러스텀 밖
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
