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
		L"���� ���"                   // Font name
	);


	// Step 2: Select the font into the device context
	HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);

	wchar_t buffer[50];
	wsprintf(buffer, L" ���ּ� ž�� �� ��: %d", m_iRimCount);
	// ���ڿ� ��� (�����ڵ�)
	TextOutW(hDC, int(m_tRect.left), int(m_tRect.top + 10), buffer, lstrlenW(buffer));

	RECT rect = { m_tRect.left, m_tRect.top + 30, m_tRect.right, m_tRect.bottom }; // �ؽ�Ʈ�� ����� ����

	DrawText(
		hDC,
		L" \n"
		L" ������ ���� �ο� ����,\n"
		L" ����� �Ĺ����� ����������\n"
		L" ���ּ��� �߻��߽��ϴ�.\n"
		L" �����ڵ��� �� �༺��\n"
		L" �������� ȯ���� �ڷ��ϰ�,\n"
		L" ���� �� ���ο� ����\n"
		L" ���� ���ư��ϴ�.\n",
		-1,
		&rect,
		DT_NOCLIP
	);

	// Step 4: Restore the old font
	SelectObject(hDC, hOldFont);

	// Optional: Step 5: Delete the created font when done
	DeleteObject(hFont);
}
