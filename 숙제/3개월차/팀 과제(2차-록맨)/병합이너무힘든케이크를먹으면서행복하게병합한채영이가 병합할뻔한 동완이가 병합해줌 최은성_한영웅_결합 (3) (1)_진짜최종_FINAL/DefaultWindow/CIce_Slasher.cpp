#include "pch.h"
#include "CIce_Slasher.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"




void CIce_Slasher::Initialize()
{
	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 30.f;

	m_fSpeed = 5.f;

	if (m_eDir == DIR_LEFT)
		m_fSpeed *= -1;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/boss_ice_all.bmp", L"BossIceAll");
}

void CIce_Slasher::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"BossIceAll");

	GdiTransparentBlt(hDC,            // 복사 받을 DC
		m_tRect.left + iScrollX,    // 복사 받을 위치 좌표 X, Y    
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,            // 복사 받을 이미지의 가로, 세로    라이트 - 레프트 해서 길이를 넣어줘야하니까
		(int)m_tInfo.fCY,
		hMemDC,                        // 복사할 이미지 DC    
		553,                            // 비트맵 출력 시작 좌표(Left, top)
		21,
		(int)m_tInfo.fCX,            // 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB(128, 0, 128));            // 제거할 색상
}

void CIce_Slasher::OnCollision(CObj* _pOther, OBJID _eOtherID)
{
	if (_eOtherID == OBJ_PLAYER)
	{
		//_pOther->Set_
	}
}

int CIce_Slasher::Update()
{
	if (m_bDead)
		return OBJ_DEAD;


	m_tInfo.fX += m_fSpeed;

	__super::Update_Rect();

	return 0;
}

void CIce_Slasher::Late_Update()
{
}

void CIce_Slasher::Release()
{
}
