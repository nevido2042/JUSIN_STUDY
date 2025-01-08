#include "pch.h"
#include "Ship.h"
#include "ObjMgr.h"
#include "Pawn.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ColonyMgr.h"
#include "KeyMgr.h"
#include "TileMgr.h"

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

	//�۾� ��������� ������ �˸���.
	CColonyMgr::Get_Instance()->Notify_TaskChange();

	////���鿡�� ���� �ٲ���� ���� ������ �˸���, �׺���̼� ����
	//for (CObj* pObj : CObjMgr::Get_Instance()->Get_List()[OBJ_ENEMY])
	//{
	//	//static_cast<CPawn*>(pObj)->RequestNavStop();
	//	static_cast<CPawn*>(pObj)->Set_Navigating(false);//�ƴѰ� ���⵵�ϰ�
	//	static_cast<CPawn*>(pObj)->Set_Navigating(true);
	//}

	//for (CObj* pObj : CObjMgr::Get_Instance()->Get_List()[OBJ_RIM])
	//{
	//	static_cast<CPawn*>(pObj)->Set_Navigating(false);//�ƴѰ� ���⵵�ϰ�
	//	static_cast<CPawn*>(pObj)->Set_Navigating(true);
	//}
}

int CShip::Update()
{
	return CObj::Update();
}

void CShip::Late_Update()
{
	if (Is_MouseHovered_Scrolled() && 
		CColonyMgr::Get_Instance()->Get_Target()&&
		CKeyMgr::Get_Instance()->Key_Up(VK_RBUTTON)
	)
	{
		//Ŭ���ϸ� ���ּ����� �̵�
		CObj* pTarget = CColonyMgr::Get_Instance()->Get_Target();
		pTarget->Set_Target(this);

		if (CPawn* pPawn = dynamic_cast<CPawn*>(pTarget))
		{
			pPawn->Change_State(CPawn::BOARDING, this);
		}
	}
}

void CShip::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	POINT tPoint = CCamera::Get_Instance()->WorldToScreen((float)m_tRect.left, (float)m_tRect.top);
	float fZoom = CCamera::Get_Instance()->Get_Zoom();

	GdiTransparentBlt(hDC,
		(int)(tPoint.x + fScrollX),
		(int)(tPoint.y + fScrollY),
		(int)(m_tInfo.fCX * fZoom),
		(int)(m_tInfo.fCY * fZoom),
		hMemDC,
		0, 0, 512, 512,
		RGB_WHITE);
}

void CShip::Release()
{
}
