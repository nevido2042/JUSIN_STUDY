#include "pch.h"
#include "Ship.h"
#include "ObjMgr.h"
#include "Pawn.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CShip::CShip()
{
}

CShip::~CShip()
{
	Release();
}

void CShip::Initialize()
{
	Set_ImgKey(L"ShipEngine_north");

	m_tInfo.fCX = 128.f;
	m_tInfo.fCY = 128.f;

	m_eRenderID = RENDER_GAMEOBJECT;

	//작업 변경사항을 림에게 알린다.
	CColonyMgr::Get_Instance()->Notify_TaskChange();

	////적들에게 길이 바뀌었을 수도 있음을 알리며, 네비게이션 종료
	//for (CObj* pObj : CObjMgr::Get_Instance()->Get_List()[OBJ_ENEMY])
	//{
	//	//static_cast<CPawn*>(pObj)->RequestNavStop();
	//	static_cast<CPawn*>(pObj)->Set_Navigating(false);//아닌거 같기도하고
	//	static_cast<CPawn*>(pObj)->Set_Navigating(true);
	//}

	//for (CObj* pObj : CObjMgr::Get_Instance()->Get_List()[OBJ_RIM])
	//{
	//	static_cast<CPawn*>(pObj)->Set_Navigating(false);//아닌거 같기도하고
	//	static_cast<CPawn*>(pObj)->Set_Navigating(true);
	//}
}

int CShip::Update()
{
	if (m_bDestroyed)
	{
		return OBJ_DESTROYED;
	}

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CShip::Late_Update()
{

}

void CShip::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC,
		(int)(m_tRect.left + fScrollX),
		(int)(m_tRect.top + fScrollY),
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		0, 0, 512, 512,
		RGB_WHITE);
}

void CShip::Release()
{
}
