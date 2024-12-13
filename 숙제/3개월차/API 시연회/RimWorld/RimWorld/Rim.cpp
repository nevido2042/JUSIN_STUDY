#include "pch.h"
#include "Rim.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "KeyMgr.h"
#include "ColonyMgr.h"
#include "PathFinder.h"

CRim::CRim()
    :m_bNavigating(false)
{
}

CRim::~CRim()
{
    Release();
}

void CRim::Move_To(POS _Pos)
{
    for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
    m_NodeList.clear();

    //이동 할 타일의 idx를 계산해서 확인한다. Blocked이면 return;

    //갈 수 있으면 길찾기를 수행한다.(Astar/JPS)

    m_NodeList = move(CPathFinder::Get_Instance()->Find_Path(POS{ m_tInfo.fX, m_tInfo.fY }, _Pos));

    if (!m_NodeList.empty())
    {
        m_bNavigating = true;
    }
}

void CRim::Navigate()
{
    //리스트에서 점하나를 뽑아서
    //해당 점으로 가까워질 때까지 접근(두점 사이 각도 구해서 이동)
    //거의 가까워지면 노드 해제하고
    //다음 노드를 뽑아서 추적
    //다음 노드가 없으면 종료

    CNode* pTargetNode = nullptr;

    if (!m_NodeList.empty())
    {
        pTargetNode = m_NodeList.front();
    }

    if (pTargetNode && CNode::Distance(pTargetNode->Get_Pos(), POS{ m_tInfo.fX, m_tInfo.fY }) < TILECX * 0.1f)
    {
        m_tInfo.fX = pTargetNode->Get_Pos().fX;
        m_tInfo.fY = pTargetNode->Get_Pos().fY;

        Safe_Delete<CNode*>(pTargetNode);
        m_NodeList.pop_front();

        if (!m_NodeList.empty())
        {
            pTargetNode = m_NodeList.front();
        }
        else
        {
            pTargetNode = nullptr;
        }
    }

    if (!pTargetNode)
    {
        return;
    }


    float   fWidth(0.f), fHeight(0.f), fDiagonal(0.f), fRadian(0.f);

    fWidth = pTargetNode->Get_Pos().fX - m_tInfo.fX;
    fHeight = pTargetNode->Get_Pos().fY - m_tInfo.fY;

    fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

    fRadian = acosf(fWidth / fDiagonal);

    if (pTargetNode->Get_Pos().fY > m_tInfo.fY)
        fRadian = (2.f * PI) - fRadian;

    //m_fAngle = fRadian * (180.f / PI);

    m_tInfo.fX += m_fSpeed * cosf(fRadian);
    m_tInfo.fY -= m_fSpeed * sinf(fRadian);
}

void CRim::Initialize()
{
    m_tInfo.fCX = 64.f;
    m_tInfo.fCY = 64.f;

    m_fSpeed = 10.f;

    m_eRenderID = RENDER_GAMEOBJECT;
}

int CRim::Update()
{
    if (m_bDead)
        return OBJ_DEAD;

    if (m_bNavigating)
    {
        Navigate();
    }

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

    for (CNode* pNode : m_NodeList)
    {
        Ellipse(hDC, pNode->Get_Pos().fX + iScrollX, pNode->Get_Pos().fY + iScrollY,
            pNode->Get_Pos().fX + 10.f + iScrollX, pNode->Get_Pos().fY + 10.f + iScrollY);
    }


}

void CRim::Release()
{
    for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
    m_NodeList.clear();
}
