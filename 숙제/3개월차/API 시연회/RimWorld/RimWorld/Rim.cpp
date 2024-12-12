#include "pch.h"
#include "Rim.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "KeyMgr.h"
#include "ColonyMgr.h"
#include "PathFinder.h"

CRim::CRim()
{
}

CRim::~CRim()
{
    Release();
}

void CRim::Move_To(POS _Pos)
{
    //�̵� �� Ÿ���� idx�� ����ؼ� Ȯ���Ѵ�. Blocked�̸� return;

    //�� �� ������ ��ã�⸦ �����Ѵ�.(Astar/JPS)

    //��ã�� �Ŵ����� ����
    //ex) CPathFinder::GetInstance()->Find_Path(Start, End); Ÿ�� ����Ʈ�� ��ȯ�ϰ� �ұ�?
    CPathFinder::Get_Instance()->Find_Path(POS{ m_tInfo.fX,m_tInfo.fY }, _Pos);
    
}

void CRim::Initialize()
{
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
    //���콺 Ŭ�� ���� �� Ÿ������ ����
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

    // �� ����, ��, �Ӹ� �� �׽�Ʈ
    HDC hTestDC;
    hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Naked_Male_south");

    // ����
    GdiTransparentBlt(hDC,
        m_tRect.left + iScrollX - IMAGE_OFFSET_X
        , m_tRect.top + iScrollY - IMG_OFFSET_Y,
        128, 128,
        hTestDC, 0, 0, 128, 128,  
        RGB_PURPLE);


    //��
    hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Male_Average_Normal_south");
    GdiTransparentBlt(hDC,
        m_tRect.left + iScrollX - IMAGE_OFFSET_X,
        m_tRect.top + iScrollY - HEAD_OFFSET - IMG_OFFSET_Y,
        128, 128,
        hTestDC, 0, 0, 128, 128,
        RGB_PURPLE);

    // �Ӹ�
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
