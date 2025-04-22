#include "pch.h"
#include "Breakable.h"
#include "ObjMgr.h"

CBreakable::CBreakable()
    :m_bBrokendown(false), m_fDurability(0.f), m_fMaxDurability(0.f), m_bTaskReserved(false)
{
}

CBreakable::~CBreakable()
{
    Release();
}

void CBreakable::Take_Damage(float _fDamage)
{
    m_fDurability -= _fDamage;

    //죽음 처리
    if (m_fDurability <= 0.f)
    {
        m_bBrokendown = true;
        //Set_Destroyed(); //바로 지우지말고 예약해야겠다.
    }
}

void CBreakable::Initialize()
{
    m_fMaxDurability = 10.f;
    m_fDurability = m_fMaxDurability;
}

int CBreakable::Update()
{
	if (m_bBrokendown)
	{
        //림들 중에 자신을 타겟으로 가지고 있는 림들의 타겟을 없애야한다.
        list<CObj*> RimList = CObjMgr::Get_Instance()->Get_List()[OBJ_RIM];
        for (CObj* pRim : RimList)
        {
            if (pRim->Get_Target() == this)
            {
                pRim->Set_Target(nullptr);
            }
        }

		Set_Destroyed();
	}

    return CObj::Update();
}

void CBreakable::Late_Update()
{
}

void CBreakable::Render(HDC hDC)
{
}

void CBreakable::Release()
{
}
