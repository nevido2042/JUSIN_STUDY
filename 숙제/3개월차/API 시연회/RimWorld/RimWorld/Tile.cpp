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

	m_eObjID = OBJ_TILE;
}

int CTile::Update()
{
	return CObj::Update();
}

void CTile::Late_Update()
{
	if (Is_MouseHovered_Scrolled()&& CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		//오브젝트가 있다.
		if (m_pObj)
		{
			//나무나 벽이면 리턴
			if (m_pObj->Get_ObjID() == OBJ_WALL ||
				m_pObj->Get_ObjID() == OBJ_TREE)
			{
				return;
			}
		}

		if (CColonyMgr::Get_Instance()->Get_Mode() == CColonyMgr::MODE_CONSTRUCT)//건설 모드
		{
			TASK tTask;
			tTask.eType = TASK::WALL;
			tTask.pObj = this;
			CColonyMgr::Get_Instance()->Emplace_ConstructSet(tTask);//건설 목록에 추가

		}
		else if (CColonyMgr::Get_Instance()->Get_Mode() == CColonyMgr::MODE_SHIP)
		{
			TASK tTask;
			tTask.pObj = this;
			tTask.eType = TASK::SHIP;
			CColonyMgr::Get_Instance()->Emplace_ConstructSet(tTask);//건설 목록에 추가
		}
		else if (CColonyMgr::Get_Instance()->Get_Mode() == CColonyMgr::MODE_CAMPFIRE)
		{
			TASK tTask;
			tTask.pObj = this;
			tTask.eType = TASK::CAMPFIRE;
			CColonyMgr::Get_Instance()->Emplace_ConstructSet(tTask);//건설 목록에 추가
		}
	}
}

void CTile::Render(HDC hDC)
{
	//HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	/*BitBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		TILECX,
		TILECY,
		hMemDC,
		TILECX * m_iDrawID,
		0,
		SRCCOPY);*/

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
