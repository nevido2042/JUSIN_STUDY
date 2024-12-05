#include "pch.h"
#include "BoxMgr.h"
#include "CScrollMgr.h"
#include "CKeyMgr.h"

CBoxMgr* CBoxMgr::m_pInstance = nullptr;

CBoxMgr::CBoxMgr()
	:m_fBoxSize(0)
{
	ZeroMemory(&m_tBoxPoint, sizeof(m_tBoxPoint));
}

CBoxMgr::~CBoxMgr()
{
	Release();
}

void CBoxMgr::Initialize()
{
	//CBox* pBox = new CBox(LINEPOINT{ 100.f,100.f }, 50.f);
	//m_Boxlist.push_back(pBox);

	m_fBoxSize = 50.f;
}

int CBoxMgr::Update()
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	ptMouse.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();

	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		if ((!m_tBoxPoint[HEAD].fX) && (!m_tBoxPoint[HEAD].fY))
		{
			m_tBoxPoint[HEAD].fX = (float)ptMouse.x;
			m_tBoxPoint[HEAD].fY = (float)ptMouse.y;
		}
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		m_tBoxPoint[TAIL].fX = (float)ptMouse.x;
		m_tBoxPoint[TAIL].fY = m_tBoxPoint[HEAD].fY;

	}

	if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
	{
		//HEAD와TAIL의 거리 / 박스 크기 = 박스 갯수
		float fDist = abs(m_tBoxPoint[HEAD].fX - m_tBoxPoint[TAIL].fX);
		int iBoxCount = int(fDist / m_fBoxSize);
		//HEAD부터 TAIL까지 박스들 생성
		for (int i = 0; i < iBoxCount; ++i)
		{
			CBox* pBox = new CBox(
				LINEPOINT
				{
					m_tBoxPoint[HEAD].fX + m_fBoxSize * 0.5f + i * m_fBoxSize,
					m_tBoxPoint[HEAD].fY
				}
			, m_fBoxSize);

			m_Boxlist.push_back(pBox);

			/*Rectangle(hDC,
				m_tBoxPoint[HEAD].fX + i * (int)m_fBoxSize,
				m_tBoxPoint[HEAD].fY - m_fBoxSize * 0.5f,
				m_tBoxPoint[HEAD].fX + (int)m_fBoxSize + i * (int)m_fBoxSize,
				m_tBoxPoint[HEAD].fY + (int)m_fBoxSize - m_fBoxSize * 0.5f);*/
		}

		ZeroMemory(&m_tBoxPoint, sizeof(m_tBoxPoint));
	}

	if (CKeyMgr::Get_Instance()->Key_Down('S'))
	{
		Save_Box();
		return 0;
	}

	if (CKeyMgr::Get_Instance()->Key_Down('L'))
	{
		Load_Box();
		return 0;
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		CScrollMgr::Get_Instance()->Set_ScrollX(5.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		CScrollMgr::Get_Instance()->Set_ScrollX(-5.f);

	return 0;
}

void CBoxMgr::Late_Update()
{
}

void CBoxMgr::Render(HDC hDC)
{
	for (auto& pLine : m_Boxlist)
		pLine->Render(hDC);

	MoveToEx(hDC, (int)m_tBoxPoint[HEAD].fX, (int)m_tBoxPoint[HEAD].fY, nullptr);
	LineTo(hDC, (int)m_tBoxPoint[TAIL].fX, (int)m_tBoxPoint[TAIL].fY);

	//HEAD와TAIL의 거리 / 박스 크기 = 박스 갯수
	float fDist = abs(m_tBoxPoint[HEAD].fX - m_tBoxPoint[TAIL].fX);
	int iBoxCount = int(fDist / m_fBoxSize);
	//HEAD부터 TAIL까지 박스들 출력
	for (int i = 0; i < iBoxCount ; ++i)
	{
		Rectangle(hDC,
			m_tBoxPoint[HEAD].fX + i * (int)m_fBoxSize,
			m_tBoxPoint[HEAD].fY - m_fBoxSize * 0.5f,
			m_tBoxPoint[HEAD].fX + (int)m_fBoxSize + i * (int)m_fBoxSize,
			m_tBoxPoint[HEAD].fY + (int)m_fBoxSize - m_fBoxSize * 0.5f);
	}
}

void CBoxMgr::Release()
{
	for_each(m_Boxlist.begin(), m_Boxlist.end(), Safe_Delete<CBox*>);
	m_Boxlist.clear();
}

void CBoxMgr::Save_Box()
{
}

void CBoxMgr::Load_Box()
{
}
