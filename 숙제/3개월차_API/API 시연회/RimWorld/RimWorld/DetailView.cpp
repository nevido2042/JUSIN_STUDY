#include "pch.h"
#include "DetailView.h"
#include "BmpMgr.h"
#include "ColonyMgr.h"
#include "Pawn.h"
#include "SoundMgr.h"

CDetailView::CDetailView()
{
}

CDetailView::~CDetailView()
{
	Release();
}

void CDetailView::Initialize()
{
	Set_Size(400.f, 150.f);

	m_eRenderID = RENDER_UI;

	m_bActivate = false;

	Set_ImgKey(L"DetailViewBG");
}

int CDetailView::Update()
{
	return CObj::Update();
}

void CDetailView::Late_Update()
{
}

void CDetailView::Render(HDC hDC)
{
	if (!m_bActivate)
	{
		return;
	}

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(Get_ImgKey());

	BitBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		0, 0,
		SRCCOPY);

	CObj* pObj = CColonyMgr::Get_Instance()->Get_Target();

	if (pObj)
	{
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

		RECT rect = { m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom }; // 텍스트를 출력할 영역

		CPawn::STATE eState = static_cast<CPawn*>(pObj)->Get_State();

		switch (eState)
		{
		case CPawn::WANDERING:
			DrawText(hDC, L"\n   상태: 떠도는 중", -1, &rect, DT_NOCLIP);
			break;
		case CPawn::DRAFTED:
			DrawText(hDC, L"\n   상태: 소집 됨", -1, &rect, DT_NOCLIP);
			break;
		case CPawn::UNDRAFTED:
			DrawText(hDC, L"\n   상태: 소집 안 됨", -1, &rect, DT_NOCLIP);
			break;
		case CPawn::CHASING:
			DrawText(hDC, L"\n   상태: 따라가는 중", -1, &rect, DT_NOCLIP);
			break;
		case CPawn::DECONSTRUCTING:
			DrawText(hDC, L"\n   상태: 해체 중", -1, &rect, DT_NOCLIP);
			break;
		case CPawn::CONSTRUCTING:
			DrawText(hDC, L"\n   상태: 건설 중", -1, &rect, DT_NOCLIP);
			break;
		case CPawn::TRANSPORTING:
			DrawText(hDC, L"\n   상태: 운반 중", -1, &rect, DT_NOCLIP);
			break;
		case CPawn::BOARDING:
			DrawText(hDC, L"\n   상태: 탑승 중", -1, &rect, DT_NOCLIP);
			break;
		case CPawn::LOGGING:
			DrawText(hDC, L"\n   상태: 벌목 중", -1, &rect, DT_NOCLIP);
			break;
		case CPawn::MOVETOWORK:
			DrawText(hDC, L"\n   상태: 작업 중", -1, &rect, DT_NOCLIP);
			break;
		case CPawn::END:
			break;
		default:
			break;
		}

		// Step 4: Restore the old font
		SelectObject(hDC, hOldFont);

		// Optional: Step 5: Delete the created font when done
		DeleteObject(hFont);
	}

}

void CDetailView::Release()
{
}

void CDetailView::Set_Activate(bool _bActivate)
{
	if (m_bActivate == _bActivate)
	{
		if (m_bActivate == false)
		{
			return;
		}
	}

	m_bActivate = _bActivate;

	if (m_bActivate)
	{
		CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
		CSoundMgr::Get_Instance()->PlaySound(L"TabOpen.wav", SOUND_UI, .2f);
	}
	else
	{
		CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
		CSoundMgr::Get_Instance()->PlaySound(L"TabClose.wav", SOUND_UI, .2f);
	}
}
