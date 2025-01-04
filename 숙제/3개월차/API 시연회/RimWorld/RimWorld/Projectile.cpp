#include "pch.h"
#include "Projectile.h"
#include "TimeMgr.h"
#include "Pawn.h"
#include "SteelWall.h"

CProjectile::CProjectile()
    :m_fDamage(0.f), m_pOwner(nullptr)
{
}

CProjectile::~CProjectile()
{
    Release();
}

void CProjectile::Initialize()
{
    m_tInfo.fCX = 10.f;
    m_tInfo.fCY = 10.f;

    m_eRenderID = RENDER_GAMEOBJECT;

    m_fSpeed = 20.f;

    m_fDamage = 7.f;
}

int CProjectile::Update()
{
    return CObj::Update();
}

void CProjectile::Late_Update()
{
    //Angle������ ���󰣴�.
    float fRadian = m_fAngle * (PI / 180.f);

    m_tInfo.fX += m_fSpeed * cosf(fRadian) * GAMESPEED;
    m_tInfo.fY -= m_fSpeed * sinf(fRadian) * GAMESPEED;
}

void CProjectile::Release()
{
}

void CProjectile::OnCollision(OBJID _eID, CObj* _pOther)
{
    //�Ѿ��� �� ������ �΋H���� �����Ѵ�.
    if (m_pOwner == _pOther)
    {
        return;
    }
    //�̹� ���� �ָ� �����Ѵ�.
    if (static_cast<CPawn*>(_pOther)->Get_IsDead())
    {
        return;
    }
    //���̰ų�, ��ī���̵�� �������ְ� ��������.
    if (_eID == OBJ_RIM || _eID == OBJ_ENEMY)
    {
        static_cast<CPawn*>(_pOther)->Take_Damage(m_fDamage);
        Set_Destroyed();
    }
    else if (_eID == OBJ_WALL)
    {
        static_cast<CSteelWall*>(_pOther)->Take_Damage(m_fDamage);
        Set_Destroyed();
    }
}
