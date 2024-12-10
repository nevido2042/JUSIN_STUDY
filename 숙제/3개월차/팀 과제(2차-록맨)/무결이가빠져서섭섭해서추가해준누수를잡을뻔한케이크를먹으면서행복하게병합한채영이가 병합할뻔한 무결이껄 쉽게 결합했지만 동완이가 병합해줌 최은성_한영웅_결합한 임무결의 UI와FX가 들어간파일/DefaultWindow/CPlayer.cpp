#include "pch.h"
#include "CPlayer.h"
#include "CBullet.h"
#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CLineMgr.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CRolling_Cutter.h"
#include "CRolling_Cutter_P.h"
#include "CWeaponsMenuMgr.h"
#include "CSuper_Arm.h"
#include "CAnimMgr.h"



CPlayer::CPlayer()
{
	ZeroMemory(&m_tPosin, sizeof(POINT));
	m_iScroll_Move = 0;
	m_bDamaged = 0;
	m_ullDamaged_Time = 0;
	m_ullBlink = 0;
	m_iBullet_Cooltime = 0;
	m_ullLast_Bullet = 0;
	m_iHp = 0;
	m_eBullet_ID = BUL_NORMAL;
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_tInfo = { 100.f, WINCY / 2.f, 42.f, 48.f };//초기위치
	m_fSpeed = 10.f;//이동속도
	m_fDistance = 100.f;//이동거리
	m_fJumpPower = 3.f; //기본 점프 파워
	m_iHp = 80; // 플레이어 HP(은성)
	m_iBullet_Cooltime = 200; // 총알 발사주기 ( 은성 )
	m_eDir = DIR_RIGHT; // 보고있는방향 ( 은성 )

	for (int i = 1; i < BUL_END; ++i)
	{
		m_iBulletCount[i] = -1;	
	}
	m_iBulletCount[BUL_NORMAL] = 10;
	m_iBulletCount[BUL_ICE] = 10;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/player_all_2x.bmp", L"Player");
	Initialize_Animation();
}


void CPlayer::Initialize_Animation()
{

	// Right.LT.X = IMAGE_SIZE.X - left.LT.X - FRAME_SIZE.X
	// Left 동일
	// IMAGE_SIZE = 644

	FPOINT t_size = { 48, 68 };

	//run
	ANIMATION_CREATE(L"player_run_left", this, t_size, 130, 4, L"Player_Left");
	ANIMATION_EDIT(L"player_run_left", 124, 0, t_size, 0);
	ANIMATION_EDIT(L"player_run_left", 124 + 48, 0, t_size, 1);
	ANIMATION_EDIT(L"player_run_left", 124 + 48 * 2, 0, t_size, 2);
	ANIMATION_EDIT(L"player_run_left", 124 + 48, 0, t_size, 3);
	ANIMATION_CREATE(L"player_run_right", this, t_size, 130, 4, L"Player_Right");
	ANIMATION_EDIT(L"player_run_right", 472, 0, t_size, 0);
	ANIMATION_EDIT(L"player_run_right", 472 - 48, 0, t_size, 1);
	ANIMATION_EDIT(L"player_run_right", 472 - 48 * 2, 0, t_size, 2);
	ANIMATION_EDIT(L"player_run_right", 472 - 48, 0, t_size, 3);

	//idle
	ANIMATION_CREATE(L"player_idle_right", this, t_size, 500, 1, L"Player_Right");
	ANIMATION_EDIT(L"player_idle_right", 203, 0, t_size, 0);
	ANIMATION_CREATE(L"player_idle_left", this, t_size, 500, 1, L"Player_Left");
	ANIMATION_EDIT(L"player_idle_left", 393, 0, t_size, 0);

	//jump
	int iImgSize = 644;
	t_size = { 52, 68 };
	ANIMATION_CREATE(L"player_jump_left", this, t_size, 500, 1, L"Player_Left");
	ANIMATION_EDIT(L"player_jump_left", 62, 0, t_size, 0);

	ANIMATION_CREATE(L"player_jump_right"	// 애니메이션의 키 값(map.first)
		, this								// this 고정(Get_Info 용도)
		, t_size							// 이미지 한 칸의 사이즈
		, 500								// 프레임 당 시간(ms, 50 ~ 150 추천)
		, 1									// 프레임 수
		, L"Player_Right");					// BmpMgr::Insert_Bmp로 넣을 때 지정한 원본 이미지의 키 값

	ANIMATION_EDIT(L"player_jump_right"		// 애니메이션의 키 값(map.first)
		, iImgSize - 62 - t_size.fX			// 각 프레임의 LT.X 좌표
		// 좌우반전시 (전체 가로길이 - 반대측의 LT.X좌표 - 이미지의 가로길이) 로 써도 되고 그냥 계산해서 넣어도..
		, 0									// 각 프레임의 LT.Y 좌표
		, t_size							// 이미지 한 칸의 사이즈
		, 0);								// 몇번째 프레임인지 (인덱스이므로 0부터 시작)

	//damaged 96 83
	ANIMATION_CREATE(L"player_damaged_right", this, t_size, 500, 1, L"Player_Right");
	ANIMATION_EDIT(L"player_damaged_right", 644 - 96 - 52, 83, t_size, 0);
	ANIMATION_CREATE(L"player_damaged_left", this, t_size, 500, 1, L"Player_Left");
	ANIMATION_EDIT(L"player_damaged_left", 96, 83, t_size, 0);
}
int CPlayer::Update()
{
	if (m_iHp <= 0) // 플레이어 피 소진시


	if (m_bDamaged) // 피격시
	{
		if (m_ullDamaged_Time + 500 > GetTickCount64())
		{
			switch (m_eDir)// 피격 판정 방향
			{
			case DIR_LEFT:
				m_tInfo.fX += 1.f;
				break;
			case DIR_RIGHT:
				m_tInfo.fX -= 1.f;
				break;
			default:
				break;
			}

			if (m_ullDamaged_Time + 200 < GetTickCount64())
				Key_Input();

			__super::Update_Rect();
			return OBJ_NOEVENT;
		}
		else
			m_bDamaged = false;
	}

	Key_Input();
	//CustomJumping(); // 커스텀 점프 로직
	Jumping();


	// 충돌 박스 설정
	__super::Update_Rect();

	return OBJ_NOEVENT;
}
void CPlayer::Late_Update()
{
	if (m_bDamaged)
		return;

	m_CollisionRect = { m_tRect.left, m_tRect.bottom, m_tRect.right, m_tRect.bottom + 5 };
	Offset();// 화면 스크롤 처리

}

void CPlayer::Render(HDC hDC)
{
	//스크롤 위치 계산
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();	

	switch (m_eDir)
	{
	case DIR_LEFT:
		if (m_bDamaged)
		{
			if (m_bBlink)
				ANIMATION_RENDER(hDC, L"player_damaged_left", this);
			if (m_ullBlink + 50 < GetTickCount64())
			{
				m_bBlink = !m_bBlink;
				m_ullBlink = GetTickCount64();
			}
		}
		else if (GetAsyncKeyState('Z'))
			ANIMATION_RENDER(hDC, L"player_jump_left", this);
		else if (GetAsyncKeyState(VK_LEFT))
			ANIMATION_RENDER(hDC, L"player_run_left", this);
		else
			ANIMATION_RENDER(hDC, L"player_idle_left", this);
		break;
	case DIR_RIGHT:
		if (m_bDamaged)
		{
			if (m_bBlink)
				ANIMATION_RENDER(hDC, L"player_damaged_right", this);
			if (m_ullBlink + 50 < GetTickCount64())
			{
				m_bBlink = !m_bBlink;
				m_ullBlink = GetTickCount64();
			}
		}
		else if (GetAsyncKeyState('Z'))
			ANIMATION_RENDER(hDC, L"player_jump_right", this);
		else if (GetAsyncKeyState(VK_RIGHT))
			ANIMATION_RENDER(hDC, L"player_run_right", this);
		else
			ANIMATION_RENDER(hDC, L"player_idle_right", this);

		break;
	}


	TCHAR szBuf[32] = {};
	wsprintf(szBuf, L"플레이어 좌표 : (%d,%d)", (int)m_tInfo.fX, (int)m_tInfo.fY);
	TextOut(hDC, 0, 0, szBuf, lstrlen(szBuf));

	wsprintf(szBuf, L"스크롤 : (%d,%d)", iScrollX, iScrollY);
	TextOut(hDC, 300, 0, szBuf, lstrlen(szBuf));

	wsprintf(szBuf, L"HP : (%d,%d)", m_iHp, 100);
	TextOut(hDC, 0, 30, szBuf, lstrlen(szBuf));
}

void CPlayer::Release()
{
}

void CPlayer::Create_Bullet()
{
	switch (m_eBullet_ID) // 뷸릿 타입 확인 // 나중에 선택하게 하기
	{
	case BUL_NORMAL:
		CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, CAbstractFactory<CRolling_Cutter_P>::Create(m_tInfo.fX, m_tInfo.fY, m_eDir, this, nullptr));
		break;
	default:
		break;
	}

}

void CPlayer::Set_Damaged(const DIRECTION& _eDir)
{
	if (m_bDamaged) //데미지 2번들어올때
		return;

	m_fJumpPower = 0.f;
	m_bDamaged = true;;
	m_ullDamaged_Time = m_ullBlink = GetTickCount64();
	m_eDir = _eDir;
	m_iHp -= 10;
}

void CPlayer::Set_Fallen(const DIRECTION& _eDir)
{
	if (m_bDamaged) //데미지 2번들어올때
		return;

	m_fJumpPower = 0.f;
	m_bDamaged = true;;
	m_ullDamaged_Time = m_ullBlink = GetTickCount64();
	m_eDir = _eDir;
}

void CPlayer::Key_Input()
{
	float	fY(0.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		m_eDir = DIR_LEFT;
		m_tInfo.fX -= m_fSpeed; // 왼쪽으로 이동
	}

	if (GetAsyncKeyState(VK_RIGHT))
	{
		m_eDir = DIR_RIGHT;
		m_tInfo.fX += m_fSpeed; // 오른쪽으로 이동
	}


	if (CKeyMgr::Get_Instance()->Key_Down('Z'))
	{
		if (!m_bJump) // 지면에 있을 때만 점프 시작
		{
			m_bJump = true;
			m_fJumpPower = 6.f; // 초기 점프 파워 설정
		}
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('Z'))
	{

		m_fJumpPower += 0.5f; // 점프 파워 증가

		if (m_bJump)
		{
			m_fJumpPower += 0.5f; // 점프 파워 증가

			if (m_fJumpPower >= 16.f)
				m_fJumpPower = 16.f; // 최대 점프 파워 제한
		}
	}

	//if (CKeyMgr::Get_Instance()->Key_Up('Z'))
	//{
	//	if (m_bisGround)
	//	{
	//		m_fTime = 0.1f; // 시간 초기화
	//		m_bJump = false; // 점프 종료
	//	}
	//}
	if (CKeyMgr::Get_Instance()->Key_Pressing('X'))
	{
		if (m_ullLast_Bullet + m_iBullet_Cooltime < GetTickCount64()) // 총알 발사주기
		{
			Create_Bullet();
			m_ullLast_Bullet = GetTickCount64();
		}
	}

	if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN))
	{
		CWeaponsMenuMgr::Get_Instance()->Open_Menu(m_iBulletCount);
	}
}

void CPlayer::Offset()
{
	/*int		iOffSetX = WINCX >> 1;

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();

	if (iOffSetX > m_tInfo.fX + iScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(m_fSpeed);

	if (iOffSetX < m_tInfo.fX + iScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(-m_fSpeed);*/

		//int iOffSetminX = 100; // 기존의 선생님이 써놨던 코드 
		//int iOffSetmaxX = 700; //

	int		iOffSetminX = 0;
	int		iOffSetmaxX = WINCX;
	int		iOffSetminY = 0;
	int		iOffSetmaxY = WINCY;

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	//기존코드
	//if (iOffSetminX > m_tInfo.fX + iScrollX)
		//CScrollMgr::Get_Instance()->Set_ScrollX(m_fSpeed); // 화면 왼쪽 이동

	//if (iOffSetmaxX < m_tInfo.fX + iScrollX)
		//CScrollMgr::Get_Instance()->Set_ScrollX(-m_fSpeed); // 화면 오른쪽 이동


	if (0 > m_tInfo.fX + m_tInfo.fCX * 0.5f + iScrollX)
		CScrollMgr::Get_Instance()->SET_DIR(DIR_RIGHT);

	if (WINCX < m_tInfo.fX - m_tInfo.fCX * 0.5f + iScrollX)
		CScrollMgr::Get_Instance()->SET_DIR(DIR_LEFT);

	if (0 > m_tInfo.fY + m_tInfo.fCY * 0.5f + iScrollY)
		CScrollMgr::Get_Instance()->SET_DIR(DIR_DOWN);

	if (WINCY < m_tInfo.fY - m_tInfo.fCY * 0.5f + iScrollY)
		CScrollMgr::Get_Instance()->SET_DIR(DIR_UP);

}

//void CPlayer::CustomJumping()
//{
//	float Ground_Y(0.f);
//
//	if (m_bJump)
//	{
//		m_bisGround = false;// 점프 중에는 지면에서 떨어짐
//		float temp = (m_fJumpPower * m_fTime) - (9.8f * m_fTime * m_fTime) * 0.5f;
//		m_tInfo.fY -= temp;
//		m_fTime += 0.1f; // 무결코드는 파워 0.2에서 0.1f 로 수정됨
//
//	}
//	else
//	{
//		if (!m_bisGround) // 지면에 떨어져있을때
//		{
//			//공중에 있을 때의 중력적용
//			m_tInfo.fY += 9.8f * m_fTime * m_fTime * 0.5f; // 중력
//			m_fTime += 0.1f;
//		}
//		else
//		{
//			m_fTime = 0; // 지면 도달 시 초기화
//		}
//	}
//}