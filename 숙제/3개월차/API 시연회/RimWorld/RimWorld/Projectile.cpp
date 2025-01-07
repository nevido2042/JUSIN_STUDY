#include "pch.h"
#include "Projectile.h"
#include "TimeMgr.h"
#include "Pawn.h"
#include "SteelWall.h"
#include "SoundMgr.h"

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

void CProjectile::Render(HDC hDC)
{
    //int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    //int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    //ȭ�� ��ũ�� ���� �����ɴϴ�.
    //int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    //int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    // ȭ�� Ȯ��/��� ������ �����ɴϴ�.
    //float fZoom = CCamera::Get_Instance()->Get_Zoom();

    // ���� ��ǥ�� ��ũ�� ��ǥ�� ��ȯ�մϴ�.
    POINT tScreenPosLeft = CCamera::Get_Instance()->WorldToScreen(m_tRect.left, m_tRect.top);
    POINT tScreenPosRight = CCamera::Get_Instance()->WorldToScreen(m_tRect.right, m_tRect.bottom);

    // Ellipse�� ���� ��ġ�� ũ�� ����
    int left = (int)(tScreenPosLeft.x);  // ���� ��� ��ġ ����
    int top = (int)(tScreenPosLeft.y);  // ���� ��� ��ġ ����
    int right = (int)((tScreenPosRight.x));  // ������ ��ġ ũ�� ����
    int bottom = (int)((tScreenPosRight.y));  // �Ʒ��� ��ġ ũ�� ����

    // ������ �׸���
    Ellipse(hDC,
        left,
        top,
        right,
        bottom
    );

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
        if (_eID == OBJ_RIM)
        {
            //CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
            CSoundMgr::Get_Instance()->PlaySound(L"Bullet_Flesh_1a.wav", SOUND_FLESH, .3f);
        }
        else
        {
            //CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
            CSoundMgr::Get_Instance()->PlaySound(L"Bullet_Metal_1g.wav", SOUND_METAL, .05f);
        }

        static_cast<CPawn*>(_pOther)->Take_Damage(m_fDamage);
        Set_Destroyed();
    }
    else if (_eID == OBJ_WALL)
    {
        static_cast<CSteelWall*>(_pOther)->Take_Damage(m_fDamage);
        Set_Destroyed();
    }
}
