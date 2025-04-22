#include "pch.h"
#include "LaunchMsgBox.h"
#include "ObjMgr.h"
#include "Pawn.h"

CLaunchMsgBox::CLaunchMsgBox()
	:m_iRimCount(0)
{
}

CLaunchMsgBox::~CLaunchMsgBox()
{
	Release();
}

void CLaunchMsgBox::Initialize()
{
	CMsgBox::Initialize();

	list<CObj*> RimList = CObjMgr::Get_Instance()->Get_List()[OBJ_RIM];

	for (CObj* pObj : RimList)
	{
		if (CPawn* pPawn = dynamic_cast<CPawn*>(pObj))
		{
			if (pPawn->Get_State() == CPawn::BOARDING)
			{
				++m_iRimCount;
			}
		}
	}
}

void CLaunchMsgBox::Render(HDC hDC)
{
	CMsgBox::Render(hDC);

	// Step 1: Create a new font
	HFONT hFont = CreateFont(
		24,                        // Height of font
		0,                         // Width of font (0 means default)
		0,                         // Angle of text rotation (0 for horizontal)
		0,                         // Angle of text rotation (0 for horizontal)
		FW_NORMAL,                 // Weight (normal font)
		FALSE,                     // Italic
		FALSE,                     // Underline
		FALSE,                     // Strikeout
		ANSI_CHARSET,              // Character set
		OUT_DEFAULT_PRECIS,        // Output precision
		CLIP_DEFAULT_PRECIS,       // Clipping precision
		DEFAULT_QUALITY,           // Quality
		DEFAULT_PITCH | FF_SWISS,  // Pitch and family
		L"맑은 고딕"                   // Font name
	);


	// Step 2: Select the font into the device context
	HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);

	wchar_t buffer[50];
	wsprintf(buffer, L" 우주선 탑승 림 수: %d", m_iRimCount);
	// 문자열 출력 (유니코드)
	TextOutW(hDC, int(m_tRect.left), int(m_tRect.top + 10), buffer, lstrlenW(buffer));

	RECT rect = { m_tRect.left, m_tRect.top + 30, m_tRect.right, m_tRect.bottom }; // 텍스트를 출력할 영역

	DrawText(
		hDC,
		L" \n"
		L" 수많은 고난과 싸움 끝에,\n"
		L" 당신의 식민지는 성공적으로\n"
		L" 우주선을 발사했습니다.\n"
		L" 생존자들은 이 행성의\n"
		L" 적대적인 환경을 뒤로하고,\n"
		L" 별들 속 새로운 삶을\n"
		L" 향해 나아갑니다.\n",
		-1,
		&rect,
		DT_NOCLIP
	);

	// Step 4: Restore the old font
	SelectObject(hDC, hOldFont);

	// Optional: Step 5: Delete the created font when done
	DeleteObject(hFont);
}
