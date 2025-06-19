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

    // View * Projection 행렬
    _matrix matView = m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW);
    _matrix matProj = m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ);
    _matrix matVP = matView * matProj;

    // transposition: D3DXPlaneFromMatrix expects column-major
    matVP = XMMatrixTranspose(matVP);

    // 프러스텀 평면 추출 (좌, 우, 하, 상, 근, 원)
    // Left Plane: row 4 + row 1
    m_FrustumPlane[0] = matVP.r[3] + matVP.r[0];
    // Right Plane: row 4 - row 1
    m_FrustumPlane[1] = matVP.r[3] - matVP.r[0];
    // Bottom Plane: row 4 + row 2
    m_FrustumPlane[2] = matVP.r[3] + matVP.r[1];
    // Top Plane: row 4 - row 2
    m_FrustumPlane[3] = matVP.r[3] - matVP.r[1];
    // Near Plane: row 4 + row 3
    m_FrustumPlane[4] = matVP.r[3] + matVP.r[2];
    // Far Plane: row 4 - row 3
    m_FrustumPlane[5] = matVP.r[3] - matVP.r[2];

    // 평면 정규화
    for (_uint i = 0; i < 6; ++i)
    {
        _vector& plane = m_FrustumPlane[i];
        _float length = XMVectorGetX(XMVector3Length(plane));
        plane = XMVectorDivide(plane, XMVectorReplicate(length));
    }

    return S_OK;
}


_bool CFrustum::Is_In_Frustum(_fvector vPos)
{

    for (_uint i = 0; i < 6; ++i)
    {
        // 평면과 점 사이 거리
        _float fDistance = XMVectorGetX(XMPlaneDotCoord(m_FrustumPlane[i], vPos));

        // 만약 점이 평면 바깥쪽(=음수)이라면 프러스텀 밖에 있음
        if (fDistance < 0.f)
            return false;
    }

    return true;
}

_bool CFrustum::Is_In_Frustum(_fvector vPos, _float fRadius)
{
    for (_uint i = 0; i < 6; ++i)
    {
        // 평면 방정식: Ax + By + Cz + D
        // XMPlaneDotCoord는 평면과 점 사이의 signed 거리 값을 반환
        _float fDistance = XMVectorGetX(XMPlaneDotCoord(m_FrustumPlane[i], vPos));

        // 구의 반지름보다 더 멀리 떨어져 있으면 프러스텀 밖
        if (fDistance < -fRadius)
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
    __super::Free();

	Safe_Release(m_pGameInstance);
}
