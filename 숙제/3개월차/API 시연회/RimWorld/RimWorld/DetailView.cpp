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
	if (m_bDestroyed)
		return OBJ_DESTROYED;

	__super::Update_Rect();

	return OBJ_NOEVENT;
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

	// ����� �����ϰ� ����
	SetBkMode(hDC, TRANSPARENT);

	// �ؽ�Ʈ ���� ������� ���� (RGB(255, 255, 255))
	SetTextColor(hDC, RGB(255, 255, 255));

	if (pObj)
	{
		RECT rect = { m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom }; // �ؽ�Ʈ�� ����� ����

		CPawn::STATE eState = static_cast<CPawn*>(pObj)->Get_State();

		switch (eState)
		{
		case CPawn::WANDERING:
			DrawText(hDC, L"\n   ����: ������ ��", -1, &rect, DT_NOCLIP);
			break;
		case CPawn::DRAFTED:
			DrawText(hDC, L"\n   ����: ���� ��", -1, &rect, DT_NOCLIP);
			break;
		case CPawn::UNDRAFTED:
			break;
		case CPawn::CHASING:
			break;
		case CPawn::DECONSTRUCTING:
			DrawText(hDC, L"\n   ����: ��ü ��", -1, &rect, DT_NOCLIP);
			break;
		case CPawn::CONSTRUCTING:
			DrawText(hDC, L"\n   ����: �Ǽ� ��", -1, &rect, DT_NOCLIP);
			break;
		case CPawn::END:
			break;
		default:
			break;
		}
	}

}

void CDetailView::Release()
{
}

void CDetailView::Set_Activate(bool _bActivate)
{
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
