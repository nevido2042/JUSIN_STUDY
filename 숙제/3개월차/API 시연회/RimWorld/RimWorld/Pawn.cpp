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
    ////���缭 ���� ���� �� ��ã���ؾ���!!!!!!!!!!!
    //if (m_bAttack)
    //{
    //    return;
    //}

    //��� ���󰡴� ���̸� ���� ����
    if (m_bNavigating)
    {
        return;
    }

    for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
    m_NodeList.clear();

    //�̵� �� Ÿ���� idx�� ����ؼ� Ȯ���Ѵ�. Blocked�̸� return;

    //�� �� ������ ��ã�⸦ �����Ѵ�.(Astar/JPS)
    m_NodeList = move(CPathFinder::Get_Instance()->Find_Path(POS{ m_tInfo.fX, m_tInfo.fY }, _Pos));
    //Ÿ���� Wall�̸� ���� ��ã�´�!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //Ÿ���� ��ó Ÿ���� ã�Ƽ� ���Բ� �ؾ��ҵ�


    if (!m_NodeList.empty())
    {
        m_bNavigating = true;
    }
    //���� ��ã���� ���
    else if (m_NodeList.empty() && Get_Target())
    {
        Change_State(DECONSTRUCTING);
        //Ÿ�ٰ� �ڽŻ��̿��� ��ֹ��� ã��
        //Ÿ���� �� ��ֹ��� �Ѵ�.
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

    //���� ó��
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
    //����Ʈ���� ���ϳ��� �̾Ƽ�
    //�ش� ������ ������� ������ ����(���� ���� ���� ���ؼ� �̵�)
    //���� ��������� ��� �����ϰ�
    //���� ��带 �̾Ƽ� ����
    //���� ��尡 ������ ����

    CNode* pTargetNode = nullptr;

    if (!m_NodeList.empty())
    {
        pTargetNode = m_NodeList.front();
    }

    //Ÿ�� ���� ����� ���������
    if (pTargetNode && CNode::Distance(pTargetNode->Get_Pos(), POS{ m_tInfo.fX, m_tInfo.fY }) < TILECX * 0.1f)
    {
        //�������� �����ϰ� Ÿ�� ���� ��ġ�̵�
        m_tInfo.fX = pTargetNode->Get_Pos().fX;
        m_tInfo.fY = pTargetNode->Get_Pos().fY;

        //������ ��带 ����
        Safe_Delete<CNode*>(pTargetNode);
        m_NodeList.pop_front();

        //���ο� ���� �����ϸ� �׺�������� ���� ���� ����
        //���⼭ Ÿ�ٰ��� �Ÿ��� üũ�ؼ� �� �������� ����ұ�?
        //Ÿ���� ����� �����ٸ�
        //������������ ����������Ʈ�ϰ�
        //�׺������ ����
        if (m_bAttack || m_bNavStopRequested)//���� ���̰ų� ��ã�� ���� ��û�� ������
        {
            //������������ ����������Ʈ�ϰ�
            for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
            m_NodeList.clear();
            //�׺������ ����
            m_bNavigating = false;
            m_bNavStopRequested = false;
            return;
        }
        
        //��尡 ������� �ʴٸ�
        if (!m_NodeList.empty())
        {
            //���� ��带 Ÿ�� ���� ����
            pTargetNode = m_NodeList.front();
        }
        else
        {
            //�ƴϸ� Ÿ�ٳ�� ����
            pTargetNode = nullptr;
        }
    }

    //Ÿ�� ��� ������
    if (!pTargetNode)
    {
        //�׺������ ����
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
    //����
    if (m_tInfo.fX < m_tPrevPos.fX)
    {
        m_eDir = LL;
    }
    //������
    else if (m_tInfo.fX > m_tPrevPos.fX)
    {
        m_eDir = RR;
    }
    //����
    else if (m_tInfo.fY < m_tPrevPos.fY)
    {
        m_eDir = UU;
    }
    //�Ʒ���
    else if (m_tInfo.fY > m_tPrevPos.fY)
    {
        m_eDir = DD;
    }

    //���� ������ ��ġ ����
    m_tPrevPos.fX = m_tInfo.fX;
    m_tPrevPos.fY = m_tInfo.fY;

    //���� ���̰�, Ÿ���� ���� ���� Ÿ�� �ٶ󺸱�
    if (m_bAttack && m_pTarget)
    {
        //x���� ���� ���� ���� y�� ��
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
    //Ÿ���� ������ ����
    if (!m_pTarget)
    {
        return;
    }

    //Ÿ���� �Ÿ��� fRange��
    float   fWidth(0.f), fHeight(0.f), fRadian(0.f);
    fWidth = m_pTarget->Get_Info().fX - m_tInfo.fX;
    fHeight = m_pTarget->Get_Info().fY - m_tInfo.fY;
    //Ÿ�ٰ��� �Ÿ�
    m_fTargetDist = sqrtf(fWidth * fWidth + fHeight * fHeight);

    fRadian = acosf(fWidth / m_fTargetDist);

    if (m_pTarget->Get_Info().fY > m_tInfo.fY)
        fRadian = (2.f * PI) - fRadian;

    m_fTargetAngle = fRadian * (180.f / PI);
}

bool CPawn::IsWithinRange()
{
    //������ �׾����� ����
    if (m_bDead)
    {
        return false;
    }

    //Ÿ���� ��� ����
    if (!m_pTarget)
    {
        return false;
    }

    //���Ÿ� ���Ⱑ ������
    if (!m_pRangedWeapon)
    {
        //Ÿ�� 1ĭ �Ÿ��� �Ǵ�.

        if (m_fTargetDist < TILECX)
        {
            return true;
        }


        return false;
    }

    CRangedWeapon* pRangedWeapon = static_cast<CRangedWeapon*>(m_pRangedWeapon);
    float fRange = pRangedWeapon->Get_Range();
   
    //�Ÿ��� �����ų�
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
            return false;  // ��ֹ��� ������ Ž�� ����
        }
        return true;  // ��� Ž��
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
            return false;  // ��ֹ��� ã���� Ž�� ����
        }
        return true;  // ��� Ž��
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
        return true;  // ��� Ž��
        });

    return iClosestIndex;
}


int CPawn::Find_NearestReachableTile(int iIndexX, int iIndexY)
{
    // Directions: ��, ��, ��, ��
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

        // ���� ������ Ÿ�� ã��
        if (CTileMgr::Get_Instance()->Get_TileOption(iX, iY) == OPT_REACHABLE)
        {
            return iIndex;
        }

        // �����¿� Ž��
        for (int i = 0; i < 4; ++i) {
            int nx = iX + dx[i];
            int ny = iY + dy[i];

            // ���� ���� ���� �ְ� ���� �湮���� �ʾҴٸ� ť�� �߰�
            if (nx >= 0 && nx < TILEX && ny >= 0 && ny < TILEY && !visited[nx + ny * TILEX]) 
            {
                visited[nx + ny * TILEX] = true;
                q.push(nx + ny * TILEX);
            }
        }
    }

    // ���� �� -1 ��ȯ
    return -1;
}

void CPawn::Initialize()
{
    //HP�� ����
    CObj* pObj = CAbstractFactory<CHealthBar>::Create();
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pObj);
    //HP���� Ÿ���� �� Pawn���� ����
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
    //�׾����� ����
    if (m_bDead)
    {
        return;
    }

    //�̵����� ���
    Calculate_MoveDir();

    if (m_pTarget)
    {
        //Ÿ�ٰ��� �Ÿ� �� ���� ���
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
