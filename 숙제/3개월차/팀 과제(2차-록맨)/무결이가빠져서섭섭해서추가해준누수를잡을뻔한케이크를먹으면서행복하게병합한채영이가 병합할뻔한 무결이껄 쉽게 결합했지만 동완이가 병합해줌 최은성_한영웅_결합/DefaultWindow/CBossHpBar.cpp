#include "pch.h"
#include "CBossHpBar.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CObjMgr.h"

CBossHpBar::CBossHpBar(): m_pBossMonster(nullptr), m_keyName(nullptr)
{
    ZeroMemory(&m_tHp_100, sizeof(IMAGE));
    ZeroMemory(&m_tHp_80, sizeof(IMAGE));
    ZeroMemory(&m_tHp_60, sizeof(IMAGE));
    ZeroMemory(&m_tHp_50, sizeof(IMAGE));
    ZeroMemory(&m_tHp_40, sizeof(IMAGE));
    ZeroMemory(&m_tHp_20, sizeof(IMAGE));
    ZeroMemory(&m_tHp_10, sizeof(IMAGE));
    ZeroMemory(&m_tHp_00, sizeof(IMAGE));
    ZeroMemory(&m_tCurHp, sizeof(IMAGE));
}

CBossHpBar::CBossHpBar(CObj* _Boss) : m_pBossMonster(_Boss), m_keyName(nullptr)
{
	ZeroMemory(&m_tHp_100, sizeof(IMAGE));
	ZeroMemory(&m_tHp_80, sizeof(IMAGE));
	ZeroMemory(&m_tHp_60, sizeof(IMAGE));
	ZeroMemory(&m_tHp_50, sizeof(IMAGE));
	ZeroMemory(&m_tHp_40, sizeof(IMAGE));
	ZeroMemory(&m_tHp_20, sizeof(IMAGE));
	ZeroMemory(&m_tHp_10, sizeof(IMAGE));
	ZeroMemory(&m_tHp_00, sizeof(IMAGE));
	ZeroMemory(&m_tCurHp, sizeof(IMAGE));
}

CBossHpBar::~CBossHpBar()
{
}

void CBossHpBar::Initialize()
{
	m_tInfo = { 90, 90, 18, 115 };

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/Hp_Bar/Cut_Hp.bmp", L"Boss_Cut");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/Hp_Bar/Elec_hp.bmp", L"Boss_Elec");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/Hp_Bar/Fire_Hp.bmp", L"Boss_Fire");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/Hp_Bar/Ground_Hp.bmp", L"Boss_Ground");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/Hp_Bar/Ice_Hp.bmp", L"Boss_Ice");


	BOSSTYPE temp = m_pBossMonster->GetBossType();
	switch (temp)
	{
	case BT_ELECMAN:
		m_keyName = L"Boss_Elec";
		break;
	case BT_FIREMAN:
		m_keyName = L"Boss_Fire";
		break;
	case BT_CUTMAN:
		m_keyName = L"Boss_Cut";
		break;
	case BT_GUTSMAN:
		m_keyName = L"Boss_Ground";
		break;
	case BT_ICEMAN:
		m_keyName = L"Boss_Ice";
		break;
	case BT_END:
		break;
	default:
		break;
	}

	m_tHp_00 = { 0, 0 };
	m_tHp_10 = { 19, 0 };
	m_tHp_20 = { 37, 0 };
	m_tHp_40 = { 55, 0 };
	m_tHp_50 = { 73, 0 };
	m_tHp_60 = { 91, 0 };
	m_tHp_80 = { 109, 0 };
	m_tHp_100 = { 128, 0 };
	
}

int CBossHpBar::Update()
{
	if (m_pBossMonster->GetHp() >= 90)
		m_tCurHp = m_tHp_100;
	else if (m_pBossMonster->GetHp() > 70)
		m_tCurHp = m_tHp_80;
	else if (m_pBossMonster->GetHp() > 60)
		m_tCurHp = m_tHp_60;
	else if (m_pBossMonster->GetHp() > 50)
		m_tCurHp = m_tHp_50;
	else if (m_pBossMonster->GetHp() > 40)
		m_tCurHp = m_tHp_40;
	else if (m_pBossMonster->GetHp() > 30)
		m_tCurHp = m_tHp_20;
	else if (m_pBossMonster->GetHp() > 5)
		m_tCurHp = m_tHp_10;
	else
		m_tCurHp = m_tHp_00;

	__super::Update_Rect();
    return 0;
}

void CBossHpBar::Late_Update()
{
}

void CBossHpBar::Render(HDC hdc)
{
	//HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_keyName);
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_keyName);

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hdc,						// ���� ���� DC
		m_tInfo.fX ,
		m_tInfo.fY ,					// ���� ���� ��ġ ��ǥ X, Y			
		m_tInfo.fCX, m_tInfo.fCY,				// ���� ���� �̹����� ����, ����
		hMemDC,									// ������ �̹��� DC	
		m_tCurHp._iX, m_tCurHp._iY,				// ��Ʈ�� ��� ���� ��ǥ(Left, top)
		m_tInfo.fCX, m_tInfo.fCY,				/// ������ �̹����� ����, ����
		RGB(128, 0, 128));						// ��� ȿ�� ����(�״�� ���)

}

void CBossHpBar::Release()
{
}
