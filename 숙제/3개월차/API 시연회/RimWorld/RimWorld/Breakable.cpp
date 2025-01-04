#include "pch.h"
#include "Breakable.h"

CBreakable::CBreakable()
    :m_bBrokendown(false), m_fDurability(0.f), m_fMaxDurability(0.f)
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
	if (m_bDestroyed)
	{
		return OBJ_DESTROYED;
	}

	if (m_bBrokendown)
	{
		Set_Destroyed();
	}

	__super::Update_Rect();

	return OBJ_NOEVENT;
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
