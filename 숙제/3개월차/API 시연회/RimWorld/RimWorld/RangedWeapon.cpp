#include "pch.h"
#include "RangedWeapon.h"
#include "TimeMgr.h"
#include "Pawn.h"

CRangedWeapon::CRangedWeapon()
    :m_fFireRate(0), m_pProjectile(nullptr),
    m_fRange(0.f),m_fElapsedTimeSinceShot(0.f), m_fReloadSpeed(0.f)
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

        /*  �߻� �� ���� �ð� += ���� ���ǵ� * ���� �ӵ�

        �߻� �� ���� �ð��� > ���� �߻� �ð�

        �߻�

        �߻� ��
        �߻��� �����ð� 0, ���� �߻�ð� SET*/

    //���� �߻�ð����� �̸��� �߻� ����
    if (m_fElapsedTimeSinceShot < m_fFireRate)
    {
        return false;
    }

    m_fElapsedTimeSinceShot = 0.f;

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

    m_fAngle = static_cast<CPawn*>(m_pTarget)->Get_TargetAngle();

    return true;
}

void CRangedWeapon::Initialize()
{
    m_tInfo.fCX = 64.f;
    m_tInfo.fCY = 64.f;

    m_eRenderID = RENDER_WEAPON;

    //�� �߻� ����
    m_fFireRate = 100.f;
    m_fReloadSpeed = 1.f;
    //���� �����Ÿ�
    m_fRange = 500.f;
}

int CRangedWeapon::Update()
{
    if (m_bDestroyed)
        return OBJ_DESTROYED;

    //�߻� �� ���� �ð� += ���� ���ǵ� * ���� �ӵ�
    m_fElapsedTimeSinceShot += GAMESPEED * m_fReloadSpeed;

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
