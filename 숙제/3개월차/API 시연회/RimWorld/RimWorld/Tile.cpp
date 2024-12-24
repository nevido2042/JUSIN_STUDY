#include "pch.h"
#include "Tile.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ColonyMgr.h"
#include "KeyMgr.h"

CTile::CTile() : m_eOption(OPT_END), m_pObj(nullptr)
{
}

CTile::~CTile()
{
	Release();
}

void CTile::Initialize()
{
	m_tInfo.fCX = TILECX;
	m_tInfo.fCY = TILECY;

	m_pImgKey = L"Ice";

	m_eOption = OPT_REACHABLE;
}

int CTile::Update()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CTile::Late_Update()
{
	if (Is_MouseHovered_Scrolled() && 
		CColonyMgr::Get_Instance()->Get_Mode() == CColonyMgr::MODE_CONSTRUCT)//건설 모드이고
	{
		
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
		{
			//오브젝트가 있다.
			if (m_pObj)
			{
				//벽이면 리턴
				if (!lstrcmp(m_pObj->Get_ImgKey(), L"Wall_Atlas_Smooth"))
				{
					return;
				}
			}

			TASK tTask;
			tTask.pObj = this;
			CColonyMgr::Get_Instance()->Emplace_ConstructSet(tTask);//건설 목록에 추가
		}
	}
}

void CTile::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	BitBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		TILECX,
		TILECY,
		hMemDC,
		TILECX * m_iDrawID,
		0,
		SRCCOPY);

	HPEN hPen(nullptr);
	HPEN hOldPen(nullptr);

	if (m_eOption == OPT_BLOCKED)
	{
		// 빨간색 펜 생성
		hPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0)); // 굵기 1, 빨간색
		hOldPen = (HPEN)SelectObject(hDC, hPen);       // 기존 펜 저장 및 빨간색 펜 설정
	}
	
	// 사각형 그리기
	MoveToEx(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, nullptr);         // 왼쪽 위로 이동
	LineTo(hDC, m_tRect.right+ iScrollX, m_tRect.top + iScrollY);                  // 상단 선
	LineTo(hDC, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);               // 오른쪽 선
	LineTo(hDC, m_tRect.left + iScrollX, m_tRect.bottom + iScrollY);                // 하단 선
	LineTo(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY);                   // 왼쪽 선

	if (m_eOption == OPT_BLOCKED)
	{
		// 펜 정리
		SelectObject(hDC, hOldPen); // 기존 펜 복원
		DeleteObject(hPen);         // 빨간색 펜 삭제
	}

}

void CTile::Release()
{
}
