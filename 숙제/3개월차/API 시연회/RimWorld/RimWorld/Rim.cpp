#include "pch.h"
#include "Rim.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "KeyMgr.h"
#include "ColonyMgr.h"

CRim::CRim()
{
}

CRim::~CRim()
{
    Release();
}

void CRim::Move_To(float _fX, float _fY)
{
    //길찾기
}

void CRim::Initialize()
{
    //m_tInfo.fCX = 128.f;
    //m_tInfo.fCY = 152.f;
    m_tInfo.fCX = 64.f;
    m_tInfo.fCY = 64.f;

    m_eRenderID = RENDER_GAMEOBJECT;
}

int CRim::Update()
{
    if (m_bDead)
        return OBJ_DEAD;

    __super::Update_Rect();

    return OBJ_NOEVENT;
}

void CRim::Late_Update()
{
    //마우스 클릭 했을 때 타겟으로 설정
    POINT	ptMouse{};

    GetCursorPos(&ptMouse);
    ScreenToClient(g_hWnd, &ptMouse);

    if (PtInRect(&m_tRect, ptMouse))
    {
        if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
        {
            CColonyMgr::Get_Instance()->Set_Target(this);
            return;
        }
    }
}

void CRim::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    Ellipse(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, 
        m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);

    // 림 몸통, 얼굴, 머리 순 테스트
    HDC hTestDC;
    hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Naked_Male_south");

    // 몸통
    GdiTransparentBlt(hDC,
        m_tRect.left + iScrollX - IMAGE_OFFSET_X
        , m_tRect.top + iScrollY - IMG_OFFSET_Y,
        128, 128,
        hTestDC, 0, 0, 128, 128,  
        RGB_PURPLE);


    //얼굴
    hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Male_Average_Normal_south");
    GdiTransparentBlt(hDC,
        m_tRect.left + iScrollX - IMAGE_OFFSET_X,
        m_tRect.top + iScrollY - HEAD_OFFSET - IMG_OFFSET_Y,
        128, 128,
        hTestDC, 0, 0, 128, 128,
        RGB_PURPLE);

    // 머리
    hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Afro_south");
    GdiTransparentBlt(hDC,
        m_tRect.left + iScrollX - IMAGE_OFFSET_X,
        m_tRect.top + iScrollY - HEAD_OFFSET - IMG_OFFSET_Y,
        128, 128,
        hTestDC, 0, 0, 128, 128,
        RGB_PURPLE);
}

void CRim::Release()
{
}
