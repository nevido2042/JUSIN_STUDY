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

CPawn::CPawn()
    :m_bNavigating(false), m_fHP(0.f), m_fMaxHP(0.f), m_bDead(false),
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
    //멈춰서 공격 중일 때 못찾게해야함!!!!!!!!!!!
    if (m_bAttack)
    {
        return;
    }

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
    //길을 못찾았을 경우, 타겟방향으로 총을 날리도록/타겟방향의 벽을 부수도록
    else if (m_NodeList.empty() && Get_Target())
    {
        //타겟과 자신사이에서 장애물을 찾고
        //타겟을 그 장애물로 한다.
        Set_Target(Get_ObstacleToTarget());
        m_bNavigating = true;
        ////가까워지면 그 장애물을 파괴한다.
        //CObj* pTarget = Get_Target();
        //if (pTarget)
        //{
        //    Get_Target()->Set_Destroyed();
        //}
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

    //상대가 보이는 가?(타겟과 자신 사이에 장애물이 없는가?)// 상대가 벽일 경우 이 경우 무시
    if (strcmp(typeid(*m_pTarget).name(), "class CSteelWall") && !IsCanSeeTarget()) 
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
   
    if (m_fTargetDist < fRange)
    {
        //발사 시도
        pRangedWeapon->Fire();
        //가까우면 true
        return true;
    }

    return false;
}

bool CPawn::IsCanSeeTarget()
{
    // 브레센햄 알고리즘을 사용해 장애물 여부 판단
    int iThis_Index = CTileMgr::Get_TileIndex(m_tInfo.fX, m_tInfo.fY);
    int iTarget_Index = CTileMgr::Get_TileIndex(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY);

    int iX1 = iThis_Index % TILEX;
    int iY1 = iThis_Index / TILEX;

    int iX2 = iTarget_Index % TILEX;
    int iY2 = iTarget_Index / TILEX;

    int iDistX = abs(iX2 - iX1);
    int iDistY = abs(iY2 - iY1);
    int iDirX = (iX1 < iX2) ? 1 : -1;
    int iDirY = (iY1 < iY2) ? 1 : -1;
    int iErr = iDistX - iDistY;

    while (true) {
        // 현재 타일에 장애물이 있는지 확인
        if (CTileMgr::Get_Instance()->Get_TileOption(iX1, iY1) == OPT_BLOCKED)
        {
            return false;  // 장애물이 있으면 false 반환
        }

        // 목표 지점에 도달했으면 종료
        if (iX1 == iX2 && iY1 == iY2)
        {
            break;
        }

        int iError = 2 * iErr;
        if (iError > -iDistY) 
        {
            iErr -= iDistY;
            iX1 += iDirX;
        }
        if (iError < iDistX) 
        {
            iErr += iDistX;
            iY1 += iDirY;
        }
    }

    // 경로상에 장애물이 없으면 true 반환
    return true;
}

CObj* CPawn::Get_ObstacleToTarget()
{
    // 브레센햄 알고리즘을 사용해 장애물 여부 판단
    int iThis_Index = CTileMgr::Get_TileIndex(m_tInfo.fX, m_tInfo.fY);
    int iTarget_Index = CTileMgr::Get_TileIndex(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY);

    int iX1 = iThis_Index % TILEX;
    int iY1 = iThis_Index / TILEX;

    int iX2 = iTarget_Index % TILEX;
    int iY2 = iTarget_Index / TILEX;

    int iDistX = abs(iX2 - iX1);
    int iDistY = abs(iY2 - iY1);
    int iDirX = (iX1 < iX2) ? 1 : -1;
    int iDirY = (iY1 < iY2) ? 1 : -1;
    int iErr = iDistX - iDistY;

    while (true) {
        // 현재 타일에 장애물이 있는지 확인
        if (CTileMgr::Get_Instance()->Get_TileOption(iX1, iY1) == OPT_BLOCKED)
        {
            CObj* pObj = CTileMgr::Get_Instance()->Get_TileObj(iX1, iY1);  // 장애물이 있으면 false 반환
            return pObj;
        }

        // 목표 지점에 도달했으면 종료
        if (iX1 == iX2 && iY1 == iY2)
        {
            break;
        }

        int iError = 2 * iErr;
        if (iError > -iDistY)
        {
            iErr -= iDistY;
            iX1 += iDirX;
        }
        if (iError < iDistX)
        {
            iErr += iDistX;
            iY1 += iDirY;
        }
    }

    // 경로상에 장애물이 없으면 nullptr 반환
    return nullptr;
}

// Directions: 상, 하, 좌, 우
const int dx[4] = { 0, 0, -1, 1 };
const int dy[4] = { -1 * TILEX, 1 * TILEX, 0, 0 };

int CPawn::Find_NearestReachableTile(int iIndexX, int iIndexY)
{
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

}

void CPawn::Release()
{
    for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
    m_NodeList.clear();
}

void CPawn::OnCollision(OBJID _eID, CObj* _pOther)
{
    
}
