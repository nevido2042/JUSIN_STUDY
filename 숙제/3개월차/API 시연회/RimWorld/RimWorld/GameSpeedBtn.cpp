#include "pch.h"
#include "GameSpeedBtn.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"

CGameSpeedBtn::CGameSpeedBtn()
{
}

CGameSpeedBtn::~CGameSpeedBtn()
{
	Release();
}

void CGameSpeedBtn::Initialize()
{
	m_tInfo.fCX = 64.f;
	m_tInfo.fCY = 48.f;

	m_eRenderID = RENDER_UI;
}

void CGameSpeedBtn::Late_Update()
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (PtInRect(&m_tRect, ptMouse))
	{
		if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
		{
			if (!lstrcmp(L"TimeSpeedButton_Pause", m_pImgKey))
			{
				CTimeMgr::Get_Instance()->Set_GameSpeed(0.f);
			}
			else if (!lstrcmp(L"TimeSpeedButton_Normal", m_pImgKey))
			{
				CTimeMgr::Get_Instance()->Set_GameSpeed(1.f);

			}
			else if (!lstrcmp(L"TimeSpeedButton_Fast", m_pImgKey))
			{
				CTimeMgr::Get_Instance()->Set_GameSpeed(2.f);

			}
			else if (!lstrcmp(L"TimeSpeedButton_Superfast", m_pImgKey))
			{
				CTimeMgr::Get_Instance()->Set_GameSpeed(3.f);

			}
				

			return;
		}

		m_iDrawID = 1;
	}

	else
		m_iDrawID = 0;
}

void CGameSpeedBtn::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	GdiTransparentBlt(
		hDC,                          // ��� DC
		m_tRect.left,                 // ����� X ��ǥ
		m_tRect.top,                  // ����� Y ��ǥ
		(int)m_tInfo.fCX,             // ��� �ʺ�
		(int)m_tInfo.fCY,             // ��� ����
		hMemDC,                       // ���� DC
		m_iDrawID * (int)m_tInfo.fCX, // ���� �̹����� X ��ǥ
		0,                            // ���� �̹����� Y ��ǥ
		(int)m_tInfo.fCX,             // ���� �̹����� �ʺ�
		(int)m_tInfo.fCY,             // ���� �̹����� ����
		RGB_WHITE		              // �����ϰ� ó���� ���� (��: �����)
	);
}

void CGameSpeedBtn::Release()
{
}
