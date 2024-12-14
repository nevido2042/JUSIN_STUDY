#include "pch.h"
#include "Pawn.h"
#include "PathFinder.h"
#include "KeyMgr.h"
#include "BmpMgr.h"
#include "ColonyMgr.h"
#include "ScrollMgr.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "Projectile.h"
#include "RangedWeapon.h"

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
    //경로 따라가는 중이면 실행 안함
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

    //타겟 노드와 충분히 가까워지면
    if (pTargetNode && CNode::Distance(pTargetNode->Get_Pos(), POS{ m_tInfo.fX, m_tInfo.fY }) < TILECX * 0.1f)
    {
        //도착으로 간주하고 타겟 노드로 위치이동
        m_tInfo.fX = pTargetNode->Get_Pos().fX;
        m_tInfo.fY = pTargetNode->Get_Pos().fY;

        //도착한 노드를 삭제
        Safe_Delete<CNode*>(pTargetNode);
        m_NodeList.pop_front();

        //여기서 타겟과의 거리를 체크해서 더 움직일지 고민할까?
        //타겟이 충분히 가깝다면
        //나머지노드들을 세이프딜리트하고
        //네비게이팅 종료
        if (IsWithinRange())
        {
            //나머지노드들을 세이프딜리트하고
            for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
            m_NodeList.clear();
            //네비게이팅 종료
            m_bNavigating = false;
            return;
        }
        


        //노드가 비어있지 않다면
        if (!m_NodeList.empty())
        {
            //다음 노드를 타겟 노드로 설정
            pTargetNode = m_NodeList.front();
        }
        else
        {
            //아니면 타겟노드 없음
            pTargetNode = nullptr;
        }
    }

    //타겟 노드 없으면
    if (!pTargetNode)
    {
        //네비게이팅 종료
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

    m_fAngle = fRadian * (180.f / PI);

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

bool CPawn::IsWithinRange()
{
    //타겟이 없어도 리턴
    if (!m_pTarget)
    {
        return false;
    }

    //원거리 무기가 없으면 가깝지 않다고 판단.
    if (!m_pRangedWeapon)
    {
        return false;
    }

    CRangedWeapon* pRangedWeapon = static_cast<CRangedWeapon*>(m_pRangedWeapon);
    float fRange = pRangedWeapon->Get_Range();

    //타겟의 거리와 fRange비교
    float   fWidth(0.f), fHeight(0.f), fDiagonal(0.f);
    fWidth = m_pTarget->Get_Info().fX - m_tInfo.fX;
    fHeight = m_pTarget->Get_Info().fY - m_tInfo.fY;
    //타겟과의 거리
    fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);
   
    if (fDiagonal < fRange)
    {
        //발사 시도
        //pRangedWeapon->Fire();
        //가까우면 true
        return true;
    }

    return false;
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

void CPawn::OnCollision(OBJID _eID, CObj* _pOther)
{
    if (_eID == OBJ_PROJECTILE)
    {
        CProjectile* pProjectile = static_cast<CProjectile*>(_pOther);
        Take_Damage(pProjectile->Get_Damage());
        pProjectile->Set_Destroyed();
    }
}
