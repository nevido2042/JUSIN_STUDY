#include "pch.h"
#include "BoltActionRifle.h"
#include "BmpMgr.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "Rim.h"

CBoltActionRifle::CBoltActionRifle()
{
}

CBoltActionRifle::~CBoltActionRifle()
{
	Release();
}

void CBoltActionRifle::Fire()
{
    if (!Try_Fire())
    {
        return;
    }

    CObj* pObj = CAbstractFactory<CBullet_Small>::Create(m_tInfo.fX, m_tInfo.fY, m_fAngle);
    CObjMgr::Get_Instance()->Add_Object(OBJ_PROJECTILE, pObj);
    static_cast<CProjectile*>(pObj)->Set_Owner(m_pTarget);//총알의 주인을 총의 소유자로 설정한다.
}

void CBoltActionRifle::Render(HDC hDC)
{
    //림이 소집상태가 아니면 무기 출력 하지 않도록
    if (m_pTarget)
    {
        if (CRim* pTargetRim = dynamic_cast<CRim*>(m_pTarget))
        {
            if (pTargetRim->Get_State() != CRim::DRAFTED)
            {
                return;
            }
        }
    }

    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    //무기 출력
    HDC hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"BoltActionRifle");
    GdiTransparentBlt(hDC,
        m_tRect.left + iScrollX,
        m_tRect.top + iScrollY,
        64, 64,
        hTestDC, 0, 0, 64, 64,
        RGB_WHITE);
}
