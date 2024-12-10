#include "pch.h"
#include "CWeaponsMenuMgr.h"
#include "CKeyMgr.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"

#include "CPlayer.h"

CWeaponsMenuMgr* CWeaponsMenuMgr::m_pInstance = nullptr;

CWeaponsMenuMgr::CWeaponsMenuMgr() : m_eState(NONE), m_iAnimationFrame(), m_iSelect(),
m_iAnimationX(),m_iAnimationY()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/Weapons_Menu_2x.bmp", L"Menu");
	m_ullAnimationTIme = 0;

	m_iAnimationY = 24;
	m_iAnimationX = 5;
	m_iCX = 186;
	m_iCY = 315;
	m_ullBlink = 0;
}

CWeaponsMenuMgr::~CWeaponsMenuMgr()
{
	Release();
}

bool CWeaponsMenuMgr::Update() // 인벤토리가 열려있을때
{
	switch (m_eState)
	{
	case NONE:
		return false;
	case OPENING:
		Opening();
		break;
	case ONSCREAN:
		OnScrean();
		break;
	case CLOSING:
		Closing();
		break;
	}
	return true; // 트루가 리턴되면 스크롤처럼 오브젝트 정지
}

void CWeaponsMenuMgr::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Menu");

	GdiTransparentBlt(hDC,            // 복사 받을 DC
		450,    // 복사 받을 위치 좌표 X, Y    
		100,
		m_iCX,            // 복사 받을 이미지의 가로, 세로
		m_iCY,
		hMemDC,                        // 복사할 이미지 DC    
		m_iAnimationX,                            // 비트맵 출력 시작 좌표(Left, top)
		m_iAnimationY,
		m_iCX,            // 복사할 이미지의 가로, 세로
		m_iCY,
		RGB(128, 0, 128));

	if (m_eState == ONSCREAN)
	{
		GdiTransparentBlt(hDC,            // 복사 받을 DC
			465,    // 복사 받을 위치 좌표 X, Y    
			134,
			160,            // 복사 받을 이미지의 가로, 세로
			34,
			hMemDC,                        // 복사할 이미지 DC    
			2736,                            // 비트맵 출력 시작 좌표(Left, top)
			86,
			160,            // 복사할 이미지의 가로, 세로
			34,
			RGB(128, 0, 128));


		for (int i = 1; i < BUL_ELEC_SMALL; ++i)
		{
			if (m_pBulCount[i] == -1)
			{
				GdiTransparentBlt(hDC,            // 복사 받을 DC
					465,    // 복사 받을 위치 좌표 X, Y    
					134+i*32,
					160,            // 복사 받을 이미지의 가로, 세로
					34,
					hMemDC,                        // 복사할 이미지 DC    
					2736,                            // 비트맵 출력 시작 좌표(Left, top)
					86,
					160,            // 복사할 이미지의 가로, 세로
					34,
					RGB(128, 0, 128));
			}
			//else // 탄창하려했는데 시간부족할듯
			//{
			//	GdiTransparentBlt(hDC,            // 복사 받을 DC
			//		498,    // 복사 받을 위치 좌표 X, Y    
			//		138 + i * 32,
			//		16,            // 복사 받을 이미지의 가로, 세로
			//		16,
			//		hMemDC,                        // 복사할 이미지 DC    
			//		3121,                            // 비트맵 출력 시작 좌표(Left, top)
			//		76,
			//		16,            // 복사할 이미지의 가로, 세로
			//		16,
			//		RGB(128, 0, 128));

			//}
		}
		if (m_bBlink)
		{
			if (m_iSelect % BUL_END == 0)
			{
				GdiTransparentBlt(hDC,            // 복사 받을 DC
					465,    // 복사 받을 위치 좌표 X, Y    
					134 + 6 * 32,
					34,            // 복사 받을 이미지의 가로, 세로
					34,
					hMemDC,                        // 복사할 이미지 DC    
					2736,                            // 비트맵 출력 시작 좌표(Left, top)
					86,
					34,            // 복사할 이미지의 가로, 세로
					34,
					RGB(128, 0, 128));
			}
			else
			{
				GdiTransparentBlt(hDC,            // 복사 받을 DC
					465,    // 복사 받을 위치 좌표 X, Y    
					134 + (m_iSelect % BUL_END) * 32,
					34,            // 복사 받을 이미지의 가로, 세로
					34,
					hMemDC,                        // 복사할 이미지 DC    
					2736,                            // 비트맵 출력 시작 좌표(Left, top)
					86,
					34,            // 복사할 이미지의 가로, 세로
					34,
					RGB(128, 0, 128));
			}
		}

	}

}

void CWeaponsMenuMgr::Release()
{
}

CWeaponsMenuMgr* CWeaponsMenuMgr::Get_Instance()
{
	if (m_pInstance == nullptr)
		m_pInstance = new CWeaponsMenuMgr;
	return m_pInstance;
}

void CWeaponsMenuMgr::Destroy_Instance()
{
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

void CWeaponsMenuMgr::Open_Menu(const int* _BulCount)
{
	m_eState = OPENING;
	m_pBulCount = _BulCount;
}

void CWeaponsMenuMgr::Opening()
{
	if (m_ullAnimationTIme + 30 < GetTickCount64())
	{
		m_iAnimationY = 24;
		m_iAnimationX = 5 + (m_iCX + 8) * m_iAnimationFrame;
		m_ullAnimationTIme = GetTickCount64();
		if (m_iAnimationFrame == 14)
		{
			m_eState = ONSCREAN;
		}
		m_iAnimationFrame++;
	}
}

void CWeaponsMenuMgr::OnScrean()
{
	m_iAnimationX = 2923;

	if (m_ullBlink + 200 < GetTickCount64())
	{
		m_bBlink = !m_bBlink;
		m_ullBlink = GetTickCount64();
	}

	if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN))
	{
		m_iAnimationY = 375;
		m_iAnimationX = 5;
		m_eState = CLOSING;
	}

	if (CKeyMgr::Get_Instance()->Key_Down(VK_DOWN))
	{
		do {
			m_iSelect++;
		} while (m_pBulCount[m_iSelect % BUL_END] == -1);
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_UP))
	{
		do {
			m_iSelect--;
		} while (m_pBulCount[m_iSelect % BUL_END] == -1);
	}

	if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
	{
		static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_BulId((BULLET_ID)(m_iSelect % BUL_END));
		m_iAnimationY = 375;
		m_iAnimationX = 5;
		m_eState = CLOSING;
	}
}

void CWeaponsMenuMgr::Closing()
{
	if (m_ullAnimationTIme + 30 < GetTickCount64())
	{
		m_iAnimationY = 375;
		m_iAnimationX = 5 + (m_iCX + 8) * (m_iAnimationFrame-14);
		m_ullAnimationTIme = GetTickCount64();
		if (m_iAnimationFrame == 28)
		{
			m_eState = NONE;
			m_iAnimationFrame = 0;
			return;
		}
		m_iAnimationFrame++;
	}
}
