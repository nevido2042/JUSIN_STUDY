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

    //���� ó��
    if (m_fDurability <= 0.f)
    {
        m_bBrokendown = true;
        //Set_Destroyed(); //�ٷ� ���������� �����ؾ߰ڴ�.
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
        //���� �߿� �ڽ��� Ÿ������ ������ �ִ� ������ Ÿ���� ���־��Ѵ�.
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
