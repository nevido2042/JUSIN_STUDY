#include "pch.h"
#include "Boss_FireMan.h"
#include "CBullet.h"
#include "CScrollMgr.h"
#include "CLineMgr.h"
#include "BlockMgr.h"
#include "CBmpMgr.h"

CBoss_FireMan::CBoss_FireMan()

{

	ZeroMemory(&m_p_Boss_Bullet, sizeof(m_p_Boss_Bullet));
	ZeroMemory(&HP_INFO, sizeof(HP_INFO));
	m_Boss_pPlayer = NULL;
	m_ullLast_Fire = 0;

	Hp_Count = 0;

	m_bJump = false;
	m_fJumpPower=  0.f;
	m_fTime = 0.f;


}

CBoss_FireMan::~CBoss_FireMan()
{
}

void CBoss_FireMan::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/boss_fire_all.bmp",L"Fire_Man");

	m_tInfo = { 300, 400, 42.f, 48.f };
	HP_INFO = { 200,200,200.f,200.f };
	m_fSpeed = 3.f;
	MAX_Hp = 10;
	 
}

int CBoss_FireMan::Update()
{
	m_tInfo.fY += m_fSpeed;

	if (m_bMovingLeft)
	{
		m_tInfo.fX -= m_fSpeed; // 왼쪽으로 이동
		if (m_tInfo.fX <= 200) // 왼쪽 경계
		{
			m_bMovingLeft = false; // 방향 전환
		}
	}
	else
	{
		m_tInfo.fX += m_fSpeed; // 오른쪽으로 이동
		if (m_tInfo.fX >= 500) // 오른쪽 경계
		{
			m_bMovingLeft = true; // 방향 전환
		}
	}
	
	// 랜덤 점프
	if (!m_bJump && rand() % 100 < 2) // 2% 확률로 점프 시작
	{
		m_bJump = true;
		m_fJumpPower = 15.f; // 점프 파워 설정
		m_fTime = 0.f;       // 점프 시간 초기화
	}

	Update_Rect();
	return OBJ_NOEVENT;
}


void CBoss_FireMan::Late_Update()
{
}


void CBoss_FireMan::Render(HDC hDC)
{
	// 스크롤 위치 계산
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMeDC = CBmpMgr::Get_Instance()->Find_Image(L"Fire_Man");

	// 보스 이미지 렌더링
	GdiTransparentBlt(
    hDC,
    m_tRect.left + iScrollX,              // X 좌표
    m_tRect.top + iScrollY - 29,          // Y 좌표를 수정 (보스가 바닥에 맞게 렌더링되도록 오프셋 추가)
    68,                                   // 이미지 가로 크기
    77,                                   // 이미지 세로 크기
    hMeDC,
    51,
    15,
    68,
    77,
    RGB(128, 0, 128));



	// 체력 바 그리기
	Rectangle(hDC, 90, 80, 110, 200); // 전체 체력 바 (최대 체력)

	// 체력 바 색상 설정
	HBRUSH MyBrush = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH OldBrush = (HBRUSH)SelectObject(hDC, MyBrush);

	// 현재 체력에 따른 체력 바 길이 계산&
	Rectangle(hDC, 90, 80, 110, (200 - Hp_Count));

	// 브러시 해제
	SelectObject(hDC, OldBrush);
	DeleteObject(MyBrush);
}




void CBoss_FireMan::Release()
{
}

void CBoss_FireMan::Key_Input()
{
	if (GetAsyncKeyState('2')) // 오른쪽 벽 몰기 패턴
	{
		if (m_ullLast_Fire + 300 > GetTickCount64())
		{
			return;
		}
		m_ullLast_Fire = GetTickCount64();
		m_p_Boss_Bullet->push_back(Create_Bullet(2));
	}
	if (GetAsyncKeyState('3')) // 왼쪽 벽 몰기 패턴
	{
		if (m_ullLast_Fire + 300 > GetTickCount64())
		{
			return;
		}
		m_ullLast_Fire = GetTickCount64();
		m_p_Boss_Bullet->push_back(Create_Bullet(3));

	}
	if (GetAsyncKeyState('4'))  // 5개 유도탄
	{
		if (m_ullLast_Fire + 100 > GetTickCount64())
		{
			return;
		}
		m_ullLast_Fire = GetTickCount64();
		m_p_Boss_Bullet->push_back(Create_Bullet(4));
	}

	if (m_ullLast_Fire + 800 > GetTickCount64())
	{
		return;
	}


}

CObj* CBoss_FireMan::Create_Bullet(int _type)
{
	CObj* pBullet = nullptr;

	

	return pBullet;
}

void CBoss_FireMan::Jumping()
{
	{
		float	fY(0.f);

		//라인충돌감지
		//무결코드는  m_tInfo.fY 추가
		bool bLineCol = CLineMgr::Get_Instance()->Collision_Line(m_tInfo.fX, &fY, m_tInfo.fY);
		if (m_bJump)
		{
			// 포물선 계산에 따라 y 위치 변경
			m_tInfo.fY -= (m_fJumpPower * sinf(45.f) * m_fTime) - (9.8f * m_fTime * m_fTime) * 0.5f;
			m_fTime += 0.2f;

			if (bLineCol && (fY < m_tInfo.fY))
			{
				// 충돌 시 점프 종료
				m_bJump = false;
				m_fTime = 0.f;
				m_tInfo.fY = fY;
			}
		}
		else if (bLineCol)
		{
			// 지면 충돌 시 위치 설정
			m_tInfo.fY = fY;
		}
		else
		{
			// 공중에서 중력 적용
			m_tInfo.fY += 9.8f * m_fTime * m_fTime * 0.5f;
			m_fTime += 0.2f;
		}
	}
}







