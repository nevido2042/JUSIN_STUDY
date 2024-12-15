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
    //���缭 ���� ���� �� ��ã���ؾ���!!!!!!!!!!!
    if (m_bAttack)
    {
        return;
    }

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
    //���� ��ã���� ���, Ÿ�ٹ������� ���� ��������/Ÿ�ٹ����� ���� �μ�����
    else if (m_NodeList.empty() && Get_Target())
    {
        //Ÿ�ٰ� �ڽŻ��̿��� ��ֹ��� ã��
        //Ÿ���� �� ��ֹ��� �Ѵ�.
        Set_Target(Get_ObstacleToTarget());
        m_bNavigating = true;
        ////��������� �� ��ֹ��� �ı��Ѵ�.
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

    //���� ó��
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

    //��밡 ���̴� ��?(Ÿ�ٰ� �ڽ� ���̿� ��ֹ��� ���°�?)// ��밡 ���� ��� �� ��� ����
    if (strcmp(typeid(*m_pTarget).name(), "class CSteelWall") && !IsCanSeeTarget()) 
    {
        return false;
    }

    //���Ÿ� ���Ⱑ ������ ������ �ʴٰ� �Ǵ�.
    if (!m_pRangedWeapon)
    {
        return false;
    }

    CRangedWeapon* pRangedWeapon = static_cast<CRangedWeapon*>(m_pRangedWeapon);
    float fRange = pRangedWeapon->Get_Range();
   
    if (m_fTargetDist < fRange)
    {
        //�߻� �õ�
        pRangedWeapon->Fire();
        //������ true
        return true;
    }

    return false;
}

bool CPawn::IsCanSeeTarget()
{
    // �극���� �˰����� ����� ��ֹ� ���� �Ǵ�
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
        // ���� Ÿ�Ͽ� ��ֹ��� �ִ��� Ȯ��
        if (CTileMgr::Get_Instance()->Get_TileOption(iX1, iY1) == OPT_BLOCKED)
        {
            return false;  // ��ֹ��� ������ false ��ȯ
        }

        // ��ǥ ������ ���������� ����
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

    // ��λ� ��ֹ��� ������ true ��ȯ
    return true;
}

CObj* CPawn::Get_ObstacleToTarget()
{
    // �극���� �˰����� ����� ��ֹ� ���� �Ǵ�
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
        // ���� Ÿ�Ͽ� ��ֹ��� �ִ��� Ȯ��
        if (CTileMgr::Get_Instance()->Get_TileOption(iX1, iY1) == OPT_BLOCKED)
        {
            CObj* pObj = CTileMgr::Get_Instance()->Get_TileObj(iX1, iY1);  // ��ֹ��� ������ false ��ȯ
            return pObj;
        }

        // ��ǥ ������ ���������� ����
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

    // ��λ� ��ֹ��� ������ nullptr ��ȯ
    return nullptr;
}

// Directions: ��, ��, ��, ��
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

}

void CPawn::Release()
{
    for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
    m_NodeList.clear();
}

void CPawn::OnCollision(OBJID _eID, CObj* _pOther)
{
    
}
