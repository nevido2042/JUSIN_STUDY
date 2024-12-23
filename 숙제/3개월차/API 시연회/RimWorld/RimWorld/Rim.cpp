#include "pch.h"
#include "Rim.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ColonyMgr.h"
#include "KeyMgr.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "TileMgr.h"
#include "PathFinder.h"
#include "SteelWall.h"
#include "TimeMgr.h"

CRim::CRim()
    :m_bTaskCheck(false)
{

}

CRim::~CRim()
{
    Release();
}

void CRim::Initialize()
{
    CPawn::Initialize();

    m_fSpeed = 2.f;

    m_fMaxHP = 100.f;
    m_fHP = m_fMaxHP;

    //¹«±â »ı¼º
    m_pRangedWeapon = CAbstractFactory<CBoltActionRifle>::Create(m_tInfo.fX, m_tInfo.fY);
    CObjMgr::Get_Instance()->Add_Object(OBJ_WEAPON, m_pRangedWeapon);
    m_pRangedWeapon->Set_Target(this);

    Take_Damage(10.f);

    Change_State(WANDERING);
}

int CRim::Update()
{
    return CPawn::Update();
}

void CRim::Late_Update()
{
    CPawn::Late_Update();

    //¸¶¿ì½º°¡ ¿Ã¶ó°¡Á®ÀÖ´Â°¡?
    if (Is_MouseHovered_Scrolled())
    {
        //ÁÂÅ¬¸¯Àº Å¸°ÙÀ¸·Î ¼³Á¤
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

    /*Ellipse(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY,
        m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);*/

    // ¸² ¸öÅë, ¾ó±¼, ¸Ó¸® ¼ø
    HDC hTestDC(nullptr);
    switch (m_eDir)
    {
    case UU:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Naked_Male_north");
        // ¸öÅë
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        //¾ó±¼
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Male_Average_Normal_north");
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        // ¸Ó¸®
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Afro_north");
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        break;

    case RR:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Naked_Male_east");
        // ¸öÅë
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        //¾ó±¼
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Male_Average_Normal_east");
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        // ¸Ó¸®
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Afro_east");
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        break;

    case DD:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Naked_Male_south");
        // ¸öÅë
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        //¾ó±¼
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Male_Average_Normal_south");
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        // ¸Ó¸®
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Afro_south");
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        break;

    case LL:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Naked_Male_west");
        // ¸öÅë
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        //¾ó±¼
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Male_Average_Normal_west");
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        // ¸Ó¸®
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Afro_west");
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        break;
    default:
        break;
    }

    //Á×¾úÀ» ¶§ ÇÍÀÚ±¹ Ãâ·Â
    if (m_bDead)
    {
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Blood");
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX,
            m_tRect.top + iScrollY,
            64, 64,
            hTestDC, 0, 0, 64, 64,
            RGB_WHITE);
    }
    

    //±æ Ã£±â ³ëµå Ãâ·Â
    for (CNode* pNode : m_NodeList)
    {
        Ellipse(hDC, int(pNode->Get_Pos().iX + iScrollX - 10.f), int(pNode->Get_Pos().iY + iScrollY - 10.f),
            int(pNode->Get_Pos().iX + 10.f + iScrollX), int(pNode->Get_Pos().iY + 10.f + iScrollY));
    }

    // º¯¼ö °ªÀ» À¯´ÏÄÚµå ¹®ÀÚ¿­·Î º¯È¯
    wchar_t buffer[50];
    wsprintf(buffer, L"m_bAttack: %d", m_bAttack);
    // ¹®ÀÚ¿­ Ãâ·Â (À¯´ÏÄÚµå)
    TextOutW(hDC, int(m_tInfo.fX + iScrollX), int(m_tInfo.fY + iScrollY), buffer, lstrlenW(buffer));
    // º¯¼ö °ªÀ» À¯´ÏÄÚµå ¹®ÀÚ¿­·Î º¯È¯
    wsprintf(buffer, L"m_pTarget: %p", m_pTarget);
    // ¹®ÀÚ¿­ Ãâ·Â (À¯´ÏÄÚµå)
    TextOutW(hDC, int(m_tInfo.fX + iScrollX), int(m_tInfo.fY + iScrollY + 20), buffer, lstrlenW(buffer));

    switch (Get_State())
    {
    case CRim::WANDERING:
        wsprintf(buffer, L"m_eState: %s", L"WANDERING");
        break;
    case CRim::DRAFTED:
        wsprintf(buffer, L"m_eState: %s", L"DRAFTED");
        break;
    case CRim::UNDRAFTED:
        wsprintf(buffer, L"m_eState: %s", L"UNDRAFTED");
        break;
    case CRim::CONSTRUCTING:
        wsprintf(buffer, L"m_eState: %s", L"CONSTRUCTING");
        break;
    case CRim::DECONSTRUCTING:
        wsprintf(buffer, L"m_eState: %s", L"DECONSTRUCTING");
        break;
    case CRim::END:
        break;
    default:
        break;
    }
    // ¹®ÀÚ¿­ Ãâ·Â (À¯´ÏÄÚµå)
    TextOutW(hDC, int(m_tInfo.fX + iScrollX), int(m_tInfo.fY + iScrollY + 40), buffer, lstrlenW(buffer));

}

void CRim::Handle_Wandering()
{
    //¹èÈ¸ ÇÏÀÚ
    Wander();
    //»õ·Î¿î ÀÛ¾÷ÀÌ »ı°å´Ù¸é?????????? ÀÛ¾÷ÀÇ °¹¼ö°¡ ´Ş¶óÁ³´Ù¸é?
    //ÀÛ¾÷ ¸ñ·ÏÀÌ ´Ş¶óÁ³´Ù¸é? ÀÛ¾÷¸®½ºÆ®¸¦ ¸²ÀÌ º¹»ç¿¡¼­ °¡Áö°í ÀÖ´Â´Ù?
    // 
    //####Äİ·Î´Ï ¸Å´ÏÀú¿¡¼­ ÀÛ¾÷¸®½ºÆ® ¹Ù²î¾ú´Ù°í ¾Ë·ÁÁÖ¸é?<ÀÌ°Ô ¸Â´Âµí?>
    //####Äİ·Î´Ï ¸Å´ÏÀú¿¡¼­ ÀÛ¾÷ÀÌ ¹Ù²î¾úÀ» ¶§(Ãß°¡, »èÁ¦) ¸ğµç ¸²¿¡°Ô, ÀÛ¾÷ È®ÀÎÇÏ¶ó Áö½Ã
    
    //ÀÛ¾÷À» Ã¼Å©ÇÒ ½Ã°£ÀÌ ‰ç´Ù¸é(ÀÌ°Å »©¹ö¸®°í)
    if (m_bTaskCheck)
    {
        Check_ConstructWork();
        Check_DeconstructWork();
        //m_fElapsedTimeCheck = 0.f;
        m_bTaskCheck = false;
    }
}

void CRim::Handle_Drafted()
{
    if (m_pTarget)
    {
        //Å¸°ÙÀÌ Á×À¸¸é Å¸°Ù ¾ø¾Ö±â
        CPawn* pTarget = static_cast<CPawn*>(m_pTarget);
        if (pTarget->Get_IsDead())
        {
            Set_Target(nullptr);
            //ÀûÀÌ Á×¾úÀ¸¸é ´Ù¸¥ ÀûÀ» Ã£´Â´Ù.
            //Find_Enemy();
        }
        else
        {
            //¸ØÃç¼­ °ø°İ ÁßÀÏ ¶§ ¸øÃ£°ÔÇØ¾ßÇÔ!!!!!!!!!!!
            //Move_To(POS{ m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY });
        }
    }
    else
    {
        Find_Enemy();
    }

    //»çÁ¤°Å¸®¿¡ ÀÖ°í ÀûÀÌ º¸ÀÎ´Ù¸é
    if (IsWithinRange() && IsCanSeeTarget())
    {
        static_cast<CRangedWeapon*>(m_pRangedWeapon)->Fire();
        m_bAttack = true;
    }
    else
    {
        m_bAttack = false;
    }

}

void CRim::Handle_Undrafted()
{

}

void CRim::Handle_Deconstructing()
{
    //Å¸°ÙÀÌ °¡±î¿îÁö È®ÀÎ
    if (m_fTargetDist < TILECX * 1.5f)
    {
        //°¡±î¿ì¸é ¸ØÃá´Ù.
        //RequestNavStop();
        m_bNavigating = false;
    }
    //¸Ø­Ÿ´Âµ¥
    if (!m_bNavigating)
    {
        //Å¸°ÙÀÌ ¾ø´Ù.
        if (!m_pTarget)
        {
            return;
        }
        //Å¸°ÙÀÌ ¸Ö´Ù
        if (m_fTargetDist > TILECX * 1.5f)
        {
            //´Ù¸¥ ÀÛ¾÷ Ã¼Å©
            m_bTaskCheck = true;
            Change_State(WANDERING);
            return;
        }
        //ÇØÃ¼ ÇÏ´Â°Å
        Deconstruct();
    }

    //ÇØÃ¼ ÁøÇàÁßÀÎ ¹Ù »ı¼º
    //¸îÃÊ µÚ ÇØÃ¼ ¿Ï·á
}

void CRim::Handle_Constructing()
{
    //Å¸°ÙÀÌ °¡±î¿îÁö È®ÀÎ
    if (m_fTargetDist < TILECX * 1.5f)
    {
        //°¡±î¿ì¸é ¸ØÃá´Ù.
        //RequestNavStop();
        m_bNavigating = false;
    }
    //¸Ø­Ÿ´Âµ¥
    if (!m_bNavigating)
    {
        //Å¸°ÙÀÌ ¾ø´Ù.
        if (!m_pTarget)
        {
            return;
        }
        //Å¸°ÙÀÌ ¸Ö´Ù
        if (m_fTargetDist > TILECX * 1.5f)
        {
            //´Ù¸¥ ÀÛ¾÷ Ã¼Å©
            m_bTaskCheck = true;
            m_pTarget = nullptr;
            Change_State(WANDERING);
            return;
        }
        //°Ç¼³
        Construct();
    }



    //ÇØÃ¼ ÁøÇàÁßÀÎ ¹Ù »ı¼º
    //¸îÃÊ µÚ ÇØÃ¼ ¿Ï·á
}

void CRim::Check_CloseTask()
{
}

void CRim::Deconstruct()
{
    CSteelWall* pWall = static_cast<CSteelWall*>(m_pTarget);
    pWall->Set_IsBrokenDown();
    Change_State(WANDERING);
}

void CRim::Construct()
{
    CObj* pObj = CAbstractFactory<CSteelWall>::Create(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY);
    CObjMgr::Get_Instance()->Add_Object(OBJ_WALL, pObj);
    CTileMgr::Get_Instance()->Set_TileOption(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY, OPT_BLOCKED);
    CTileMgr::Get_Instance()->Set_TileObj(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY, pObj);

    //ÀÛ¾÷»èÁ¦
    //CColonyMgr::Get_Instance()->Get_ConstructSet()->erase(m_pTarget);
    set<TASK>& ConstructSet = *CColonyMgr::Get_Instance()->Get_ConstructSet();
    for (auto Iter = ConstructSet.begin(); Iter != ConstructSet.end();)
    {
        if ((*Iter).pObj == m_pTarget)
        {
            Iter = ConstructSet.erase(Iter);
        }
        else
        {
            ++Iter;
        }
    }

    m_pTarget = nullptr;
    Change_State(WANDERING);
}

void CRim::Check_DeconstructWork()
{
    //½Ä¹ÎÁö °ü¸®ÀÚ¿¡ ÇØÃ¼ÇÒ º®µéÀÌ ÀÖ´ÂÁö È®ÀÎ         //±×¸®°í ±æ µû¶ó°¡´Â ÁßÀÌ¾Æ´Ï°í, ÀÛ¾÷»óÅÂÁßÀÌ ¾Æ´Ò¶§ ¸¸ , »õ·Î¿î ÀÛ¾÷ÀÌ »ı°åÀ» ¶§ °Ë»ç
    if (!CColonyMgr::Get_Instance()->Get_DeconstructSet()->empty() && Get_State() == WANDERING) //ÀÌ°Å ¸ó½ºÅÍ º®ºÎ¼ö·¯°¡´Â °Å¿¡ Àû¿ë ÇÏ¸é µÉµí?
    {
        //ÇØÃ¼ÇÒ º®µé Áß ±æÀ» Ã£À» ¼ö ÀÖ´Â °ÍÀÌ ³ª¿À¸é
        //ÇØ´ç º®µ¹ ÁÖº¯ÀÇ 8°³ÀÇ Å¸ÀÏÀ» È®ÀÎÇØ¼­ ±æÀ» Ã£À» ¼ö ÀÖ´ÂÁö È®ÀÎ
        //±æÀ» ¸øÃ£À¸¸é ÇØÃ¼ÇÏÁö¸»°í, ±æÀ» Ã£À¸¸é ÇØÃ¼ÇÏ·¯°¡¶ó

        //SetÀ» vector·Î º¹»çÈÄ Á¤·Ä
        set<TASK>& DeconstructSet = *CColonyMgr::Get_Instance()->Get_DeconstructSet();
        vector<TASK> vecDeconstruct(DeconstructSet.begin(), DeconstructSet.end());
        // »ç¿ëÀÚ Á¤ÀÇ Á¤·Ä: ±âÁØÁ¡°úÀÇ °Å¸®¸¦ °è»êÇØ Á¤·Ä
        std::sort(vecDeconstruct.begin(), vecDeconstruct.end(), 
            [this](const TASK _tTaskA, const TASK _tTaskB) 
            {
                // ¿¹¾àµÇÁö ¾ÊÀ¸¸é ¿ì¼±¼øÀ§ ¾ÕÀ¸·Î
                if (!_tTaskA.pRimReserved && _tTaskB.pRimReserved)
                {
                    return true;  // _tTaskA°¡ ´õ ¾Õ¿¡ ¿Àµµ·Ï
                }

                if (_tTaskA.pRimReserved && !_tTaskB.pRimReserved)
                {
                    return false; // _tTaskB°¡ ´õ ¾Õ¿¡ ¿Àµµ·Ï
                }

                float fDistA = CObj::Calculate_Dist(this, _tTaskA.pObj);
                float fDistB = CObj::Calculate_Dist(this, _tTaskB.pObj);
                return fDistA < fDistB; // °Å¸®°¡ °¡±î¿ï¼ö·Ï ¾ÕÂÊÀ¸·Î Á¤·Ä
            });

        //ÀÛ¾÷ÇØ¾ßÇÒ º®µé Å½»ö(³ª¿¡°Ô °¡Àå °¡±î¿î ³à¼®µé·Î Á¤·Ä)
        for (TASK _tTask : vecDeconstruct)
        {
            //±âÁ¸ ³ëµå µô¸®Æ®
            for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
            m_NodeList.clear();

            //ÀÌµ¿ °¡´ÉÇÑ Å¸ÀÏÀÌ ÀÖÀ¸¸é ³ëµå¸®½ºÆ® ¹İÈ¯
            m_NodeList = move(CTileMgr::Get_Instance()
                ->Find_ReachableTiles(POS{ (int)m_tInfo.fX, (int)m_tInfo.fY }, POS{ (int)_tTask.pObj->Get_Info().fX, (int)_tTask.pObj->Get_Info().fY }));

            if (m_NodeList.empty())
            {
                continue;
            }

            Set_Target(_tTask.pObj);
            //ÀÛ¾÷ ¸®½ºÆ®¿¡¼­ °í¸¥ ÀÛ¾÷À» ¿¹¾àÇßÀ½À» Ç¥½Ã
            for (auto Iter = DeconstructSet.begin(); Iter != DeconstructSet.end();)
            {
                if ((*Iter).pObj == m_pTarget)
                {
                    //»èÁ¦ ÈÄ
                    Iter = DeconstructSet.erase(Iter);
                    //¼öÁ¤ÇØ¼­ Ãß°¡
                    TASK tTask;
                    tTask.pObj = m_pTarget;
                    tTask.pRimReserved = this;
                    DeconstructSet.emplace(tTask);
                }
                else
                {
                    ++Iter;
                }
            }

            m_bNavigating = true;
            Change_State(DECONSTRUCTING);
            return;
            //Move_To(POS{ pTile->Get_Info().fX,pTile->Get_Info().fX });
        }
    }
}

void CRim::Check_ConstructWork()
{
    //½Ä¹ÎÁö °ü¸®ÀÚ¿¡ ÇØÃ¼ÇÒ º®µéÀÌ ÀÖ´ÂÁö È®ÀÎ         //±×¸®°í ±æ µû¶ó°¡´Â ÁßÀÌ¾Æ´Ï°í, ÀÛ¾÷»óÅÂÁßÀÌ ¾Æ´Ò¶§ ¸¸ , »õ·Î¿î ÀÛ¾÷ÀÌ »ı°åÀ» ¶§ °Ë»ç
    if (!CColonyMgr::Get_Instance()->Get_ConstructSet()->empty() && Get_State() == WANDERING) //ÀÌ°Å ¸ó½ºÅÍ º®ºÎ¼ö·¯°¡´Â °Å¿¡ Àû¿ë ÇÏ¸é µÉµí?
    {
        //ÇØÃ¼ÇÒ º®µé Áß ±æÀ» Ã£À» ¼ö ÀÖ´Â °ÍÀÌ ³ª¿À¸é
        //ÇØ´ç º®µ¹ ÁÖº¯ÀÇ 8°³ÀÇ Å¸ÀÏÀ» È®ÀÎÇØ¼­ ±æÀ» Ã£À» ¼ö ÀÖ´ÂÁö È®ÀÎ
        //±æÀ» ¸øÃ£À¸¸é ÇØÃ¼ÇÏÁö¸»°í, ±æÀ» Ã£À¸¸é ÇØÃ¼ÇÏ·¯°¡¶ó

        //SetÀ» vector·Î º¹»çÈÄ Á¤·Ä
        set<TASK>& ConstructSet = *CColonyMgr::Get_Instance()->Get_ConstructSet();

        //ÇØ´ç Å¸ÀÏ À§¿¡ Ã¶ ¾ÆÀÌÅÛÀÌ ¾øÀ¸¸é ¸øÁöÀ½.

        vector<TASK> vecConstruct(ConstructSet.begin(), ConstructSet.end());

        // »ç¿ëÀÚ Á¤ÀÇ Á¤·Ä: ±âÁØÁ¡°úÀÇ °Å¸®¸¦ °è»êÇØ Á¤·Ä
        std::sort(vecConstruct.begin(), vecConstruct.end(), 
            [this](const TASK _tTaskA, const TASK _tTaskB)
            {
                // ¿¹¾àµÇÁö ¾ÊÀ¸¸é ¿ì¼±¼øÀ§ ¾ÕÀ¸·Î
                if (!_tTaskA.pRimReserved && _tTaskB.pRimReserved)
                {
                    return true;  // _tTaskA°¡ ´õ ¾Õ¿¡ ¿Àµµ·Ï
                }

                if (_tTaskA.pRimReserved && !_tTaskB.pRimReserved)
                {
                    return false; // _tTaskB°¡ ´õ ¾Õ¿¡ ¿Àµµ·Ï
                }

                float fDistA = CObj::Calculate_Dist(this, _tTaskA.pObj);
                float fDistB = CObj::Calculate_Dist(this, _tTaskB.pObj);
                return fDistA < fDistB; // °Å¸®°¡ °¡±î¿ï¼ö·Ï ¾ÕÂÊÀ¸·Î Á¤·Ä
            });

        //ÀÛ¾÷ÇØ¾ßÇÒ º®µé Å½»ö(³ª¿¡°Ô °¡Àå °¡±î¿î ³à¼®µé·Î Á¤·Ä)
        for (TASK tTask : vecConstruct)
        {
            //±âÁ¸ ³ëµå µô¸®Æ®
            for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
            m_NodeList.clear();

            //ÀÌµ¿ °¡´ÉÇÑ Å¸ÀÏÀÌ ÀÖÀ¸¸é ³ëµå¸®½ºÆ® ¹İÈ¯
            m_NodeList = move(CTileMgr::Get_Instance()
                ->Find_ReachableTiles(POS{ (int)m_tInfo.fX, (int)m_tInfo.fY }, POS{ (int)tTask.pObj->Get_Info().fX, (int)tTask.pObj->Get_Info().fY }));

            if (m_NodeList.empty())
            {
                continue;
            }

            Set_Target(tTask.pObj);
            //ÀÛ¾÷ ¸®½ºÆ®¿¡¼­ °í¸¥ ÀÛ¾÷À» ¿¹¾àÇßÀ½À» Ç¥½Ã
            for (auto Iter = ConstructSet.begin(); Iter != ConstructSet.end();)
            {
                if ((*Iter).pObj == m_pTarget)
                {
                    //»èÁ¦ ÈÄ
                    Iter = ConstructSet.erase(Iter);
                    //¼öÁ¤ÇØ¼­ Ãß°¡
                    TASK tTask;
                    tTask.pObj = m_pTarget;
                    tTask.pRimReserved = this;
                    ConstructSet.emplace(tTask);
                }
                else
                {
                    ++Iter;
                }
            }
            m_bNavigating = true;
            Change_State(CONSTRUCTING);
            return;
            //Move_To(POS{ pTile->Get_Info().fX,pTile->Get_Info().fX });
        }
    }
}

void CRim::Find_Enemy()
{
    for (CObj* pObj : CObjMgr::Get_Instance()->Get_List()[OBJ_ENEMY])
    {
        if (static_cast<CPawn*>(pObj)->Get_IsDead())
        {
            return;
        }

        Set_Target(pObj);
        Measure_Target();
        if (IsWithinRange())
        {
            break;
        }
        else
        {
            Set_Target(nullptr);
        }
    }
}

