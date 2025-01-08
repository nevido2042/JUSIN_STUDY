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
    //°¡·Á´Â °÷ÀÌ ¸·Èù ºÎºÐÀÌ¸é ¸®ÅÏ
    if (CTileMgr::Get_Instance()->Get_TileOption(_Pos) == OPT_BLOCKED)
    {
        return;
    }

    for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
    m_NodeList.clear();

    //ÀÌµ¿ ÇÒ Å¸ÀÏÀÇ idx¸¦ °è»êÇØ¼­ È®ÀÎÇÑ´Ù. BlockedÀÌ¸é return;

    //°¥ ¼ö ÀÖÀ¸¸é ±æÃ£±â¸¦ ¼öÇàÇÑ´Ù.(Astar/JPS)
    m_NodeList = move(CPathFinder::Get_Instance()->Find_Path(POS{ (int)m_tInfo.fX, (int)m_tInfo.fY }, _Pos));
    //Å¸°ÙÀÌ WallÀÌ¸é ±æÀ» ¸øÃ£´Â´Ù!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //Å¸°ÙÀÇ ±ÙÃ³ Å¸ÀÏÀ» Ã£¾Æ¼­ °¡°Ô²û ÇØ¾ßÇÒµí


    if (!m_NodeList.empty())
    {
        m_bNavigating = true;
    }
    //±æÀ» ¸øÃ£¾ÒÀ» °æ¿ì
    else if (m_NodeList.empty() && Get_Target())
    {
        Change_State(DECONSTRUCTING, m_pTarget);
        //Å¸°Ù°ú ÀÚ½Å»çÀÌ¿¡¼­ Àå¾Ö¹°À» Ã£°í
        //Å¸°ÙÀ» ±× Àå¾Ö¹°·Î ÇÑ´Ù.
        Set_Target(Get_ObstacleToTarget());
    }
}

void CPawn::Take_Damage(const float _fDamage)
{
    if (m_bDead)
    {
        return;
    }

    m_fHP -= _fDamage;

    //Á×À½ Ã³¸®
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
    //¸®½ºÆ®¿¡¼­ Á¡ÇÏ³ª¸¦ »Ì¾Æ¼­
    //ÇØ´ç Á¡À¸·Î °¡±î¿öÁú ¶§±îÁö Á¢±Ù(µÎÁ¡ »çÀÌ °¢µµ ±¸ÇØ¼­ ÀÌµ¿)
    //°ÅÀÇ °¡±î¿öÁö¸é ³ëµå ÇØÁ¦ÇÏ°í
    //´ÙÀ½ ³ëµå¸¦ »Ì¾Æ¼­ ÃßÀû
    //´ÙÀ½ ³ëµå°¡ ¾øÀ¸¸é Á¾·á

    CNode* pTargetNode = nullptr;

    if (!m_NodeList.empty())
    {
        pTargetNode = m_NodeList.front();
    }

    //Å¸°Ù ³ëµå¿Í ÃæºÐÈ÷ °¡±î¿öÁö¸é
    if (pTargetNode && CNode::Distance(pTargetNode->Get_Pos(), POS{ (int)m_tInfo.fX, (int)m_tInfo.fY }) < TILECX * 0.1f)
    {
        //µµÂøÀ¸·Î °£ÁÖÇÏ°í Å¸°Ù ³ëµå·Î À§Ä¡ÀÌµ¿
        m_tInfo.fX = (float)pTargetNode->Get_Pos().iX;
        m_tInfo.fY = (float)pTargetNode->Get_Pos().iY;

        //µµÂøÇÑ ³ëµå¸¦ »èÁ¦
        Safe_Delete<CNode*>(pTargetNode);
        m_NodeList.pop_front();

        //»õ·Î¿î ±æÀ» Á¦½ÃÇÏ¸é ³×ºñ°ÔÀÌÆÃÀ¸ Á¾·á ¿¹¾à °ÉÀÚ
        //¿©±â¼­ Å¸°Ù°úÀÇ °Å¸®¸¦ Ã¼Å©ÇØ¼­ ´õ ¿òÁ÷ÀÏÁö °í¹ÎÇÒ±î?
        //Å¸°ÙÀÌ ÃæºÐÈ÷ °¡±õ´Ù¸é
        //³ª¸ÓÁö³ëµåµéÀ» ¼¼ÀÌÇÁµô¸®Æ®ÇÏ°í
        //³×ºñ°ÔÀÌÆÃ Á¾·á
        if (m_bAttack /*|| m_bNavStopRequested*/)//°ø°Ý ÁßÀÌ°Å³ª ±æÃ£±â Á¾·á ¿äÃ»ÀÌ ÀÖÀ¸¸é
        {
            //³ª¸ÓÁö³ëµåµéÀ» ¼¼ÀÌÇÁµô¸®Æ®ÇÏ°í
            for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
            m_NodeList.clear();
            //³×ºñ°ÔÀÌÆÃ Á¾·á
            m_bNavigating = false;
            /*m_bNavStopRequested = false;*/
            return;
        }
        
        //³ëµå°¡ ºñ¾îÀÖÁö ¾Ê´Ù¸é
        if (!m_NodeList.empty())
        {
            //´ÙÀ½ ³ëµå¸¦ Å¸°Ù ³ëµå·Î ¼³Á¤
            pTargetNode = m_NodeList.front();
        }
        else
        {
            //¾Æ´Ï¸é Å¸°Ù³ëµå ¾øÀ½
            pTargetNode = nullptr;
        }
    }

    //Å¸°Ù ³ëµå ¾øÀ¸¸é
    if (!pTargetNode)
    {
        //³×ºñ°ÔÀÌÆÃ Á¾·á
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
    //¸¶Áö¸· ³ëµåÀÇ ¹æÇâÀ¸·Î ÆÇ´Ü
    if (m_bNavigating && !m_NodeList.empty())
    {
        CNode* pLastNode = m_NodeList.back();
        POS tLastPos = pLastNode->Get_Pos();

        //¿ÞÂÊ
        if ((int)m_tInfo.fX > tLastPos.iX)
        {
            m_eDir = LL;
        }
        //¿À¸¥ÂÊ
        else if ((int)m_tInfo.fX < tLastPos.iX)
        {
            m_eDir = RR;
        }
        //À§ÂÊ
        else if ((int)m_tInfo.fY > tLastPos.iY)
        {
            m_eDir = UU;
        }
        //¾Æ·¡ÂÊ
        else if ((int)m_tInfo.fY < tLastPos.iY)
        {
            m_eDir = DD;
        }
    }
    else
    {
        m_eDir = DD;
    }

    //ÀÌÀü ÇÁ·¹ÀÓ À§Ä¡ ÀúÀå
    //m_tPrevPos.iX = (int)m_tInfo.fX;
    //m_tPrevPos.iY = (int)m_tInfo.fY;


    if (m_pTarget)
    {
        //°ø°Ý ÁßÀÌ°í, Å¸°ÙÀÌ ÀÖÀ» ¶§¸¸ Å¸°Ù ¹Ù¶óº¸±â    //¸Ø­ŸÀ» ¶§ Å¸°Ù ¹Ù¶óº¸±â
        if (m_bAttack || !m_bNavigating)
        {
            //xÃàÀÌ °ÅÀÇ °°À» ¶§¸¸ yÃà ºñ±³
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
    //Å¸°ÙÀÌ ¾øÀ¸¸é ¸®ÅÏ
    if (!m_pTarget)
    {
        return;
    }

    //Å¸°ÙÀÇ °Å¸®¿Í fRangeºñ±³
    float   fWidth(0.f), fHeight(0.f), fRadian(0.f);
    fWidth = m_pTarget->Get_Info().fX - m_tInfo.fX;
    fHeight = m_pTarget->Get_Info().fY - m_tInfo.fY;
    //Å¸°Ù°úÀÇ °Å¸®
    m_fTargetDist = sqrtf(fWidth * fWidth + fHeight * fHeight);

    fRadian = acosf(fWidth / m_fTargetDist);

    if (m_pTarget->Get_Info().fY > m_tInfo.fY)
        fRadian = (2.f * PI) - fRadian;

    m_fTargetAngle = fRadian * (180.f / PI);
}

bool CPawn::IsWithinRange()
{
    //º»ÀÎÀÌ Á×¾úÀ¸¸é ¸®ÅÏ
    if (m_bDead)
    {
        return false;
    }

    //Å¸°ÙÀÌ ¾ø¾îµµ ¸®ÅÏ
    if (!m_pTarget)
    {
        return false;
    }

    //¿ø°Å¸® ¹«±â°¡ ¾øÀ¸¸é
    if (!m_pRangedWeapon)
    {
        //Å¸ÀÏ 1Ä­ °Å¸®·Î ÆÇ´Ü.

        if (m_fTargetDist < TILECX)
        {
            return true;
        }


        return false;
    }

    CRangedWeapon* pRangedWeapon = static_cast<CRangedWeapon*>(m_pRangedWeapon);
    float fRange = pRangedWeapon->Get_Range();
   
    //°Å¸®°¡ °¡±õ°Å³ª
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
            return false;  // Àå¾Ö¹°ÀÌ ÀÖÀ¸¸é Å½»ö Á¾·á
        }
        return true;  // °è¼Ó Å½»ö
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
            return false;  // Àå¾Ö¹°À» Ã£À¸¸é Å½»ö Á¾·á
        }
        return true;  // °è¼Ó Å½»ö
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
        return true;  // °è¼Ó Å½»ö
        });

    return iClosestIndex;
}


int CPawn::Find_NearestReachableTile(int iIndexX, int iIndexY)
{
    // Directions: »ó, ÇÏ, ÁÂ, ¿ì
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

        // Á¢±Ù °¡´ÉÇÑ Å¸ÀÏ Ã£À½
        if (CTileMgr::Get_Instance()->Get_TileOption(iX, iY) == OPT_REACHABLE)
        {
            return iIndex;
        }

        // »óÇÏÁÂ¿ì Å½»ö
        for (int i = 0; i < 4; ++i) {
            int nx = iX + dx[i];
            int ny = iY + dy[i];

            // Áöµµ ¹üÀ§ ³»¿¡ ÀÖ°í ¾ÆÁ÷ ¹æ¹®ÇÏÁö ¾Ê¾Ò´Ù¸é Å¥¿¡ Ãß°¡
            if (nx >= 0 && nx < TILEX && ny >= 0 && ny < TILEY && !visited[nx + ny * TILEX]) 
            {
                visited[nx + ny * TILEX] = true;
                q.push(nx + ny * TILEX);
            }
        }
    }

    // ½ÇÆÐ ½Ã -1 ¹ÝÈ¯
    return -1;
}

void CPawn::Initialize()
{
    //HP¹Ù »ý¼º
    CObj* pObj = CAbstractFactory<CHealthBar>::Create();
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pObj);
    //HP¹ÙÀÇ Å¸°ÙÀ» ÀÌ PawnÀ¸·Î ¼³Á¤
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
    ////Á×¾úÀ¸¸é ¸®ÅÏ
    //if (m_bDead)
    //{
    //    return OBJ_NOEVENT;
    //}

    return CObj::Update();
}

void CPawn::Late_Update()
{
    //ÀÓ½Ã ¹æÆíÀ¸·Î Çß´Âµ¥ ÀÌ°Ô ¸Â´ÂÁö ¸ð¸£°ÚÀ½.
    if (m_bDestroyed)
        return;

    //Á×¾úÀ¸¸é ¸®ÅÏ
    if (m_bDead)
    {
        return;
    }

    //ÀÌµ¿¹æÇâ °è»ê
    Calculate_MoveDir();

    if (m_pTarget)
    {
        //Å¸°Ù°úÀÇ °Å¸® ¹× °¢µµ °è»ê
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


    //ÁÂÅ¬¸¯Àº Å¸°ÙÀ¸·Î ¼³Á¤
    if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
    {
        //¸¶¿ì½º°¡ ¿Ã¶ó°¡Á®ÀÖ´Â°¡?
        if (Is_MouseHovered_Scrolled())
        {
            CColonyMgr::Get_Instance()->Set_Target(this);
        }
    }

    //¿ìÅ¬¸¯Àº Å¸°ÙÀÇ °ø°Ý Å¸°ÙÀ¸·Î ¼³Á¤
    if (CKeyMgr::Get_Instance()->Key_Up(VK_RBUTTON))
    {
        //¸¶¿ì½º°¡ ¿Ã¶ó°¡Á®ÀÖ´Â°¡?
        if (Is_MouseHovered_Scrolled())
        {
            if (CObj* pTarget = CColonyMgr::Get_Instance()->Get_Target())
            {
                //ÀÚ±â ÀÚ½ÅÀÌ Å¸°ÙÀÌ µÉ ¼ö ¾øÀ½.
                if (pTarget != this)
                {
                    pTarget->Set_Target(this);
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

    //±æ Ã£±â ³ëµå Ãâ·Â
    /*for (CNode* pNode : m_NodeList)
    {
        Ellipse(hDC, int(pNode->Get_Pos().iX + iScrollX - 10.f), int(pNode->Get_Pos().iY + iScrollY - 10.f),
            int(pNode->Get_Pos().iX + 10.f + iScrollX), int(pNode->Get_Pos().iY + 10.f + iScrollY));
    }*/

    // ±æ Ã£±â ³ëµå Ãâ·Â (³ëµå¸¦ ¿¬°áÇÏ´Â µÎ²¨¿î È¸»ö ¼± ±×¸®±â)
    if (!m_NodeList.empty()) {
        // ÀÌÀü ³ëµå Æ÷ÀÎÅÍ
        CNode* pPrevNode = nullptr;

        // È¸»ö Ææ »ý¼º (µÎ²² 3)
        HPEN hGrayPen = CreatePen(PS_SOLID, int(3 * CCamera::Get_Instance()->Get_Zoom()), RGB(255, 255, 255));
        HPEN hOldPen = (HPEN)SelectObject(hDC, hGrayPen);

        // ³ëµå ¸®½ºÆ® ¼øÈ¸
        for (CNode* pNode : m_NodeList) {
            // ÇöÀç ³ëµåÀÇ À§Ä¡

            POINT tPoint = CCamera::Get_Instance()->WorldToScreen((float)(pNode->Get_Pos().iX), (float)(pNode->Get_Pos().iY));

            int iCurrentX = int(tPoint.x);
            int iCurrentY = int(tPoint.y);

            // ÀÌÀü ³ëµå°¡ ÀÖ´Ù¸é ¼±À» ±×¸²
            if (pPrevNode != nullptr) 
            {
                POINT tPoint = CCamera::Get_Instance()->WorldToScreen((float)(pPrevNode->Get_Pos().iX), (float)(pPrevNode->Get_Pos().iY));

                int iPrevX = int(tPoint.x);
                int iPrevY = int(tPoint.y);

                // ¼± ±×¸®±â
                MoveToEx(hDC, iPrevX, iPrevY, nullptr);
                LineTo(hDC, iCurrentX, iCurrentY);
            }

            // ÇöÀç ³ëµå¸¦ ÀÌÀü ³ëµå·Î ¼³Á¤
            pPrevNode = pNode;
        }

        // ¿ø·¡ ÆæÀ¸·Î º¹¿øÇÏ°í È¸»ö Ææ »èÁ¦
        SelectObject(hDC, hOldPen);
        DeleteObject(hGrayPen);
    }


    // º¯¼ö °ªÀ» À¯´ÏÄÚµå ¹®ÀÚ¿­·Î º¯È¯
    wchar_t buffer[50];
    wsprintf(buffer, L"m_bAttack: %d", m_bAttack);
    // ¹®ÀÚ¿­ Ãâ·Â (À¯´ÏÄÚµå)
    //TextOutW(hDC, int(m_tInfo.fX + iScrollX), int(m_tInfo.fY + iScrollY), buffer, lstrlenW(buffer));
    // º¯¼ö °ªÀ» À¯´ÏÄÚµå ¹®ÀÚ¿­·Î º¯È¯
    wsprintf(buffer, L"m_pTarget: %p", m_pTarget);
    // ¹®ÀÚ¿­ Ãâ·Â (À¯´ÏÄÚµå)
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
    // ¹®ÀÚ¿­ Ãâ·Â (À¯´ÏÄÚµå)
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
        //°¥ ¼ö ÀÖ´Â °÷ ·£´ý À§Ä¡ ÇÏ³ª Á¤ÇØ¼­ ¿òÁ÷ÀÎ´Ù.
        //°¥ ¼ö ÀÖ´Â Å¸ÀÏÀ» Å¸ÀÏ¸Å´ÏÀú°¡ ¸®½ºÆ®·Î ÇÏ³ª °¡Áö°í ÀÖ´Ù°¡. °Ç³×ÁÖ´Â ¹æ½ÄÀÌ ÁÁ°Ú´Ù.########

        //ÇÑ Ä­¾¿ ±æÃ£°ÔÇÏÀÚ
        //½Ã°è¹æÇâÀ¸·Î Å½»ö, °¥ ¼ö ÀÖ´Â Å¸ÀÏÀÌ ÀÖÀ¸¸é º¤ÅÍ¿¡ ³Ö´Â´Ù.
        //³Ö°í ·£´ý »Ì±â
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
