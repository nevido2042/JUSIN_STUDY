#include "pch.h"
#include "Pawn.h"
#include "PathFinder.h"
#include "KeyMgr.h"
#include "BmpMgr.h"
#include "ColonyMgr.h"
#include "ScrollMgr.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"

CPawn::CPawn()
    :m_bNavigating(false), m_fHP(0.f), m_fMaxHP(0.f), m_bDead(false), m_pRangedWeapon(nullptr)
{
    ZeroMemory(&m_tPrevPos, sizeof(POS));
}

CPawn::~CPawn()
{
    Release();
}

void CPawn::Move_To(POS _Pos)
{
    if (m_bNavigating)
    {
        return;
    }

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

void CPawn::Take_Damage(float _fDamage)
{
    m_fHP -= _fDamage;

    //죽음 처리
    if (m_fHP <= 0.f)
    {
        m_bDead = true;
        Dead();
    }
}

void CPawn::Navigate()
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
        m_bNavigating = false;
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

void CPawn::Calculate_MoveDir()
{
    //왼쪽
    if (m_tInfo.fX < m_tPrevPos.fX)
    {
        m_eDir = LL;
    }
    //오른쪽
    else if (m_tInfo.fX > m_tPrevPos.fX)
    {
        m_eDir = RR;
    }
    //위쪽
    else if (m_tInfo.fY < m_tPrevPos.fY)
    {
        m_eDir = UU;
    }
    //아래쪽
    else if (m_tInfo.fY > m_tPrevPos.fY)
    {
        m_eDir = DD;
    }

    //이전 프레임 위치 저장
    m_tPrevPos.fX = m_tInfo.fX;
    m_tPrevPos.fY = m_tInfo.fY;
}

void CPawn::Initialize()
{
    //HP바 생성
    CObj* pObj = CAbstractFactory<CHealthBar>::Create();
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pObj);
    //HP바의 타겟을 이 Pawn으로 설정
    pObj->Set_Target(this);

    m_fMaxHP = 100.f;
    m_fHP = m_fMaxHP;
}

int CPawn::Update()
{
    if (m_bDestroyed)
        return OBJ_DESTROYED;

    if (m_bNavigating)
    {
        Navigate();
    }

    __super::Update_Rect();

    return OBJ_NOEVENT;
}

void CPawn::Late_Update()
{
    Calculate_MoveDir();

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


        if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
        {
            CColonyMgr::Get_Instance()->Set_Target(this);
            return;
        }
    }
}

void CPawn::Release()
{
    for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
    m_NodeList.clear();
}
