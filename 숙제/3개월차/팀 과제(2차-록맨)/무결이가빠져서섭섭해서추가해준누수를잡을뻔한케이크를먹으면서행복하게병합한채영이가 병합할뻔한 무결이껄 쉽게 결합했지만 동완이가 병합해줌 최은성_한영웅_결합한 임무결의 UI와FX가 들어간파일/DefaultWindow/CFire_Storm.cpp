#include "pch.h"
#include "CFire_Storm.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"

void CFire_Storm::Initialize()
{
	m_tInfo.fY -= 40.f;
	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 30.f;
	m_fSpeed = 5.f;
	m_fAngle = 5.f;
	m_pPlayer = CObjMgr::Get_Instance()->Get_Player();
	
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/boss_fire_all.bmp", L"Left_Fire");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/boss_fire_all_X_Change.bmp", L"Right_Fire");
	if (m_eDir == DIR_LEFT)
		m_fSpeed *= -1;
}

int CFire_Storm::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_tInfo.fX += m_fSpeed;

	Update_Rect();

	return 0;
}

void CFire_Storm::Late_Update()
{
}

void CFire_Storm::Render(HDC hDC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX(); // 화면의 X축 스크롤 싸악 가져오고
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY(); // 화면의 Y축 스크롤 싸악 가져오고

	// 방향에 따라 사용할 이미지 선택
	LPCWSTR bulletImage = (m_eDir == DIR_LEFT) ? L"Left_Fire" : L"Right_Fire";
	// m_eDir 값이 DIR_LEFT라면 Left_Fire 이미지를 사용하고 아니라면 "Right_Fire" 이미지 사용

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(bulletImage);

	// 이미지 출력 (투명 처리 포함)
	GdiTransparentBlt(
		hDC,                            // 출력 대상 DC (화면에 출력할 HDC)
		m_tRect.left + iScrollX,        // 출력할 위치의 X 좌표 (현재 스크롤 값을 더해 계산)
		m_tRect.top + iScrollY,         // 출력할 위치의 Y 좌표 (현재 스크롤 값을 더해 계산)
		(int)m_tInfo.fCX + 15,          // 출력할 이미지의 너비 (m_tInfo.fCX 안보여서..+15 추가)
		(int)m_tInfo.fCY + 40,          // 출력할 이미지의 높이 (m_tInfo.fCY 안보여서.. +40 추가)
 		hMemDC,                         // 출력할 원본 이미지의 DC (이미지 핸들)
		(m_eDir == DIR_LEFT) ? 308 : 161, // 원본 이미지에서 복사할 시작 X 좌표 (방향에 따라 다름)
		(m_eDir == DIR_LEFT) ? 98 : 102,  // 원본 이미지에서 복사할 시작 Y 좌표 (방향에 따라 다름)
		(int)m_tInfo.fCX + 15,          // 원본 이미지에서 복사할 영역의 너비 
		(int)m_tInfo.fCY + 40,          // 원본 이미지에서 복사할 영역의 높이 
		RGB(128, 0, 128));             // 투명 처리 색상 (이미지에서 이 색상을 투명하게 처리)
}

void CFire_Storm::Release()
{
}
//m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f));
//m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * (PI / 180.f));