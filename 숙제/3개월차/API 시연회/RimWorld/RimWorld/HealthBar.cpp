#include "pch.h"
#include "HealthBar.h"
#include "ScrollMgr.h"
#include "Pawn.h"

CHealthBar::CHealthBar()
{
}

CHealthBar::~CHealthBar()
{
    Release();
}

void CHealthBar::Initialize()
{
	m_tInfo.fCX = 80.f;
	m_tInfo.fCY = 10.f;

    m_eRenderID = RENDER_INUI;
}

int CHealthBar::Update()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;

	__super::Update_Rect();


	return OBJ_NOEVENT;
}

void CHealthBar::Late_Update()
{
	Follow_Pawn();
}

void CHealthBar::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    // ������ ��� �귯�� ����
    HBRUSH hRedBrush = CreateSolidBrush(RGB(255, 0, 0));   // ������ �귯��
    // ������ ��� �귯�� ����
    HBRUSH hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));   // ������ �귯��
    // ���� HDC�� �⺻ ��� �귯�ø� ����
    HBRUSH hDefaultBrush = (HBRUSH)SelectObject(hDC, hBlackBrush);
    // ������ �簢�� �׸��� (��ġ ����)
    Rectangle(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);

    //HP����
    CPawn* pPawn = static_cast<CPawn*>(m_pTarget);
    float  fHPRatio(0.f);
    try
    {
        //0 ������ Ȯ��
        float fMaxHp = pPawn->Get_MaxHP();
        if (fMaxHp == 0.f)
        {
            throw fMaxHp;
        }

        fHPRatio = pPawn->Get_HP() / pPawn->Get_MaxHP();
    }
    catch(float)
    {
        //�ִ�ü���� 0�̸� ü�¹ٰ� ������� ���׳�����
        fHPRatio = 0.f;
    }

    //ü�¹� ���� 0.f~ 1.f ����
    if (fHPRatio < 0.f)
    {
        fHPRatio = 0.f;
    }
    else if (fHPRatio > 1.f)
    {
        fHPRatio = 1.f;
    }
    
    //HP���� ���� �κ��� Right�� �缳��
    int iRedRight = m_tRect.left + int((m_tRect.right - m_tRect.left) * fHPRatio);
    // ������ �簢�� �׸���
    SelectObject(hDC, hRedBrush);
    Rectangle(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, iRedRight + iScrollX, m_tRect.bottom + iScrollY);

    // ���� ��� �귯�÷� ����
    SelectObject(hDC, hDefaultBrush);

    // ���ҽ� ����
    DeleteObject(hRedBrush);
    DeleteObject(hBlackBrush);
}

void CHealthBar::Release()
{
	
}

void CHealthBar::Follow_Pawn()
{
	Set_Pos(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY + HELATHBAR_OFFSET);
}
