#include "pch.h"
#include "CHpBar.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CObjMgr.h"
#include "CPlayer.h"

CHpBar::CHpBar():
m_pPlayer(nullptr)
{
	ZeroMemory(&m_tHp_100, sizeof(IMAGE));
	ZeroMemory(&m_tHp_80, sizeof(IMAGE));
	ZeroMemory(&m_tHp_60, sizeof(IMAGE));
	ZeroMemory(&m_tHp_50, sizeof(IMAGE));
	ZeroMemory(&m_tHp_40, sizeof(IMAGE));
	ZeroMemory(&m_tHp_20, sizeof(IMAGE));
	ZeroMemory(&m_tHp_10, sizeof(IMAGE));
	ZeroMemory(&m_tHp_00, sizeof(IMAGE));
	ZeroMemory(&m_tNowState, sizeof(IMAGE));

}

CHpBar::~CHpBar()
{
}

void CHpBar::Initialize()
{
	m_tInfo = { 60, 90, 18, 115 };
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/Hp_Bar/Player_Hp.bmp", L"PlayerHp");
	m_pPlayer = CObjMgr::Get_Instance()->Get_Player();

	///이미지 좌표
	m_tHp_100 = {128, 0};
	m_tHp_80 = {19, 0};
	m_tHp_60 = {37, 0};
	m_tHp_50 = {55, 0};
	m_tHp_40 = {73, 0};
	m_tHp_20 = {91, 0};
	m_tHp_10 = {109, 0};
	m_tHp_00 = {0, 0};
}

int CHpBar::Update()
{
	if (m_pPlayer->GetHp() >= 80)
		m_tNowState = m_tHp_100;
	else if (m_pPlayer->GetHp() >= 70)
		m_tNowState = m_tHp_80;
	else if (m_pPlayer->GetHp() >= 60)
		m_tNowState = m_tHp_60;
	else if (m_pPlayer->GetHp() >= 50)
		m_tNowState = m_tHp_50;
	else if (m_pPlayer->GetHp() >= 40)
		m_tNowState = m_tHp_40;
	else if (m_pPlayer->GetHp() >= 30)
		m_tNowState = m_tHp_20;
	else if (m_pPlayer->GetHp() >= 20)
		m_tNowState = m_tHp_10;
	else if (m_pPlayer->GetHp() >= 10)
		m_tNowState = m_tHp_10;
	else
		m_tNowState = m_tHp_00;

	__super::Update_Rect();
	return 0;
}

void CHpBar::Late_Update()
{

}

void CHpBar::Render(HDC hdc)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"PlayerHp");

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hdc,						// 복사 받을 DC
		m_tInfo.fX ,
		m_tInfo.fY ,					// 복사 받을 위치 좌표 X, Y			
		m_tInfo.fCX, m_tInfo.fCY,				// 복사 받을 이미지의 가로, 세로
		hMemDC,									// 복사할 이미지 DC	
		m_tNowState._iX, m_tNowState._iY,		// 비트맵 출력 시작 좌표(Left, top)
		m_tInfo.fCX, m_tInfo.fCY,				/// 복사할 이미지의 가로, 세로
		RGB(128, 0, 128));						// 출력 효과 설정(그대로 출력)


	//Ellipse(hdc, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CHpBar::Release()
{
}
