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

    // View * Projection ���
    _matrix matView = m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW);
    _matrix matProj = m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ);
    _matrix matVP = matView * matProj;

    // transposition: D3DXPlaneFromMatrix expects column-major
    matVP = XMMatrixTranspose(matVP);

    // �������� ��� ���� (��, ��, ��, ��, ��, ��)
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

    // ��� ����ȭ
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
        // ���� �� ���� �Ÿ�
        _float fDistance = XMVectorGetX(XMPlaneDotCoord(m_FrustumPlane[i], vPos));

        // ���� ���� ��� �ٱ���(=����)�̶�� �������� �ۿ� ����
        if (fDistance < 0.f)
            return false;
    }

    return true;
}

_bool CFrustum::Is_In_Frustum(_fvector vPos, _float fRadius)
{
    for (_uint i = 0; i < 6; ++i)
    {
        // ��� ������: Ax + By + Cz + D
        // XMPlaneDotCoord�� ���� �� ������ signed �Ÿ� ���� ��ȯ
        _float fDistance = XMVectorGetX(XMPlaneDotCoord(m_FrustumPlane[i], vPos));

        // ���� ���������� �� �ָ� ������ ������ �������� ��
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
