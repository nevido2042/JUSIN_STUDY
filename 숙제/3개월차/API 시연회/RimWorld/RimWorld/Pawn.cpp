#include "pch.h"
#include "Pawn.h"
#include "PathFinder.h"
#include "BmpMgr.h"

#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "RangedWeapon.h"
#include "TimeMgr.h"
#include "TileMgr.h"
#include "SteelWall.h"
#include "KeyMgr.h"
#include "ColonyMgr.h"
#include "EffectMgr.h"

CPawn::CPawn()
    :m_bNavigating(false), m_fHP(0.f), m_fMaxHP(0.f), m_bDead(false), m_eState(END),
    m_pRangedWeapon(nullptr), m_fTargetDist(0.f), m_fTargetAngle(0.f), m_bAttack(false),
    m_fMeleeAttackSpeed(0.f), m_fMeleeElapsed(0.f), m_fConstructTime(0.f), m_fConstructElapsed(0.f)
{
    //ZeroMemory(&m_tPrevPos, sizeof(POS));
    ZeroMemory(&m_eCurrentTask, sizeof(TASK));
}

CPawn::~CPawn()
{
    Release();
}

void CPawn::Move_To(POS _Pos)
{
    //가려는 곳이 막힌 부분이면 리턴
    if (CTileMgr::Get_Instance()->Get_TileOption(_Pos) == OPT_BLOCKED)
    {
        return;
    }

    for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
    m_NodeList.clear();

    //이동 할 타일의 idx를 계산해서 확인한다. Blocked이면 return;

    //갈 수 있으면 길찾기를 수행한다.(Astar/JPS)
    m_NodeList = move(CPathFinder::Get_Instance()->Find_Path(POS{ (int)m_tInfo.fX, (int)m_tInfo.fY }, _Pos));
    //타겟이 Wall이면 길을 못찾는다!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //타겟의 근처 타일을 찾아서 가게끔 해야할듯


    if (!m_NodeList.empty())
    {
        m_bNavigating = true;
    }
    //길을 못찾았을 경우
    else if (m_NodeList.empty() && Get_Target())
    {
        //타겟과 자신사이에서 장애물을 찾고
        //타겟을 그 장애물로 한다.
        Set_Target(Get_ObstacleToTarget());
        Change_State(DECONSTRUCTING, m_pTarget);
    }
}

void CPawn::Take_Damage(const float _fDamage)
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

void CPawn::Change_State(STATE _eState, CObj* _pTarget)
{
    if (m_eState == _eState)
    {
        return;
    }

    m_eState = _eState;
    m_pTarget = _pTarget;
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
    if (pTargetNode && CNode::Distance(pTargetNode->Get_Pos(), POS{ (int)m_tInfo.fX, (int)m_tInfo.fY }) < TILECX * 0.1f)
    {
        //도착으로 간주하고 타겟 노드로 위치이동
        m_tInfo.fX = (float)pTargetNode->Get_Pos().iX;
        m_tInfo.fY = (float)pTargetNode->Get_Pos().iY;

        //도착한 노드를 삭제
        Safe_Delete<CNode*>(pTargetNode);
        m_NodeList.pop_front();

        //새로운 길을 제시하면 네비게이팅으 종료 예약 걸자
        //여기서 타겟과의 거리를 체크해서 더 움직일지 고민할까?
        //타겟이 충분히 가깝다면
        //나머지노드들을 세이프딜리트하고
        //네비게이팅 종료
        if (m_bAttack /*|| m_bNavStopRequested*/)//공격 중이거나 길찾기 종료 요청이 있으면
        {
            //나머지노드들을 세이프딜리트하고
            for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
            m_NodeList.clear();
            //네비게이팅 종료
            m_bNavigating = false;
            /*m_bNavStopRequested = false;*/
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

    fWidth = pTargetNode->Get_Pos().iX - m_tInfo.fX;
    fHeight = pTargetNode->Get_Pos().iY - m_tInfo.fY;

    fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

    fRadian = acosf(fWidth / fDiagonal);

    if (pTargetNode->Get_Pos().iY > m_tInfo.fY)
        fRadian = (2.f * PI) - fRadian;

    m_fAngle = fRadian * (180.f / PI);

    m_tInfo.fX += m_fSpeed * cosf(fRadian) * GAMESPEED;
    m_tInfo.fY -= m_fSpeed * sinf(fRadian) * GAMESPEED;
}

void CPawn::Calculate_MoveDir()
{
    //마지막 노드의 방향으로 판단
    if (m_bNavigating && !m_NodeList.empty())
    {
        CNode* pLastNode = m_NodeList.back();
        POS tLastPos = pLastNode->Get_Pos();

        //왼쪽
        if ((int)m_tInfo.fX > tLastPos.iX)
        {
            m_eDir = LL;
        }
        //오른쪽
        else if ((int)m_tInfo.fX < tLastPos.iX)
        {
            m_eDir = RR;
        }
        //위쪽
        else if ((int)m_tInfo.fY > tLastPos.iY)
        {
            m_eDir = UU;
        }
        //아래쪽
        else if ((int)m_tInfo.fY < tLastPos.iY)
        {
            m_eDir = DD;
        }
    }
    else
    {
        m_eDir = DD;
    }

    //이전 프레임 위치 저장
    //m_tPrevPos.iX = (int)m_tInfo.fX;
    //m_tPrevPos.iY = (int)m_tInfo.fY;


    if (m_pTarget)
    {
        //공격 중이고, 타겟이 있을 때만 타겟 바라보기    //멈췃을 때 타겟 바라보기
        if (m_bAttack || !m_bNavigating)
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
    if (!m_pTarget)
    {
        return nullptr;
    }

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

    m_tInfo.fCX = 64.f;
    m_tInfo.fCY = 64.f;

    m_eRenderID = RENDER_GAMEOBJECT;

    Change_State(WANDERING);

    m_fMeleeAttackSpeed = 50.f;

    m_fConstructTime = 300.f;
}

int CPawn::Update()
{
    ////죽었으면 리턴
    //if (m_bDead)
    //{
    //    return OBJ_NOEVENT;
    //}

    return CObj::Update();
}

void CPawn::Late_Update()
{
    //임시 방편으로 했는데 이게 맞는지 모르겠음.
    if (m_bDestroyed)
        return;

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
    case CPawn::TRANSPORTING:
        Handle_Transporting();
        break;
    case CPawn::BOARDING:
        Handle_Boarding();
        break;
    case CPawn::LOGGING:
        Handle_Logging();
        break;
    case CPawn::MOVETOWORK:
        Handle_MoveToWork();
        break;
    case CPawn::END:
        break;
    default:
        break;
    }


    //좌클릭은 타겟으로 설정
    if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
    {
        //마우스가 올라가져있는가?
        if (Is_MouseHovered_Scrolled())
        {
            CColonyMgr::Get_Instance()->Set_Target(this);
        }
    }

    //우클릭은 타겟의 공격 타겟으로 설정
    if (CKeyMgr::Get_Instance()->Key_Up(VK_RBUTTON))
    {
        //마우스가 올라가져있는가?
        if (Is_MouseHovered_Scrolled())
        {
            if (CObj* pTarget = CColonyMgr::Get_Instance()->Get_Target())
            {
                //자기 자신이 타겟이 될 수 없음.
                if (pTarget != this)
                {
                    pTarget->Set_Target(this);
                    
                    if (CPawn* pPawn = dynamic_cast<CPawn*>(pTarget))
                    {
                        pPawn->Change_State(CHASING, this);
                    }


                    POS tPos{ (int)m_tInfo.fX, (int)m_tInfo.fY };
                    CEffectMgr::Get_Instance()->Create_Effect(tPos, 64.f, 64.f, L"FeedbackShoot", 30.f);
                }
            }
        }
    }

    m_fMeleeElapsed += GAMESPEED;
}

void CPawn::Render(HDC hDC)
{

    //길 찾기 노드 출력
    /*for (CNode* pNode : m_NodeList)
    {
        Ellipse(hDC, int(pNode->Get_Pos().iX + iScrollX - 10.f), int(pNode->Get_Pos().iY + iScrollY - 10.f),
            int(pNode->Get_Pos().iX + 10.f + iScrollX), int(pNode->Get_Pos().iY + 10.f + iScrollY));
    }*/

    // 길 찾기 노드 출력 (노드를 연결하는 두꺼운 회색 선 그리기)
    if (!m_NodeList.empty()) {
        // 이전 노드 포인터
        CNode* pPrevNode = nullptr;

        // 회색 펜 생성 (두께 3)
        HPEN hGrayPen = CreatePen(PS_SOLID, int(3 * CCamera::Get_Instance()->Get_Zoom()), RGB(255, 255, 255));
        HPEN hOldPen = (HPEN)SelectObject(hDC, hGrayPen);

        // 노드 리스트 순회
        for (CNode* pNode : m_NodeList) {
            // 현재 노드의 위치

            POINT tPoint = CCamera::Get_Instance()->WorldToScreen((float)(pNode->Get_Pos().iX), (float)(pNode->Get_Pos().iY));

            int iCurrentX = int(tPoint.x);
            int iCurrentY = int(tPoint.y);

            // 이전 노드가 있다면 선을 그림
            if (pPrevNode != nullptr) 
            {
                POINT tPoint = CCamera::Get_Instance()->WorldToScreen((float)(pPrevNode->Get_Pos().iX), (float)(pPrevNode->Get_Pos().iY));

                int iPrevX = int(tPoint.x);
                int iPrevY = int(tPoint.y);

                // 선 그리기
                MoveToEx(hDC, iPrevX, iPrevY, nullptr);
                LineTo(hDC, iCurrentX, iCurrentY);
            }

            // 현재 노드를 이전 노드로 설정
            pPrevNode = pNode;
        }

        // 원래 펜으로 복원하고 회색 펜 삭제
        SelectObject(hDC, hOldPen);
        DeleteObject(hGrayPen);
    }


    // 변수 값을 유니코드 문자열로 변환
    wchar_t buffer[50];
    wsprintf(buffer, L"m_bAttack: %d", m_bAttack);
    // 문자열 출력 (유니코드)
    //TextOutW(hDC, int(m_tInfo.fX + iScrollX), int(m_tInfo.fY + iScrollY), buffer, lstrlenW(buffer));
    // 변수 값을 유니코드 문자열로 변환
    wsprintf(buffer, L"m_pTarget: %p", m_pTarget);
    // 문자열 출력 (유니코드)
    //TextOutW(hDC, int(m_tInfo.fX + iScrollX), int(m_tInfo.fY + iScrollY + 20), buffer, lstrlenW(buffer));

    switch (Get_State())
    {
    case CPawn::WANDERING:
        wsprintf(buffer, L"m_eState: %s", L"WANDERING");
        break;
    case CPawn::DRAFTED:
        wsprintf(buffer, L"m_eState: %s", L"DRAFTED");
        break;
    case CPawn::UNDRAFTED:
        wsprintf(buffer, L"m_eState: %s", L"UNDRAFTED");
        break;
    case CPawn::CONSTRUCTING:
        wsprintf(buffer, L"m_eState: %s", L"CONSTRUCTING");
        break;
    case CPawn::DECONSTRUCTING:
        wsprintf(buffer, L"m_eState: %s", L"DECONSTRUCTING");
        break;
    case CPawn::TRANSPORTING:
        wsprintf(buffer, L"m_eState: %s", L"TRANSPORTING");
        break;
    case CPawn::BOARDING:
        wsprintf(buffer, L"m_eState: %s", L"BOARDING");
        break;
    case CPawn::MOVETOWORK:
        wsprintf(buffer, L"m_eState: %s", L"MOVE TO WORK");
        break;
    case CPawn::LOGGING:
        wsprintf(buffer, L"m_eState: %s", L"LOGGING");
        break;
    case CPawn::END:
        break;
    default:
        break;
    }
    // 문자열 출력 (유니코드)
    //TextOutW(hDC, int(m_tInfo.fX + iScrollX), int(m_tInfo.fY + iScrollY + 40), buffer, lstrlenW(buffer));
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
    if (!m_pTarget)
    {
        Change_State(WANDERING);
    }

    //타겟 있으면 따라가기
    if (m_pTarget)
    {
        POS tMoveToPos{ (int)m_pTarget->Get_Info().fX, (int)m_pTarget->Get_Info().fY };

        //타겟이 Pawn이라면
        if (CPawn* pPawnTarget = dynamic_cast<CPawn*>(m_pTarget))//타겟이 Pawn이고 죽었으면
        {
            //Pawn이 죽었다면
            if (pPawnTarget->Get_IsDead())
            {
                Set_Target(nullptr);
                Change_State(WANDERING);

                //RequestNavStop();
                m_bNavigating = false;
            }

        }
        if (!m_bNavigating)
        {
            Move_To(tMoveToPos);
        }

    }

    //사정거리 내에 있고, 적이 보여야함
    if (IsWithinRange() && IsCanSeeTarget())
    {
        //RequestNavStop();
        m_bNavigating = false;
        static_cast<CRangedWeapon*>(m_pRangedWeapon)->Fire();
        m_bAttack = true;
    }
    else
    {
        m_bAttack = false;
    }
}

void CPawn::Handle_Deconstructing()
{
}

void CPawn::Handle_Constructing()
{
}

void CPawn::Handle_Transporting()
{
}

void CPawn::Handle_Boarding()
{
}

void CPawn::Handle_Logging()
{
}

void CPawn::Handle_MoveToWork()
{
}

void CPawn::Wander()
{
    if (!m_bNavigating)
    {
        //갈 수 있는 곳 랜덤 위치 하나 정해서 움직인다.
        //갈 수 있는 타일을 타일매니저가 리스트로 하나 가지고 있다가. 건네주는 방식이 좋겠다.########

        //한 칸씩 길찾게하자
        //시계방향으로 탐색, 갈 수 있는 타일이 있으면 벡터에 넣는다.
        //넣고 랜덤 뽑기
        int iDX[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
        int iDY[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
        vector<CObj*> vecReachableTile;
        vecReachableTile.reserve(8);
        for (int i = 0; i < 8; ++i)
        {
            float fX = m_tInfo.fX + (iDX[i] * TILECX);
            float fY = m_tInfo.fY + (iDY[i] * TILECY);

            if (CTileMgr::Get_Instance()->Get_TileOption(fX, fY) == OPT_REACHABLE)
            {
                CObj* pObj = CTileMgr::Get_Instance()->Get_Tile(fX, fY);
                vecReachableTile.push_back(pObj);
            }
        }

        if (vecReachableTile.empty())
        {
            return;
        }

        CObj* pRandTile = vecReachableTile.at(rand() % vecReachableTile.size());

        POS tPos
        {
            (int)pRandTile->Get_Info().fX,
            (int)pRandTile->Get_Info().fY
        };

        Move_To(tPos);
    }
}
