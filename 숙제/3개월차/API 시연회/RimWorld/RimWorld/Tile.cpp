#include "pch.h"
#include "Tile.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ColonyMgr.h"
#include "KeyMgr.h"
#include "Camera.h"

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
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		if (Is_MouseHovered_Scrolled()) //왜 안되는거야 창사이즈보다 멀리있는 곳은
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
}

void CTile::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	// 줌을 적용한 타일 크기 계산
	int zoomedTileCX = int(TILECX * CCamera::Get_Instance()->Get_Zoom());
	int zoomedTileCY = int(TILECY * CCamera::Get_Instance()->Get_Zoom());

	POINT tPoint = CCamera::Get_Instance()->WorldToScreen(m_tRect.left, m_tRect.top);
	// 타일 렌더링 (StretchBlt 사용)
	StretchBlt(
		hDC,
		tPoint.x,
		tPoint.y,
		zoomedTileCX,
		zoomedTileCY,
		hMemDC,
		0,
		0,
		TILECX,
		TILECY,
		SRCCOPY // 단순 복사
	);

	//BitBlt(
	//	hDC,
	//	tPoint.x,        // 타일의 화면 좌표 X
	//	tPoint.y,        // 타일의 화면 좌표 Y
	//	TILECX,          // 원본 타일의 너비 (크기 고정)
	//	TILECY,          // 원본 타일의 높이 (크기 고정)
	//	hMemDC,
	//	0,
	//	0,
	//	SRCCOPY          // 단순 복사
	//);

	//HPEN hPen(nullptr);
	//HPEN hOldPen(nullptr);

	//if (m_eOption == OPT_BLOCKED)
	//{
	//	// 빨간색 펜 생성
	//	hPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0)); // 굵기 1, 빨간색
	//	hOldPen = (HPEN)SelectObject(hDC, hPen);       // 기존 펜 저장 및 빨간색 펜 설정
	//}
	//
	//// 사각형 그리기
	////MoveToEx(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, nullptr);         // 왼쪽 위로 이동
	////LineTo(hDC, m_tRect.right+ iScrollX, m_tRect.top + iScrollY);                  // 상단 선
	////LineTo(hDC, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);               // 오른쪽 선
	////LineTo(hDC, m_tRect.left + iScrollX, m_tRect.bottom + iScrollY);                // 하단 선
	////LineTo(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY);                   // 왼쪽 선

	//// 사각형 그리기
	//if (m_eOption == OPT_BLOCKED)
	//{
	//	// 월드 좌표를 화면 좌표로 변환
	//	POINT topLeft = CCamera::Get_Instance()->WorldToScreen(m_tRect.left, m_tRect.top);
	//	POINT bottomRight = CCamera::Get_Instance()->WorldToScreen(m_tRect.right, m_tRect.bottom);

	//	// 줌 적용
	//	int zoomedLeft = topLeft.x;
	//	int zoomedTop = topLeft.y;
	//	int zoomedRight = bottomRight.x;
	//	int zoomedBottom = bottomRight.y;

	//	// 사각형 그리기
	//	MoveToEx(hDC, zoomedLeft, zoomedTop, nullptr);             // 왼쪽 위로 이동
	//	LineTo(hDC, zoomedRight, zoomedTop);                       // 상단 선
	//	LineTo(hDC, zoomedRight, zoomedBottom);                    // 오른쪽 선
	//	LineTo(hDC, zoomedLeft, zoomedBottom);                     // 하단 선
	//	LineTo(hDC, zoomedLeft, zoomedTop);                        // 왼쪽 선
	//}


	//if (m_eOption == OPT_BLOCKED)
	//{
	//	// 펜 정리
	//	SelectObject(hDC, hOldPen); // 기존 펜 복원
	//	DeleteObject(hPen);         // 빨간색 펜 삭제
	//}

}

void CTile::Release()
{
}
