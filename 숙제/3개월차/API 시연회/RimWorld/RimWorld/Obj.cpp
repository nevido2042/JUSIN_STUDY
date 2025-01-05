#include "pch.h"
#include "Obj.h"
#include "ScrollMgr.h"
#include "TimeMgr.h"

CObj::CObj() : m_fSpeed(0.f), m_eDir(DIR_END), m_bDestroyed(false),
m_fAngle(0.f), m_fDistance(0.f), m_pTarget(nullptr), m_pImgKey(nullptr),
m_iDrawID(0), m_eRenderID(RENDER_END), m_bActivate(false), m_pParent(nullptr),
m_eObjID(OBJ_END)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CObj::~CObj()
{
	//Release();
}

float CObj::Calculate_Dist(CObj* _pDst, CObj* _pSrc)
{
	float fWidth(0.f), fHeight(0.f), fDist(0.f);

	fWidth = _pDst->Get_Info().fX - _pSrc->Get_Info().fX;
	fHeight = _pDst->Get_Info().fY - _pSrc->Get_Info().fY;

	fDist = sqrtf(fWidth * fWidth + fHeight * fHeight);

	return fDist;
}

bool CObj::Is_MouseHovered_Scrolled()
{
	//마우스 클릭 했을 때 타겟으로 설정
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	ptMouse.x -= iScrollX;
	ptMouse.y -= iScrollY;

	if (PtInRect(&m_tRect, ptMouse))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CObj::Is_MouseHovered()
{
	//마우스 클릭 했을 때 타겟으로 설정
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (PtInRect(&m_tRect, ptMouse))
	{
		return true;
	}
	else
	{
		return false;
	}
}

int CObj::Update()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;

	Update_Rect();

	return OBJ_NOEVENT;
}

void CObj::OnCollision(OBJID _eID, CObj* _pOther)
{
}

void CObj::Update_Rect()
{
	m_tRect.left	= LONG(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRect.top		= LONG(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
	m_tRect.right	= LONG(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRect.bottom	= LONG(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
}

void CObj::Move_Frame()
{
	m_tFrame.ullElapsedTime += (ULONGLONG)GAMESPEED;

	if (m_tFrame.ullElapsedTime > m_tFrame.ullTime)
	{
		++m_tFrame.iFrameStart;

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart = 0;

		m_tFrame.ullElapsedTime = 0;
	}
}
