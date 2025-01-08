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
#include "Campfire.h"
#include "Camera.h"

CRim::CRim()
    :m_bTaskCheck(false), m_pTransportingItem(nullptr),
    m_fTaskCheckTime(0.f), m_fTaskCheckTime_Elapsed(0.f),
    m_eDraftState(DRAFTED_END)
{

}

CRim::~CRim()
{
    Release();
}

void CRim::Initialize()
{
    CPawn::Initialize();

    m_fTaskCheckTime = 100.f;

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
}

void CRim::Render(HDC hDC)
{

    CPawn::Render(hDC);

    /*Ellipse(hDC, m_tRect.left + iScrollX, m_tRect.top + iScrollY,
        m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);*/

    // ¸² ¸öÅë, ¾ó±¼, ¸Ó¸® ¼ø
    HDC hTestDC(nullptr);


    switch (m_eDir) 
    {
    case UU:
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Body[NORTH], IMAGE_OFFSET_X, IMAGE_OFFSET_Y); // ¸öÅë
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Face[NORTH], IMAGE_OFFSET_X, HEAD_OFFSET + IMAGE_OFFSET_Y); // ¾ó±¼
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Hair[NORTH], IMAGE_OFFSET_X, HEAD_OFFSET + IMAGE_OFFSET_Y); // ¸Ó¸®
        break;

    case RR:
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Body[EAST], IMAGE_OFFSET_X, IMAGE_OFFSET_Y); // ¸öÅë
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Face[EAST], IMAGE_OFFSET_X, HEAD_OFFSET + IMAGE_OFFSET_Y); // ¾ó±¼
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Hair[EAST], IMAGE_OFFSET_X, HEAD_OFFSET + IMAGE_OFFSET_Y); // ¸Ó¸®
        break;

    case DD:
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Body[SOUTH], IMAGE_OFFSET_X, IMAGE_OFFSET_Y); // ¸öÅë
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Face[SOUTH], IMAGE_OFFSET_X, HEAD_OFFSET + IMAGE_OFFSET_Y); // ¾ó±¼
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Hair[SOUTH], IMAGE_OFFSET_X, HEAD_OFFSET + IMAGE_OFFSET_Y); // ¸Ó¸®
        break;

    case LL:
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Body[WEST], IMAGE_OFFSET_X, IMAGE_OFFSET_Y); // ¸öÅë
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Face[WEST], IMAGE_OFFSET_X, HEAD_OFFSET + IMAGE_OFFSET_Y); // ¾ó±¼
        DrawImage(hDC, m_tRect, m_ImgKeyArr_Hair[WEST], IMAGE_OFFSET_X, HEAD_OFFSET + IMAGE_OFFSET_Y); // ¸Ó¸®
        break;

    default:
        break;
    }


    //Á×¾úÀ» ¶§ ÇÍÀÚ±¹ Ãâ·Â
    if (m_bDead)
    {
        POINT tWorldPoint = CCamera::Get_Instance()->WorldToScreen(float(m_tRect.left), float(m_tRect.top));
        float fZoom = CCamera::Get_Instance()->Get_Zoom();
        hTestDC = CBmpMgr::Get_Instance()->Find_Image(L"Blood");
        GdiTransparentBlt(hDC,
            tWorldPoint.x,// m_tRect.left + iScrollX,
            tWorldPoint.y,//m_tRect.top + iScrollY,
            int(64* fZoom), int(64* fZoom),
            hTestDC, 0, 0, 64, 64,
            RGB_WHITE);
    }
}

void CRim::Handle_Wandering()
{
    m_fTaskCheckTime_Elapsed += GAMESPEED;

    if (m_fTaskCheckTime < m_fTaskCheckTime_Elapsed)
    {
        m_bTaskCheck = true;
        m_fTaskCheckTime_Elapsed = 0.f;
    }

    //ÅÛ µé°í ÀÖÁö ¾Êµµ·Ï
    if (m_pTransportingItem)
    {
        PutDown_Item();
    }

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
        //°Ç¼³ÀÌ ¾Õ¿¡ ¼­¸é, Àç·á°¡ ¾ø¾îµµ transport »óÅÂ°¡ µÇ¼­ ÇØÃ¼(?), ¹ú¸ñÀ» ¾ÈÇÑ´Ù
        Check_ConstructWork();
        //¼ø¼­°¡ Áß¿äÇÔ
        Check_DeconstructWork();
        Check_LoggingWork();

        //Check_TransportingWork();
        //m_fElapsedTimeCheck = 0.f;
        m_bTaskCheck = false;
    }
}

void CRim::Handle_Drafted()
{
    //Àû Å½»ö »óÅÂ
    //Àû Á¶ÁØ »óÅÂ
    //Àû »ç°Ý °¡´É »óÅÂ

    switch (m_eDraftState)
    {
    case CRim::DRAFTED_SEARCH:
        Drfated_Search();
        break;
    case CRim::DRAFHTED_AIM:
        Drfated_Aim();
        break;
    case CRim::DRAFTED_FIRE:
        Drfated_Fire();
        break;
    case CRim::DRAFTED_END:
        break;
    default:
        break;
    }


    //if (m_pTarget)
    //{
    //    //»çÁ¤°Å¸®¿¡ ÀÖ°í ÀûÀÌ º¸ÀÎ´Ù¸é
    //    if (IsWithinRange() && IsCanSeeTarget())
    //    {
    //        static_cast<CRangedWeapon*>(m_pRangedWeapon)->Fire();
    //        m_bAttack = true;
    //    }
    //    else
    //    {
    //        Set_Target(nullptr);
    //        m_bAttack = false;
    //    }
    //}

    //if (m_pTarget)
    //{
    //    //Å¸°ÙÀÌ Á×À¸¸é Å¸°Ù ¾ø¾Ö±â
    //    CPawn* pTarget = static_cast<CPawn*>(m_pTarget);
    //    if (pTarget->Get_IsDead())
    //    {
    //        Set_Target(nullptr);
    //        //ÀûÀÌ Á×¾úÀ¸¸é ´Ù¸¥ ÀûÀ» Ã£´Â´Ù.
    //        //Find_Enemy();
    //    }
    //}
    //else
    //{
    //    if (!m_bNavigating)
    //    {
    //        Find_Enemy();
    //    }
    //}

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

    //ÇØÃ¼ ÁøÇàÁßÀÎ ¹Ù »ý¼º
    //¸îÃÊ µÚ ÇØÃ¼ ¿Ï·á
}

void CRim::Handle_Transporting()
{
    //Ã³À½µé¾î¿Ã¶§ Å¸°ÙÀÌ ¾ø´Â°Ô Á¤»ó
    //Å¸°Ù ¾ÆÀÌÅÛÀÌ °¡±î¿öÁö¸é ¾ÆÀÌÅÛÀ» µé¾î¶ó
    if (m_pTarget)
    {
        if (!m_pTransportingItem && m_fTargetDist < TILECX * 1.5f)
        {

            PickUp_Item(m_pTarget);

        }
    }

    //ÇöÀç °Ç¼³ÀÛ¾÷ÀÌ ¹ºÁö ÆÄ¾ÇÇÏ°í, ¾î¶² Àç·á¸¦ °¡Á®°¡¾ßÇÒÁö ÆÇ´Ü
    if (!m_pTarget)
    {
        CObj* pItem(nullptr);

        if (m_eCurrentTask.eType == TASK::CAMPFIRE)
        {
            //³ª¹«
            if (!m_pTransportingItem)
            {
                pItem = Find_Item(L"WoodLog_b");

                if (pItem)
                {
                    Set_Target(pItem);
                }
                else
                {
                    //m_bTaskCheck = true;
                    Change_State(WANDERING);
                    return;
                }
            }
        }
        else
        {
            //Ã¶
            if (!m_pTransportingItem)
            {
                pItem = Find_Item(L"Steel_b");

                if (pItem)
                {
                    Set_Target(pItem);
                }
                else
                {
                    m_bTaskCheck = true;
                    Change_State(WANDERING);
                    return;
                }
            }
        }
    }

    if (m_pTarget && !m_bNavigating)
    {
        POS tPos{ (int)m_pTarget->Get_Info().fX, (int)m_pTarget->Get_Info().fY };
        Move_To(tPos);
    }


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

    //¾ÆÀÌÅÛÀ» µé¾úÀ¸¸é °Ç¼³ÇÏ·¯°¡¶ó
    if (m_pTransportingItem)
    {
        Change_State(MOVETOWORK, m_eCurrentTask.pObj);
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
        if (!m_bNavigating)
        {
            //±âÁ¸ ³ëµå µô¸®Æ®
            for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
            m_NodeList.clear();

            //ÀÌµ¿ °¡´ÉÇÑ Å¸ÀÏÀÌ ÀÖÀ¸¸é ³ëµå¸®½ºÆ® ¹ÝÈ¯
            m_NodeList = move(CTileMgr::Get_Instance()
                ->Find_ReachableTiles(POS{ (int)m_tInfo.fX, (int)m_tInfo.fY }, POS{ (int)m_pTarget->Get_Info().fX, (int)m_pTarget->Get_Info().fY }));

            if (m_NodeList.empty())
            {
                return;
            }

            m_bNavigating = true;
        }

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
    else
    {
        //´Ù¸¥ ÀÛ¾÷ Ã¼Å©
        m_bTaskCheck = true;
        Change_State(WANDERING);
        return;
    }

    //Å¸°ÙÀÌ °¡±î¿îÁö È®ÀÎ
    if (m_fTargetDist < TILECX * 1.5f)
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
        else if (m_pTarget->Get_ObjID() == OBJ_TILE)
        {
            Change_State(CONSTRUCTING, m_pTarget);
        }
    }
}

void CRim::Change_State(STATE _eState, CObj* _pTarget)
{
    CPawn::Change_State(_eState, _pTarget);

    if (_eState == STATE::DRAFTED)
    {
        m_eDraftState = DRAFTED_SEARCH;
    }
}

void CRim::Change_DraftedState(DRAFTED_STATE _eState)
{
    if (m_eDraftState == _eState)
    {
        return;
    }

    m_eDraftState = _eState;
}

void CRim::Drfated_Search()
{
     Find_Enemy();

     if (m_pTarget)
     {
         Change_DraftedState(DRAFHTED_AIM);
     }
}

void CRim::Drfated_Aim()
{
    if (m_pTarget)
    {
        //Å¸°ÙÀÌ Á×À¸¸é Å¸°Ù ¾ø¾Ö±â
        CPawn* pTarget = static_cast<CPawn*>(m_pTarget);
        if (pTarget->Get_IsDead())
        {
            Set_Target(nullptr);
            //ÀûÀÌ Á×¾úÀ¸¸é ´Ù¸¥ ÀûÀ» Ã£´Â´Ù.
            Change_DraftedState(DRAFTED_SEARCH);
        }
    }


    if (m_pTarget)
    {
        //»çÁ¤°Å¸®¿¡ ÀÖ°í ÀûÀÌ º¸ÀÎ´Ù¸é
        if (IsWithinRange() && IsCanSeeTarget())
        {
            Change_DraftedState(DRAFTED_FIRE);
        }
    }
    else
    {
        Change_DraftedState(DRAFTED_SEARCH);
    }
}

void CRim::Drfated_Fire()
{
    if (m_bNavigating)
    {
        m_bAttack = false;
        return;
    }

    if (m_pTarget)
    {
        //Å¸°ÙÀÌ Á×À¸¸é Å¸°Ù ¾ø¾Ö±â
        CPawn* pTarget = static_cast<CPawn*>(m_pTarget);
        if (pTarget->Get_IsDead())
        {
            Set_Target(nullptr);
            //ÀûÀÌ Á×¾úÀ¸¸é ´Ù¸¥ ÀûÀ» Ã£´Â´Ù.
            m_bAttack = false;
            Change_DraftedState(DRAFTED_SEARCH);
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
        m_bAttack = false;
        Change_DraftedState(DRAFTED_SEARCH);
    }
}

void CRim::DrawImage(HDC hDC, const RECT& m_tRect, const std::wstring& imageKey, int offsetX, int offsetY)
{

    POINT tPoint = CCamera::Get_Instance()->WorldToScreen((float)m_tRect.left, (float)m_tRect.top);

    // ÀÌ¹ÌÁö ±×¸®±â ÇÔ¼ö
        HDC hTestDC = CBmpMgr::Get_Instance()->Find_Image(imageKey.c_str());
        GdiTransparentBlt(hDC,
            tPoint.x - int(offsetX * CCamera::Get_Instance()->Get_Zoom()),
            tPoint.y - int(offsetY * CCamera::Get_Instance()->Get_Zoom()),
            int(128 * CCamera::Get_Instance()->Get_Zoom()), int(128 * CCamera::Get_Instance()->Get_Zoom()),
            hTestDC, 0, 0, 128, 128,
            RGB_PURPLE);
    
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

    //ÀÌ¹Ì Å¸ÀÏ À§¿¡ ¹¹°¡ ÀÖÀ¸¸é Ãë¼Ò ½ÃÅ°±â
    if (CTileMgr::Get_Instance()->Get_TileObj(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY))
    {
        m_bTaskCheck = true;
        PutDown_Item();
        Change_State(WANDERING);
        return;
    }

    //ÇØÃ¼ ½Ã°£ °É¸®°Ô ÇÏ°í »ç¿îµå Ãâ·Â
    if (m_fConstructTime > m_fConstructElapsed)
    {
        switch (m_eCurrentTask.eType)
        {
        case TASK::TYPE::CAMPFIRE:
            //³ª¹«¼Ò¸® ³ª°Ô
            //CSoundMgr::Get_Instance()->StopSound(SOUND_CONSTRUCT_WOOD);
            CSoundMgr::Get_Instance()->PlaySound(L"Hammer_Nail_Wood_1a.wav", SOUND_CONSTRUCT_WOOD, .5f);
            break;
        case TASK::TYPE::WALL:
        case TASK::TYPE::SHIP:
            //µå¸±¼Ò¸® ³ª°Ô
            //CSoundMgr::Get_Instance()->StopSound(SOUND_CONSTRUCT_STEEL);
            CSoundMgr::Get_Instance()->PlaySound(L"DrillB.wav", SOUND_CONSTRUCT_STEEL, .5f);
            break;
        }


        m_fConstructElapsed += GAMESPEED;
        return;
    }

    m_fConstructElapsed = 0.f;

    CSoundMgr::Get_Instance()->StopSound(SOUND_CONSTRUCT_FINISH);
    CSoundMgr::Get_Instance()->PlaySound(L"BuildingComplete.wav", SOUND_CONSTRUCT_FINISH, .5f);

    //µé°í ÀÖ´Â Ã¶À» »èÁ¦
    m_pTransportingItem->Set_Destroyed();
    m_pTransportingItem = nullptr;

    if (m_eCurrentTask.eType == TASK::SHIP)
    {
        if (CTutorialMgr::Get_Instance()->Get_CurQuest() == CTutorialMgr::QUEST_CONSTRUCT_SHIP)
        {
            CTutorialMgr::Get_Instance()->IncreaseQuestProgress();
        }

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
        if (CTutorialMgr::Get_Instance()->Get_CurQuest() == CTutorialMgr::QUEST::QUEST_CONSTRUCT_WALL)
        {
            CTutorialMgr::Get_Instance()->IncreaseQuestProgress();
        }

        //º® °Ç¼³
        CObj* pObj = CAbstractFactory<CSteelWall>::Create(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY);
        CObjMgr::Get_Instance()->Add_Object(OBJ_WALL, pObj);
        CTileMgr::Get_Instance()->Set_TileOption(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY, OPT_BLOCKED);
        CTileMgr::Get_Instance()->Set_TileObj(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY, pObj);
    }
    else if (m_eCurrentTask.eType == TASK::CAMPFIRE)
    {
        if (CTutorialMgr::Get_Instance()->Get_CurQuest() == CTutorialMgr::QUEST::QUEST_CONSTRUCT_CAMPFIRE)
        {
            CTutorialMgr::Get_Instance()->IncreaseQuestProgress();
        }

        //¸ð´ÚºÒ °Ç¼³
        CObj* pObj = CAbstractFactory<CCampfire>::Create(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY);
        CObjMgr::Get_Instance()->Add_Object(OBJ_CAMPFIRE, pObj);
        CTileMgr::Get_Instance()->Set_TileObj(m_pTarget->Get_Info().fX, m_pTarget->Get_Info().fY, pObj);
    }

    //ÀÛ¾÷»èÁ¦
    //CColonyMgr::Get_Instance()->Get_ConstructSet()->erase(m_pTarget);
    set<TASK>& ConstructSet = *CColonyMgr::Get_Instance()->Get_ConstructSet();
    for (auto Iter = ConstructSet.begin(); Iter != ConstructSet.end();)
    {
        if ((*Iter).pObj == m_pTarget)
        {
            if (CTile* pTile = dynamic_cast<CTile*>(Iter->pObj))
            {
                pTile->Set_Reserved(CTile::RESERVED_END);
            }

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
            ////±âÁ¸ ³ëµå µô¸®Æ®
            //for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
            //m_NodeList.clear();

            ////ÀÌµ¿ °¡´ÉÇÑ Å¸ÀÏÀÌ ÀÖÀ¸¸é ³ëµå¸®½ºÆ® ¹ÝÈ¯
            //m_NodeList = move(CTileMgr::Get_Instance()
            //    ->Find_ReachableTiles(POS{ (int)m_tInfo.fX, (int)m_tInfo.fY }, POS{ (int)_tTask.pObj->Get_Info().fX, (int)_tTask.pObj->Get_Info().fY }));

            //if (m_NodeList.empty())
            //{
            //    continue;
            //}

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
            Change_State(MOVETOWORK, _tTask.pObj);
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
            //for_each(m_NodeList.begin(), m_NodeList.end(), Safe_Delete<CNode*>);
            //m_NodeList.clear();

            //ÀÌµ¿ °¡´ÉÇÑ Å¸ÀÏÀÌ ÀÖÀ¸¸é ³ëµå¸®½ºÆ® ¹ÝÈ¯
            list<CNode*> NodeList = move(CTileMgr::Get_Instance()
                ->Find_ReachableTiles(POS{ (int)m_tInfo.fX, (int)m_tInfo.fY }, POS{ (int)tTask.pObj->Get_Info().fX, (int)tTask.pObj->Get_Info().fY }));

            if (NodeList.empty())
            {
                continue;
            }

            for_each(NodeList.begin(), NodeList.end(), Safe_Delete<CNode*>);
            NodeList.clear();

            //Àç·á°¡ ÀÖ´ÂÁö È®ÀÎ
            CObj* pItem(nullptr);
            switch (tTask.eType)
            {
            case TASK::CAMPFIRE:
                pItem = Find_Item(L"WoodLog_b");
                break;
            case TASK::SHIP:
                pItem = Find_Item(L"Steel_b");
                break;
            case TASK::WALL:
                pItem = Find_Item(L"Steel_b");
                break;
            default:
                break;
            }

            if (!pItem)
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
            Change_State(TRANSPORTING);
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
    CObj* pClosestItem = nullptr;
    float fMinDistance = FLT_MAX; // ÃÊ±â ÃÖ¼Ò °Å¸® °ªÀ» ¸Å¿ì Å« °ªÀ¸·Î ¼³Á¤

    // ¾ÆÀÌÅÛ ¸®½ºÆ®¿¡¼­ °¡Àå °¡±î¿î ¾ÆÀÌÅÛÀ» Ã£´Â´Ù.
    for (CObj* pObj : CObjMgr::Get_Instance()->Get_List()[OBJ_ITEM])
    {
        // ÀÌ¹ÌÁö Å°°¡ ÀÏÄ¡ÇÏÁö ¾ÊÀ¸¸é °Ç³Ê¶Ü
        if (lstrcmp(pObj->Get_ImgKey(), _pImgKey))
        {
            continue;
        }

        // ÁÖÀÎÀÌ ÀÖ´Â ¾ÆÀÌÅÛÀº °Ç³Ê¶Ü
        if (pObj->Get_Target())
        {
            continue;
        }

        // ÇöÀç À§Ä¡¿Í ¾ÆÀÌÅÛ À§Ä¡¸¦ °¡Á®¿È
        POS tStart{ (int)m_tInfo.fX, (int)m_tInfo.fY };
        POS tEnd{ (int)pObj->Get_Info().fX, (int)pObj->Get_Info().fY };

        // °æ·Î¸¦ Ã£À½
        list<CNode*> PathList = move(CPathFinder::Get_Instance()->Find_Path(tStart, tEnd));

        // °æ·Î°¡ ¾øÀ¸¸é °Ç³Ê¶Ü
        if (PathList.empty())
        {
            continue;
        }

        // ³ëµå¸¦ ¸ðµÎ »èÁ¦
        for_each(PathList.begin(), PathList.end(), Safe_Delete<CNode*>);
        PathList.clear();

        // °Å¸® °è»ê (ÇÇÅ¸°í¶ó½º °ø½Ä)
        float fDistance = sqrtf(powf(float(tEnd.iX - tStart.iX), 2.f) + powf(float(tEnd.iY - tStart.iY), 2.f));

        // °¡Àå °¡±î¿î ¾ÆÀÌÅÛÀ» ¼±ÅÃ
        if (fDistance < fMinDistance)
        {
            fMinDistance = fDistance;
            pClosestItem = pObj;
        }
    }

    return pClosestItem;

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

