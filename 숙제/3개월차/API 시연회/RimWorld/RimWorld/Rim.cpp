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
#include "SoundMgr.h"
#include "Ship.h"

CRim::CRim()
    :m_bTaskCheck(false), m_pTransportingItem(nullptr)
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

    //¹«±â »ý¼º
    m_pRangedWeapon = CAbstractFactory<CBoltActionRifle>::Create(m_tInfo.fX, m_tInfo.fY);
    CObjMgr::Get_Instance()->Add_Object(OBJ_WEAPON, m_pRangedWeapon);
    m_pRangedWeapon->Set_Target(this);

    //Take_Damage(10.f);

    Change_State(WANDERING);

    //Ä³¸¯ÅÍ ÀÌ¹ÌÁö ¼³Á¤
    //m_pImgKey_Body = L"Naked_Female"; //L"Naked_Male";
    //m_pImgKey_Face = L"Female_Average_Normal";//L"Male_Average_Normal";
    //m_pImgKey_Hair = L"Decent"; //L"Afro";

    int iRand(0);
    iRand = rand() % 3;
    // ·£´ýÀ¸·Î Body ÀÌ¹ÌÁö ¼±ÅÃ
    if (iRand == 0)
    {
        m_pImgKey_Body = L"Naked_Female";
    }
    else if(iRand == 1)
    {
        m_pImgKey_Body = L"Naked_Male";
    }
    else if (iRand == 2)
    {
        m_pImgKey_Body = L"Naked_Fat";
    }

    // ·£´ýÀ¸·Î Face ÀÌ¹ÌÁö ¼±ÅÃ
    if (rand() % 2 == 0) 
    {
        m_pImgKey_Face = L"Female_Average_Normal";
    }
    else 
    {
        m_pImgKey_Face = L"Male_Average_Normal";
    }

    // ·£´ýÀ¸·Î Hair ÀÌ¹ÌÁö ¼±ÅÃ
    iRand = rand() % 3;
    if (iRand == 0)
    {
        m_pImgKey_Hair = L"Decent";
    }
    else if(iRand == 1)
    {
        m_pImgKey_Hair = L"Afro";
    }
    else if (iRand == 2)
    {
        m_pImgKey_Hair = L"Pigtails";
    }

    m_ImgKeyArr_Body[NORTH] = m_pImgKey_Body + L"_north";
    m_ImgKeyArr_Body[EAST] = m_pImgKey_Body + L"_east";
    m_ImgKeyArr_Body[SOUTH] = m_pImgKey_Body + L"_south";
    m_ImgKeyArr_Body[WEST] = m_pImgKey_Body + L"_west";

    m_ImgKeyArr_Face[NORTH] = m_pImgKey_Face + L"_north";
    m_ImgKeyArr_Face[EAST] = m_pImgKey_Face + L"_east";
    m_ImgKeyArr_Face[SOUTH] = m_pImgKey_Face + L"_south";
    m_ImgKeyArr_Face[WEST] = m_pImgKey_Face + L"_west";

    m_ImgKeyArr_Hair[NORTH] = m_pImgKey_Hair + L"_north";
    m_ImgKeyArr_Hair[EAST] = m_pImgKey_Hair + L"_east";
    m_ImgKeyArr_Hair[SOUTH] = m_pImgKey_Hair + L"_south";
    m_ImgKeyArr_Hair[WEST] = m_pImgKey_Hair + L"_west";
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

    CPawn::Render(hDC);

    /*Ellipse(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY,
        m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);*/

    // ¸² ¸öÅë, ¾ó±¼, ¸Ó¸® ¼ø
    HDC hTestDC(nullptr);
    switch (m_eDir)
    {
    case UU:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(m_ImgKeyArr_Body[NORTH].c_str());
        // ¸öÅë
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        //¾ó±¼
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(m_ImgKeyArr_Face[NORTH].c_str());
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        // ¸Ó¸®
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(m_ImgKeyArr_Hair[NORTH].c_str());
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        break;

    case RR:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(m_ImgKeyArr_Body[EAST].c_str());
        // ¸öÅë
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        //¾ó±¼
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(m_ImgKeyArr_Face[EAST].c_str());
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        // ¸Ó¸®
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(m_ImgKeyArr_Hair[EAST].c_str());
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        break;

    case DD:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(m_ImgKeyArr_Body[SOUTH].c_str());
        // ¸öÅë
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        //¾ó±¼
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(m_ImgKeyArr_Face[SOUTH].c_str());
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        // ¸Ó¸®
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(m_ImgKeyArr_Hair[SOUTH].c_str());
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        break;

    case LL:
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(m_ImgKeyArr_Body[WEST].c_str());
        // ¸öÅë
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X
            , m_tRect.top + iScrollY - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        //¾ó±¼
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(m_ImgKeyArr_Face[WEST].c_str());
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX - IMAGE_OFFSET_X,
            m_tRect.top + iScrollY - HEAD_OFFSET - IMAGE_OFFSET_Y,
            128, 128,
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
        // ¸Ó¸®
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(m_ImgKeyArr_Hair[WEST].c_str());
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
}

void CRim::Handle_Wandering()
{
    //¹èÈ¸ ÇÏÀÚ
    Wander();

    //»õ·Î¿î ÀÛ¾÷ÀÌ »ý°å´Ù¸é?????????? ÀÛ¾÷ÀÇ °¹¼ö°¡ ´Þ¶óÁ³´Ù¸é?
    //ÀÛ¾÷ ¸ñ·ÏÀÌ ´Þ¶óÁ³´Ù¸é? ÀÛ¾÷¸®½ºÆ®¸¦ ¸²ÀÌ º¹»ç¿¡¼­ °¡Áö°í ÀÖ´Â´Ù?
    // 
    //####ÄÝ·Î´Ï ¸Å´ÏÀú¿¡¼­ ÀÛ¾÷¸®½ºÆ® ¹Ù²î¾ú´Ù°í ¾Ë·ÁÁÖ¸é?<ÀÌ°Ô ¸Â´Âµí?>
    //####ÄÝ·Î´Ï ¸Å´ÏÀú¿¡¼­ ÀÛ¾÷ÀÌ ¹Ù²î¾úÀ» ¶§(Ãß°¡, »èÁ¦) ¸ðµç ¸²¿¡°Ô, ÀÛ¾÷ È®ÀÎÇÏ¶ó Áö½Ã
    
    //ÀÛ¾÷À» Ã¼Å©ÇÒ ½Ã°£ÀÌ ‰ç´Ù¸é(ÀÌ°Å »©¹ö¸®°í)
    if (m_bTaskCheck)
    {
        Check_ConstructWork();
        Check_DeconstructWork();
        Check_LoggingWork();
        //Check_TransportingWork();
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
            //¸ØÃç¼­ °ø°Ý ÁßÀÏ ¶§ ¸øÃ£°ÔÇØ¾ßÇÔ!!!!!!!!!!!
            //Move_To(POS{ m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY });
        }
    }
    else
    {
        if (!m_bNavigating)
        {
            Find_Enemy();
        }
    }

    //»çÁ¤°Å¸®¿¡ ÀÖ°í ÀûÀÌ º¸ÀÎ´Ù¸é
    if (IsWithinRange() && IsCanSeeTarget())
    {
        static_cast<CRangedWeapon*>(m_pRangedWeapon)->Fire();
        m_bAttack = true;
    }
    else
    {
        Set_Target(nullptr);
        m_bAttack = false;
    }

}

void CRim::Handle_Undrafted()
{

}

void CRim::Handle_Deconstructing()
{
    if (!m_pTarget)
    {
        m_bTaskCheck = true;
        Change_State(WANDERING);
        return;
    }

    Deconstruct();
}

void CRim::Handle_Constructing()
{

    //°Ç¼³ ÇÒ¶§ ÀÌ¹Ì º®ÀÌ ÀÖÀ¸¸é Ã¶ ³»·Á³õ°í ´Ù¸¥ ÀÏ Ã£¾Æ¶ó
    if (CTileMgr::Get_Instance()->Get_TileOption(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY) == OPT_BLOCKED)
    {
        m_bTaskCheck = true;
        Change_State(WANDERING);
        PutDown_Item();
        return;
    }

  
    //¿Å±â´Â Ã¶ÀÌ ¾ø´Ù.
    if (!m_pTransportingItem)
    {
        m_bTaskCheck = true;
        Change_State(WANDERING);
        return;
    }

    //Å¸°ÙÀÌ °¡±î¿îÁö È®ÀÎ
    if (m_fTargetDist < TILECX * 1.2f)
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
        if (m_fTargetDist > TILECX * 1.2f)
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



    //ÇØÃ¼ ÁøÇàÁßÀÎ ¹Ù »ý¼º
    //¸îÃÊ µÚ ÇØÃ¼ ¿Ï·á
}

void CRim::Handle_Transporting()
{

    //Å¸°ÙÀ¸·Î ÇÑ Ã¶ÀÌ ´Ù¸¥ ¾Ö°¡ ÀÌ¹Ì °¡Á®°¬À¸¸é
    if (m_pTarget)
    {
        CObj* pOwner = m_pTarget->Get_Target();
        if (pOwner && pOwner != this)
        {
            m_bTaskCheck = true;
            m_pTarget = nullptr;
            return;
        }
    }

    //Ã¶À» ¸øÃ£¾ÒÀ¸¸é
    if (!m_pTarget)
    {
        CObj* pItem = Find_Item(L"Steel_b");

        if (pItem)
        {
            //µé ¼ö ÀÖ´Â Ã¶ÀÌ ÀÖÀ¸¸é
            //¿î¹ÝÀ¸·Î º¯°æ
            m_pTarget = pItem;

            POS tPos{ (int)pItem->Get_Info().fX, (int)pItem->Get_Info().fY };
            Move_To(tPos);
            return;
            //PickUp_Item(pItem);
        }
        else
        {
            m_bTaskCheck = true;
            Change_State(WANDERING);
            PutDown_Item();
            return;
        }
    }

    //Å¸°Ù ÀÖ°í, ¿î¹ÝÇÏ´Â°Ô ¾ø°í, ³×ºñ°ÔÀÌ¼Ç ³¡³µÀ¸¸é Á¾·á
    if (m_pTarget && !m_pTransportingItem && !m_bNavigating)
    {
        m_bTaskCheck = true;
        Change_State(WANDERING);
        PutDown_Item();
        return;
    }


    //¿î¹ÝÇÏ°íÀÖ´Â°Ô ÀÖÀ¸¸é
    if (m_pTransportingItem)
    {
        m_pTarget = nullptr;

        Check_ConstructWork();

        //°Ç¼³ ÇÒ°Ô ¾øÀ¸¸é
        if (Get_State() != CONSTRUCTING)
        {
            m_bTaskCheck = true;
            Change_State(WANDERING);
            PutDown_Item();
        }
    }

    //Å¸°Ù ¾ÆÀÌÅÛÀÌ °¡±î¿öÁö¸é ¾ÆÀÌÅÛÀ» µé¾î¶ó
    if (!m_pTransportingItem && m_fTargetDist < TILECX * 0.5f)
    {
        if (m_pTarget)
        {
            PickUp_Item(m_pTarget);
        }
    }
}

void CRim::Handle_Boarding()
{
    if (!m_bNavigating)
    {
        int iTileIndex = CTileMgr::Get_TileIndex(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY);
        int iReachableTileIndex = Find_NearestReachableTile(iTileIndex % TILEX, iTileIndex / (TILEX));

        POS tPos{ (int)((iReachableTileIndex % TILEX) * TILECX + TILECX * 0.5f),
            (int)((iReachableTileIndex / TILEX) * TILECY + TILECY * 0.5f) };

        Move_To(tPos);
    }
    
    //Å¸°Ù(¿ìÁÖ¼±)°ú °Å¸®°¡ °¡±î¿öÁö¸é Å¾½Â
    if (m_fTargetDist < TILECX * 2.f)
    {
        m_bNavigating = false;
        Set_Pos(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY);
    }

}

void CRim::Handle_Logging()
{
    if (!m_pTarget)
    {
        m_bTaskCheck = true;
        Change_State(WANDERING);
        return;
    }

    Log();
}

void CRim::Handle_MoveToWork()
{
    if (m_pTarget)
    {
        if (CBreakable* pWall = dynamic_cast<CBreakable*>(m_pTarget))
        {
            if (pWall->Get_IsBrokenDown())
            {
                //´Ù¸¥ ÀÛ¾÷ Ã¼Å©
                m_bTaskCheck = true;
                Change_State(WANDERING);
                return;
            }
        }
        //Å¸°ÙÀÌ ÆÄ±«‰ç´Ù.
        if (m_pTarget->Get_Destroyed())
        {
            //´Ù¸¥ ÀÛ¾÷ Ã¼Å©
            m_bTaskCheck = true;
            Change_State(WANDERING);
            return;
        }
    }

    //Å¸°ÙÀÌ °¡±î¿îÁö È®ÀÎ
    if (m_fTargetDist < TILECX * 1.2f)
    {
        //°¡±î¿ì¸é ¸ØÃá´Ù.
        //RequestNavStop();
        m_bNavigating = false;

        if (m_pTarget->Get_ObjID() == OBJ_TREE)
        {
            Change_State(LOGGING, m_pTarget);
        }
        else if(m_pTarget->Get_ObjID() == OBJ_WALL)
        {
            Change_State(DECONSTRUCTING, m_pTarget);
        }


    }
}

void CRim::Check_CloseTask()
{
}

void CRim::Deconstruct()
{
    if (m_fMeleeAttackSpeed > m_fMeleeElapsed)
    {
        return;
    }

    m_fMeleeElapsed = 0.f;

    CSoundMgr::Get_Instance()->StopSound(SOUND_DECONSTRUCT);
    CSoundMgr::Get_Instance()->PlaySound(L"PickHitA.wav", SOUND_DECONSTRUCT, .5f);

    CBreakable* pWall = static_cast<CBreakable*>(m_pTarget);
    //pWall->Set_IsBrokenDown();
    //Change_State(WANDERING);

    //ÇØ´ç º®ÀÌ ¹«³ÊÁú ¶§ ±îÁö °ø°Ý ¹Ýº¹
    //½Ã°£ °ø°ÝÇÏ´Â ½Ã°£ Á¤ÇØ¾ßÇÔ

    pWall->Take_Damage(1.f);

}

void CRim::Construct()
{
    //Ã¶ »èÁ¦
    //CObj* pSteel =  CTileMgr::Get_Instance()->Get_TileObj(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY);
    //pSteel->Set_Destroyed();

    //ÇØÃ¼ ½Ã°£ °É¸®°Ô ÇÏ°í »ç¿îµå Ãâ·Â
    if (m_fConstructTime > m_fConstructElapsed)
    {
        //µå¸±¼Ò¸® ³ª°Ô
        CSoundMgr::Get_Instance()->PlaySound(L"DrillB.wav", SOUND_CONSTRUCT, .5f);
        m_fConstructElapsed += GAMESPEED;
        return;
    }

    m_fConstructElapsed = 0.f;

    CSoundMgr::Get_Instance()->StopSound(SOUND_CONSTRUCT);
    CSoundMgr::Get_Instance()->PlaySound(L"HammerA.wav", SOUND_CONSTRUCT, .5f);

    //µé°í ÀÖ´Â Ã¶À» »èÁ¦
    m_pTransportingItem->Set_Destroyed();
    m_pTransportingItem = nullptr;

    if (m_eCurrentTask.eType == TASK::SHIP)
    {
        //¿ìÁÖ¼± °Ç¼³
        CObj* pObj = CAbstractFactory<CShip>::Create(m_pTarget->Get_Info().fX + TILECX * 0.5f, m_pTarget->Get_Info().fY + TILECY * 0.5f);
        CObjMgr::Get_Instance()->Add_Object(OBJ_WALL, pObj);
        CTileMgr::Get_Instance()->Set_TileOption(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY, OPT_BLOCKED);
        CTileMgr::Get_Instance()->Set_TileOption(m_pTarget->Get_Info().fX + TILECX, m_pTarget->Get_Info().fY, OPT_BLOCKED);
        CTileMgr::Get_Instance()->Set_TileOption(m_pTarget->Get_Info().fX + TILECX, m_pTarget->Get_Info().fY + TILECY, OPT_BLOCKED);
        CTileMgr::Get_Instance()->Set_TileOption(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY + TILECY, OPT_BLOCKED);
        CTileMgr::Get_Instance()->Set_TileObj(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY, pObj);
    }
    else if (m_eCurrentTask.eType == TASK::WALL)
    {
        //º® °Ç¼³
        CObj* pObj = CAbstractFactory<CSteelWall>::Create(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY);
        CObjMgr::Get_Instance()->Add_Object(OBJ_WALL, pObj);
        CTileMgr::Get_Instance()->Set_TileOption(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY, OPT_BLOCKED);
        CTileMgr::Get_Instance()->Set_TileObj(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY, pObj);
    }

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

void CRim::Log()
{
    if (m_fMeleeAttackSpeed > m_fMeleeElapsed)
    {
        return;
    }

    m_fMeleeElapsed = 0.f;

    CSoundMgr::Get_Instance()->StopSound(SOUND_DECONSTRUCT);
    CSoundMgr::Get_Instance()->PlaySound(L"Tree_Chop_1a.wav", SOUND_DECONSTRUCT, .5f);

    CBreakable* pTree = static_cast<CBreakable*>(m_pTarget);
    //pWall->Set_IsBrokenDown();
    //Change_State(WANDERING);

    //ÇØ´ç º®ÀÌ ¹«³ÊÁú ¶§ ±îÁö °ø°Ý ¹Ýº¹
    //½Ã°£ °ø°ÝÇÏ´Â ½Ã°£ Á¤ÇØ¾ßÇÔ

    pTree->Take_Damage(1.f);

    if (pTree->Get_IsBrokenDown())
    {
        m_pTarget = nullptr;
    }

}

void CRim::Check_DeconstructWork()
{
    //½Ä¹ÎÁö °ü¸®ÀÚ¿¡ ÇØÃ¼ÇÒ º®µéÀÌ ÀÖ´ÂÁö È®ÀÎ         //±×¸®°í ±æ µû¶ó°¡´Â ÁßÀÌ¾Æ´Ï°í, ÀÛ¾÷»óÅÂÁßÀÌ ¾Æ´Ò¶§ ¸¸ , »õ·Î¿î ÀÛ¾÷ÀÌ »ý°åÀ» ¶§ °Ë»ç
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

            //ÀÌµ¿ °¡´ÉÇÑ Å¸ÀÏÀÌ ÀÖÀ¸¸é ³ëµå¸®½ºÆ® ¹ÝÈ¯
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
            Change_State(DECONSTRUCTING, _tTask.pObj);
            //Set_Target(_tTask.pObj);
            return;
            //Move_To(POS{ pTile->Get_Info().fX,pTile->Get_Info().fX });
        }
    }
}

void CRim::Check_ConstructWork()
{
    //½Ä¹ÎÁö °ü¸®ÀÚ¿¡ ÇØÃ¼ÇÒ º®µéÀÌ ÀÖ´ÂÁö È®ÀÎ         //±×¸®°í ±æ µû¶ó°¡´Â ÁßÀÌ¾Æ´Ï°í, ÀÛ¾÷»óÅÂÁßÀÌ ¾Æ´Ò¶§ ¸¸ , »õ·Î¿î ÀÛ¾÷ÀÌ »ý°åÀ» ¶§ °Ë»ç
    if (!CColonyMgr::Get_Instance()->Get_ConstructSet()->empty() && (Get_State() == WANDERING|| Get_State() == TRANSPORTING)) //ÀÌ°Å ¸ó½ºÅÍ º®ºÎ¼ö·¯°¡´Â °Å¿¡ Àû¿ë ÇÏ¸é µÉµí?
    {

        //³»°¡ Ã¶À» µé°í ÀÖ¾î¾ß¸¸ °Ç¼³ °¡´É
        //Ã¶À» ¾Èµé¾úÀ¸¸é Ã¶ Ã£¾Æ¼­ µé¾î¶ó
        //Ã¶ÀÌ ÀÖ¾î¾ßÇÔ
        if (!m_pTransportingItem)
        {
            CObj* pItem = Find_Item(L"Steel_b");

            if (pItem)
            {
                Change_State(TRANSPORTING);
                return;
            }
            else
            {
                return;
            }
        }

        //ÇØÃ¼ÇÒ º®µé Áß ±æÀ» Ã£À» ¼ö ÀÖ´Â °ÍÀÌ ³ª¿À¸é
        //ÇØ´ç º®µ¹ ÁÖº¯ÀÇ 8°³ÀÇ Å¸ÀÏÀ» È®ÀÎÇØ¼­ ±æÀ» Ã£À» ¼ö ÀÖ´ÂÁö È®ÀÎ
        //±æÀ» ¸øÃ£À¸¸é ÇØÃ¼ÇÏÁö¸»°í, ±æÀ» Ã£À¸¸é ÇØÃ¼ÇÏ·¯°¡¶ó

        //SetÀ» vector·Î º¹»çÈÄ Á¤·Ä
        set<TASK>& ConstructSet = *CColonyMgr::Get_Instance()->Get_ConstructSet();
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

            //ÀÌµ¿ °¡´ÉÇÑ Å¸ÀÏÀÌ ÀÖÀ¸¸é ³ëµå¸®½ºÆ® ¹ÝÈ¯
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
                    TASK tTas = *Iter;
                    //»èÁ¦ ÈÄ
                    Iter = ConstructSet.erase(Iter);
                    //¼öÁ¤ÇØ¼­ Ãß°¡
                    //tTask.pObj = m_pTarget;
                    tTask.pRimReserved = this;
                    //tTask.eType = TASK::WALL;
                    ConstructSet.emplace(tTask);
                }
                else
                {
                    ++Iter;
                }
            }
            m_bNavigating = true;
            m_eCurrentTask = tTask;
            Change_State(CONSTRUCTING, tTask.pObj);
            //Set_Target(tTask.pObj);
            return;
            //Move_To(POS{ pTile->Get_Info().fX,pTile->Get_Info().fX });
        }
    }
}

void CRim::Check_LoggingWork()
{
    //½Ä¹ÎÁö °ü¸®ÀÚ¿¡ ÇØÃ¼ÇÒ º®µéÀÌ ÀÖ´ÂÁö È®ÀÎ         //±×¸®°í ±æ µû¶ó°¡´Â ÁßÀÌ¾Æ´Ï°í, ÀÛ¾÷»óÅÂÁßÀÌ ¾Æ´Ò¶§ ¸¸ , »õ·Î¿î ÀÛ¾÷ÀÌ »ý°åÀ» ¶§ °Ë»ç
    if (!CColonyMgr::Get_Instance()->Get_LoggingSet()->empty() && Get_State() == WANDERING) //ÀÌ°Å ¸ó½ºÅÍ º®ºÎ¼ö·¯°¡´Â °Å¿¡ Àû¿ë ÇÏ¸é µÉµí?
    {
        //ÇØÃ¼ÇÒ º®µé Áß ±æÀ» Ã£À» ¼ö ÀÖ´Â °ÍÀÌ ³ª¿À¸é
        //ÇØ´ç º®µ¹ ÁÖº¯ÀÇ 8°³ÀÇ Å¸ÀÏÀ» È®ÀÎÇØ¼­ ±æÀ» Ã£À» ¼ö ÀÖ´ÂÁö È®ÀÎ
        //±æÀ» ¸øÃ£À¸¸é ÇØÃ¼ÇÏÁö¸»°í, ±æÀ» Ã£À¸¸é ÇØÃ¼ÇÏ·¯°¡¶ó

        //SetÀ» vector·Î º¹»çÈÄ Á¤·Ä
        set<TASK>& LoggingSet = *CColonyMgr::Get_Instance()->Get_LoggingSet();
        vector<TASK> vecLogging(LoggingSet.begin(), LoggingSet.end());
        // »ç¿ëÀÚ Á¤ÀÇ Á¤·Ä: ±âÁØÁ¡°úÀÇ °Å¸®¸¦ °è»êÇØ Á¤·Ä
        std::sort(vecLogging.begin(), vecLogging.end(),
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
        for (TASK _tTask : vecLogging)
        {
            //±âÁ¸ ³ëµå µô¸®Æ®
            for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
            m_NodeList.clear();

            //ÀÌµ¿ °¡´ÉÇÑ Å¸ÀÏÀÌ ÀÖÀ¸¸é ³ëµå¸®½ºÆ® ¹ÝÈ¯
            m_NodeList = move(CTileMgr::Get_Instance()
                ->Find_ReachableTiles(POS{ (int)m_tInfo.fX, (int)m_tInfo.fY }, POS{ (int)_tTask.pObj->Get_Info().fX, (int)_tTask.pObj->Get_Info().fY }));

            if (m_NodeList.empty())
            {
                continue;
            }

            Set_Target(_tTask.pObj);
            //ÀÛ¾÷ ¸®½ºÆ®¿¡¼­ °í¸¥ ÀÛ¾÷À» ¿¹¾àÇßÀ½À» Ç¥½Ã
            for (auto Iter = LoggingSet.begin(); Iter != LoggingSet.end();)
            {
                if ((*Iter).pObj == m_pTarget)
                {
                    //»èÁ¦ ÈÄ
                    Iter = LoggingSet.erase(Iter);
                    //¼öÁ¤ÇØ¼­ Ãß°¡
                    TASK tTask;
                    tTask.pObj = m_pTarget;
                    tTask.pRimReserved = this;
                    LoggingSet.emplace(tTask);
                }
                else
                {
                    ++Iter;
                }
            }

            m_bNavigating = true;
            Change_State(MOVETOWORK, _tTask.pObj);
            //Set_Target(_tTask.pObj);
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
            continue;
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

CObj* CRim::Find_Item(const TCHAR* _pImgKey)
{
    CObj* pItem(nullptr);
    //¾ÆÀÌÅÛ ¸®½ºÆ®¿¡¼­ Ã£°íÀÚ ÇÏ´Â ¾ÆÀÌÅÛÀ» Ã£¾Æ¶ó
    for (CObj* pObj : CObjMgr::Get_Instance()->Get_List()[OBJ_ITEM])
    {
        if (lstrcmp(pObj->Get_ImgKey(), _pImgKey))
        {
            continue;
        }

        //ÁÖÀÎÀÌ ÀÖÀ¸¸é
        if (pObj->Get_Target())
        {
            continue;
        }

        //Ã£°í ±× ¾ÆÀÌÅÛ±îÁö µµ´Þ ÇÒ ¼ö ÀÖ´Â ±æÀ» Ã£¾Æ¶ó.
        POS tStart{ (int)m_tInfo.fX, (int)m_tInfo.fY };
        POS tEnd{ (int)pObj->Get_Info().fX, (int)pObj->Get_Info().fY };

        list<CNode*> PathList = move(CPathFinder::Get_Instance()->Find_Path(tStart, tEnd));
        if (PathList.empty())
        {
            //¸ø Ã£À¸¸é ÄÁÆ¼´º
            continue;
        }
        else
        {
            //³ëµå µô¸®Æ®
            for_each(PathList.begin(), PathList.end(), Safe_Delete<CNode*>);
            PathList.clear();

            //±æÀ» Ã£¾ÒÀ¸¸é ±× ¾ÆÀÌÅÛ ¸®ÅÏ
            pItem = pObj;
            break;
        }
    }

    return pItem;
}

void CRim::PickUp_Item(CObj* _pObj)
{
    //ÇØ´çÅ¸ÀÏ¿¡ ¾ÆÀÌÅÛÀÌ ¾ø¾îÁ³´Ù°í ¾Ë¸²
    //POS tPos{ (int)m_pTarget->Get_Info().fX, (int)m_pTarget->Get_Info().fY };
    //CTileMgr::Get_Instance()->Set_TileObj(tPos, nullptr);

    m_pTransportingItem = _pObj;
    _pObj->Set_Target(this);
    _pObj = nullptr;
}

void CRim::PutDown_Item()
{
    if(!m_pTransportingItem)
    {
        return;
    }
    else
    {
        m_pTransportingItem->Set_Target(nullptr);
        m_pTransportingItem = nullptr;
    }

}

