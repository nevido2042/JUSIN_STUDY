#include "pch.h"
#include "Boomrat.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "KeyMgr.h"
#include "ColonyMgr.h"
#include "SteelWall.h"
#include "TileMgr.h"
#include "SoundMgr.h"
#include "EffectMgr.h"

CBoomrat::CBoomrat()
{
}

CBoomrat::~CBoomrat()
{
    Release();
}

void CBoomrat::Initialize()
{
    CEnemy::Initialize();

    m_fSpeed = 1.5f;

    m_fMaxHP = 10.f;
    m_fHP = m_fMaxHP;

    //Take_Damage(99.f);

    Change_State(WANDERING);
}

int CBoomrat::Update()
{
    return CEnemy::Update();
}

void CBoomrat::Late_Update()
{
    CPawn::Late_Update();
}

void CBoomrat::Render(HDC hDC)
{
    // ȭ�� ��ũ�� ���� �����ɴϴ�.
    int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    // ȭ�� Ȯ��/��� ������ �����ɴϴ�.
    float fZoom = CCamera::Get_Instance()->Get_Zoom();

    // ���� ��ǥ�� ��ũ�� ��ǥ�� ��ȯ�մϴ�.
    POINT tScreenPos = CCamera::Get_Instance()->WorldToScreen((float)m_tRect.left, (float)m_tRect.top);

    // �� ����, ��, �Ӹ� ��
    HDC hTestDC(nullptr);
    switch (m_eDir)
    {
    case UU:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Boomrat_north");
        // ����
        GdiTransparentBlt(hDC,
            (int)(tScreenPos.x),  // ��ġ ������ �ϰ� fZoom�� ũ�� �������� ���
            (int)(tScreenPos.y),
            (int)(64 * fZoom),  // ũ�� ���� (fZoom�� ũ�� ����)
            (int)(64 * fZoom),
            hTestDC, 0, 0, 64, 64,
            RGB_PURPLE);
        break;

    case RR:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Boomrat_east");
        // ����
        GdiTransparentBlt(hDC,
            (int)(tScreenPos.x),  // ��ġ ������ �ϰ� fZoom�� ũ�� �������� ���
            (int)(tScreenPos.y),
            (int)(64 * fZoom),  // ũ�� ���� (fZoom�� ũ�� ����)
            (int)(64 * fZoom),
            hTestDC, 0, 0, 64, 64,
            RGB_PURPLE);
        break;

    case DD:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Boomrat_south");
        // ����
        GdiTransparentBlt(hDC,
            (int)(tScreenPos.x),  // ��ġ ������ �ϰ� fZoom�� ũ�� �������� ���
            (int)(tScreenPos.y),
            (int)(64 * fZoom),  // ũ�� ���� (fZoom�� ũ�� ����)
            (int)(64 * fZoom),
            hTestDC, 0, 0, 64, 64,
            RGB_PURPLE);
        break;

    case LL:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Boomrat_west");
        // ����
        GdiTransparentBlt(hDC,
            (int)(tScreenPos.x),  // ��ġ ������ �ϰ� fZoom�� ũ�� �������� ���
            (int)(tScreenPos.y),
            (int)(64 * fZoom),  // ũ�� ���� (fZoom�� ũ�� ����)
            (int)(64 * fZoom),
            hTestDC, 0, 0, 64, 64,
            RGB_PURPLE);
        break;

    default:
        break;
    }

}

void CBoomrat::Dead()
{
    CPawn::Dead();
    CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
    CSoundMgr::Get_Instance()->PlaySound(L"Boomrat_Death_1b.wav", SOUND_EFFECT, .1f);
    Boom();
}

void CBoomrat::Handle_Chasing()
{
    if (!m_pTarget)
    {
        Change_State(WANDERING);
    }

    //Ÿ�� ������ ���󰡱�
    if (m_pTarget)
    {
        POS tMoveToPos{ (int)m_pTarget->Get_Info().fX, (int)m_pTarget->Get_Info().fY };

        //Ÿ���� Pawn�̶��
        if (CPawn* pPawnTarget = dynamic_cast<CPawn*>(m_pTarget))//Ÿ���� Pawn�̰� �׾�����
        {
            //Pawn�� �׾��ٸ�
            if (pPawnTarget->Get_IsDead())
            {
                Set_Target(nullptr);
                //RequestNavStop();
                m_bNavigating = false;
            }
        }

        if (!m_bNavigating)
        {
            Move_To(tMoveToPos);
        }
    }

    //�����Ÿ� ���� �ְ�, ���� ��������
    if (IsWithinRange() && IsCanSeeTarget())
    {
        //RequestNavStop();
        m_bNavigating = false;
        //static_cast<CRangedWeapon*>(m_pRangedWeapon)->Fire();
        m_bAttack = true;
        //����
        Boom();
    }
    else
    {
        m_bAttack = false;
    }
}

void CBoomrat::Boom()
{
    //���� ����Ʈ
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            POS tPos{ (int)m_tInfo.fX + j * TILECX, (int)m_tInfo.fY + i * TILECY };
            CEffectMgr::Get_Instance()->Create_Effect(tPos, 64.f, 64.f, L"BlastFlame", 2.f);
        }
    }

    //���� ����
    CSoundMgr::Get_Instance()->StopSound(SOUND_EXPLOSION);
    CSoundMgr::Get_Instance()->PlaySound(L"GiantExplosion.wav", SOUND_EXPLOSION, 0.5f);

    //���� ���� ��ŭ
    //üũ�ؼ�
    //�������� �ش�.
    //�ڱ� �ڽ��� ���δ�.
    float fExplosionRadius(100.f);
    //float fDamage(100.f);

    for (CObj* pObj : CObjMgr::Get_Instance()->Get_List()[OBJ_RIM])
    {
        float fDist =  CObj::Calculate_Dist(this, pObj);

        if (fExplosionRadius > fDist)
        {
            static_cast<CPawn*>(pObj)->Take_Damage(m_fMaxHP);
        }
    }
    for (CObj* pObj : CObjMgr::Get_Instance()->Get_List()[OBJ_ENEMY])
    {
        float fDist = CObj::Calculate_Dist(this, pObj);

        //if (pObj == this)
        //{
        //    if (static_cast<CPawn*>(pObj)->Get_IsDead())
        //    {
        //        // �ι� ���̸� ���� ���׳�
        //        continue;
        //    }
        //}

        if (fExplosionRadius > fDist)
        {
            static_cast<CPawn*>(pObj)->Take_Damage(m_fMaxHP);
        }
    }
    for (CObj* pObj : CObjMgr::Get_Instance()->Get_List()[OBJ_WALL])
    {
        float fDist = CObj::Calculate_Dist(this, pObj);

        if (fExplosionRadius > fDist)
        {
            static_cast<CSteelWall*>(pObj)->Take_Damage(m_fMaxHP);
        }
    }

}
