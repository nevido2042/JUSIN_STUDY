#include "pch.h"
#include "Pawn.h"
#include "PathFinder.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "RangedWeapon.h"
#include "TimeMgr.h"
#include "TileMgr.h"
#include "SteelWall.h"

CPawn::CPawn()
    :m_bNavigating(false), m_fHP(0.f), m_fMaxHP(0.f), m_bDead(false), m_eState(END),
    m_pRangedWeapon(nullptr), m_fTargetDist(0.f), m_fTargetAngle(0.f), m_bAttack(false), m_bNavStopRequested(false)
{
    ZeroMemory(&m_tPrevPos, sizeof(POS));
}

CPawn::~CPawn()
{
    Release();
}

void CPawn::Move_To(POS _Pos)
{
    ////멈춰서 공격 중일 때 못찾게해야함!!!!!!!!!!!
    //if (m_bAttack)
    //{
    //    return;
    //}

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
    //타겟이 Wall이면 길을 못찾는다!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //타겟의 근처 타일을 찾아서 가게끔 해야할듯


    if (!m_NodeList.empty())
    {
        m_bNavigating = true;
    }
    //길을 못찾았을 경우
    else if (m_NodeList.empty() && Get_Target())
    {
        Change_State(DECONSTRUCTING);
        //타겟과 자신사이에서 장애물을 찾고
        //타겟을 그 장애물로 한다.
        Set_Target(Get_ObstacleToTarget());
    }
}

void CPawn::Take_Damage(float _fDamage)
{
    if (m_bDead)
    {
        return;
    }

    m_fHP -= _fDamage;

    //죽음 처리
    if (m_fHP <= 0.f)
    {
        m_bDead = true;
        Dead();
    }
}

void CPawn::Change_State(STATE _eState)
{
    m_eState = _eState;
    //m_pTarget = nullptr;
}

void CPawn::Dead()
{
    m_fSpeed = 0.f;
    m_bAttack = false;
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

        //새로운 길을 제시하면 네비게이팅으 종료 예약 걸자
        //여기서 타겟과의 거리를 체크해서 더 움직일지 고민할까?
        //타겟이 충분히 가깝다면
        //나머지노드들을 세이프딜리트하고
        //네비게이팅 종료
        if (m_bAttack || m_bNavStopRequested)//공격 중이거나 길찾기 종료 요청이 있으면
        {
            //나머지노드들을 세이프딜리트하고
            for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
            m_NodeList.clear();
            //네비게이팅 종료
            m_bNavigating = false;
            m_bNavStopRequested = false;
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

    m_tInfo.fX += m_fSpeed * cosf(fRadian) * GAMESPEED;
    m_tInfo.fY -= m_fSpeed * sinf(fRadian) * GAMESPEED;
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

    //공격 중이고, 타겟이 있을 때만 타겟 바라보기
    if (m_bAttack && m_pTarget)
    {
        //x축이 거의 같을 때만 y축 비교
        if (fabs(m_pTarget->Get_Info().fX - m_tInfo.fX) < FLT_EPSILON)
        {
            if (m_pTarget->Get_Info().fY > m_tInfo.fY)
            {
                m_eDir = DD;
            }
            else
            {
                m_eDir = UU;
            }
        }
        else if (m_pTarget->Get_Info().fX > m_tInfo.fX)
        {
            m_eDir = RR;
        }
        else
        {
            m_eDir = LL;
        }
    }
}

void CPawn::Measure_Target()
{
    //타겟이 없으면 리턴
    if (!m_pTarget)
    {
        return;
    }

    //타겟의 거리와 fRange비교
    float   fWidth(0.f), fHeight(0.f), fRadian(0.f);
    fWidth = m_pTarget->Get_Info().fX - m_tInfo.fX;
    fHeight = m_pTarget->Get_Info().fY - m_tInfo.fY;
    //타겟과의 거리
    m_fTargetDist = sqrtf(fWidth * fWidth + fHeight * fHeight);

    fRadian = acosf(fWidth / m_fTargetDist);

    if (m_pTarget->Get_Info().fY > m_tInfo.fY)
        fRadian = (2.f * PI) - fRadian;

    m_fTargetAngle = fRadian * (180.f / PI);
}

bool CPawn::IsWithinRange()
{
    //본인이 죽었으면 리턴
    if (m_bDead)
    {
        return false;
    }

    //타겟이 없어도 리턴
    if (!m_pTarget)
    {
        return false;
    }

    //원거리 무기가 없으면
    if (!m_pRangedWeapon)
    {
        //타일 1칸 거리로 판단.

        if (m_fTargetDist < TILECX)
        {
            return true;
        }


        return false;
    }

    CRangedWeapon* pRangedWeapon = static_cast<CRangedWeapon*>(m_pRangedWeapon);
    float fRange = pRangedWeapon->Get_Range();
   
    //거리가 가깝거나
    if (m_fTargetDist < fRange)
    {
        return true;
    }

    return false;
}

bool CPawn::IsCanSeeTarget()
{
    int iX1 = int(m_tInfo.fX / TILECX);
    int iY1 = int(m_tInfo.fY / TILECY);
    int iX2 = int(m_pTarget->Get_Info().fX / TILECX);
    int iY2 = int(m_pTarget->Get_Info().fY / TILECY);

    bool bCanSee = true;
    Bresenham(iX1, iY1, iX2, iY2, [&](int x, int y) {
        if (CTileMgr::Get_Instance()->Get_TileOption(x, y) == OPT_BLOCKED)
        {
            bCanSee = false;
            return false;  // 장애물이 있으면 탐색 종료
        }
        return true;  // 계속 탐색
        });

    return bCanSee;
}


CObj* CPawn::Get_ObstacleToTarget()
{
    int iX1 = int(m_tInfo.fX / TILECX);
    int iY1 = int(m_tInfo.fY / TILECY);
    int iX2 = int(m_pTarget->Get_Info().fX / TILECX);
    int iY2 = int(m_pTarget->Get_Info().fY / TILECY);

    CObj* pObstacle = nullptr;
    Bresenham(iX1, iY1, iX2, iY2, [&](int x, int y) {
        if (CTileMgr::Get_Instance()->Get_TileOption(x, y) == OPT_BLOCKED)
        {
            pObstacle = CTileMgr::Get_Instance()->Get_TileObj(x, y);
            return false;  // 장애물을 찾으면 탐색 종료
        }
        return true;  // 계속 탐색
        });

    return pObstacle;
}


int CPawn::Get_ReachableToTarget()
{
    int iX1 = int(m_tInfo.fX / TILECX);
    int iY1 = int(m_tInfo.fY / TILECY);
    int iX2 = int(m_pTarget->Get_Info().fX / TILECX);
    int iY2 = int(m_pTarget->Get_Info().fY / TILECY);

    int iClosestDist = INT_MAX;
    int iClosestIndex = -1;

    Bresenham(iX1, iY1, iX2, iY2, [&](int x, int y) {
        if (CTileMgr::Get_Instance()->Get_TileOption(x, y) == OPT_REACHABLE)
        {
            int iDist = abs(x - iX2) + abs(y - iY2);
            if (iDist < iClosestDist)
            {
                iClosestDist = iDist;
                iClosestIndex = x + y * TILEX;
            }
        }
        return true;  // 계속 탐색
        });

    return iClosestIndex;
}


int CPawn::Find_NearestReachableTile(int iIndexX, int iIndexY)
{
    // Directions: 상, 하, 좌, 우
    const int dx[4] = { 0, 0, -1, 1 };
    const int dy[4] = { -1 * TILEX, 1 * TILEX, 0, 0 };

    queue<int> q;
    bool visited[TILEY * TILEX] = { false };

    q.push(iIndexX + TILEX * iIndexY);
    visited[iIndexX + TILEX * iIndexY] = true;

    while (!q.empty()) {
        int iIndex = q.front();
        q.pop();

        int iX = iIndex % TILEX;
        int iY = iIndex / TILEX;

        // 접근 가능한 타일 찾음
        if (CTileMgr::Get_Instance()->Get_TileOption(iX, iY) == OPT_REACHABLE)
        {
            return iIndex;
        }

        // 상하좌우 탐색
        for (int i = 0; i < 4; ++i) {
            int nx = iX + dx[i];
            int ny = iY + dy[i];

            // 지도 범위 내에 있고 아직 방문하지 않았다면 큐에 추가
            if (nx >= 0 && nx < TILEX && ny >= 0 && ny < TILEY && !visited[nx + ny * TILEX]) 
            {
                visited[nx + ny * TILEX] = true;
                q.push(nx + ny * TILEX);
            }
        }
    }

    // 실패 시 -1 반환
    return -1;
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
    //죽었으면 리턴
    if (m_bDead)
    {
        return;
    }

    //이동방향 계산
    Calculate_MoveDir();

    if (m_pTarget)
    {
        //타겟과의 거리 및 각도 계산
        Measure_Target();
    }

    if (m_bNavigating)
    {
        Navigate();
    }

    switch (m_eState)
    {
    case CPawn::WANDERING:
        Handle_Wandering();
        break;
    case CPawn::DRAFTED:
        Handle_Drafted();
        break;
    case CPawn::UNDRAFTED:
        Handle_Undrafted();
        break;
    case CPawn::CHASING:
        Handle_Chasing();
        break;
    case CPawn::DECONSTRUCTING:
        Handle_Deconstructing();
        break;
    case CPawn::CONSTRUCTING:
        Handle_Constructing();
        break;
    case CPawn::END:
        break;
    default:
        break;
    }
}

void CPawn::Release()
{
    for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
    m_NodeList.clear();
}

void CPawn::OnCollision(OBJID _eID, CObj* _pOther)
{
    
}

void CPawn::Handle_Wandering()
{
}

void CPawn::Handle_Drafted()
{
}

void CPawn::Handle_Undrafted()
{
}

void CPawn::Handle_Chasing()
{
}

void CPawn::Handle_Deconstructing()
{
}

void CPawn::Handle_Constructing()
{
}
