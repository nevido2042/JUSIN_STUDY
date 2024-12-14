#include "pch.h"
#include "RangedWeapon.h"
#include "TimeMgr.h"

CRangedWeapon::CRangedWeapon()
    :m_iFireRate(0), m_pProjectile(nullptr),
    m_iLastFireTime(0), m_fRange(0.f)
{
}

CRangedWeapon::~CRangedWeapon()
{
    Release();
}


void CRangedWeapon::Follow_Pawn()
{
    Set_Pos(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY);
}

void CRangedWeapon::Fire()
{

}

bool CRangedWeapon::Try_Fire()
{
    //�߻縦 �õ��ߴµ�
    //�ʹ� �߻�ӵ����� ���� ���ٸ�
    int iCurFrame = CTimeMgr::Get_Instance()->Get_CurrentFrame();
    if (m_iLastFireTime + m_iFireRate > iCurFrame)
    {
        //�߻� ���
        return false;
    }

    //�߻� ����. �߻� �ð� ����
    m_iLastFireTime = CTimeMgr::Get_Instance()->Get_CurrentFrame();

    //���������� Ÿ�ٰ� �ڽ��� ��ġ�� ������ ���
    float   fWidth(0.f), fHeight(0.f), fDiagonal(0.f), fRadian(0.f);

    //Ÿ��(���� ����)�� ������ ����
    if (!m_pTarget)
    {
        return false;
    }
    CObj* pTarget = m_pTarget->Get_Target();
    //���� ������ Ÿ���� ������ ����
    if (!pTarget)
    {
        return false;
    }

    fWidth = pTarget->Get_Info().fX - m_tInfo.fX;
    fHeight = pTarget->Get_Info().fY - m_tInfo.fY;

    fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

    fRadian = acosf(fWidth / fDiagonal);

    if (pTarget->Get_Info().fY > m_tInfo.fY)
        fRadian = (2.f * PI) - fRadian;

    m_fAngle = fRadian * (180.f / PI);

    return true;
}

void CRangedWeapon::Initialize()
{
    m_tInfo.fCX = 64.f;
    m_tInfo.fCY = 64.f;

    m_eRenderID = RENDER_WEAPON;

    //�� �߻� ����
    m_iFireRate = 100;
    m_iLastFireTime = CTimeMgr::Get_Instance()->Get_CurrentFrame();
    //���� �����Ÿ�
    m_fRange = 500.f;
}

int CRangedWeapon::Update()
{
    if (m_bDestroyed)
        return OBJ_DESTROYED;

    __super::Update_Rect();

    return OBJ_NOEVENT;
}

void CRangedWeapon::Late_Update()
{
    Follow_Pawn();    
}

void CRangedWeapon::Release()
{
}
