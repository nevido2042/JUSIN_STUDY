#include "pch.h"
#include "Colony.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "TileMgr.h"
#include "PathFinder.h"
#include "TimeMgr.h"
#include "CSoundMgr.h"

CColony::CColony()
    :m_bEnemySpawned(false)
{
}

CColony::~CColony()
{
    Release();
}

void CColony::Initialize()
{
    CSoundMgr::Get_Instance()->Initialize();
    CSoundMgr::Get_Instance()->PlayBGM(L"Alpaca.wav", 0.5f);

    CTimeMgr::Get_Instance()->Initialize();

    //UI
    //메뉴 버튼
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ButtonSubtleAtlas_Menu.bmp", L"ButtonSubtleAtlas_Menu");
    //구상 버튼
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ButtonSubtleAtlas_Architect.bmp", L"ButtonSubtleAtlas_Architect");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ButtonSubtleAtlas_Command.bmp", L"ButtonSubtleAtlas_Command");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Deconstruct.bmp", L"Deconstruct");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ButtonSubtleAtlas_Structure.bmp", L"ButtonSubtleAtlas_Structure");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/RockSmooth_MenuIcon.bmp", L"RockSmooth_MenuIcon");
    //게임 스피드 버튼
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/TimeSpeedButton_Normal.bmp", L"TimeSpeedButton_Normal");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/TimeSpeedButton_Fast.bmp", L"TimeSpeedButton_Fast");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/TimeSpeedButton_Superfast.bmp", L"TimeSpeedButton_Superfast");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/TimeSpeedButton_Pause.bmp", L"TimeSpeedButton_Pause");
    //Designations
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Designations/Deconstruct.bmp", L"Deconstruct_mini");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Designations/RockSmooth_MenuIcon.bmp", L"RockSmooth_MenuIcon_mini");


    //Rim
    //얼굴
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Head/Male_Average_Normal_south.bmp", L"Male_Average_Normal_south");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Head/Male_Average_Normal_east.bmp", L"Male_Average_Normal_east");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Head/Male_Average_Normal_north.bmp", L"Male_Average_Normal_north");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Head/Male_Average_Normal_west.bmp", L"Male_Average_Normal_west");
    //몸통
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Body/Naked_Male_south.bmp", L"Naked_Male_south");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Body/Naked_Male_north.bmp", L"Naked_Male_north");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Body/Naked_Male_east.bmp", L"Naked_Male_east");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Body/Naked_Male_west.bmp", L"Naked_Male_west");
    //머리
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Hair/Afro_south.bmp", L"Afro_south");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Hair/Afro_east.bmp", L"Afro_east");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Hair/Afro_north.bmp", L"Afro_north");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Hair/Afro_west.bmp", L"Afro_west");

    //볼트액션 소총
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Weapon/BoltActionRifle.bmp", L"BoltActionRifle");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Weapon/Bullet_Small.bmp", L"Bullet_Small");

    //Centipede
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Mechanoid/Centipede_east.bmp", L"Centipede_east");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Mechanoid/Centipede_north.bmp", L"Centipede_north");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Mechanoid/Centipede_south.bmp", L"Centipede_south");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Mechanoid/Centipede_west.bmp", L"Centipede_west");
    //나무
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Tree/TreePoplar_LeaflessA.bmp", L"TreePoplar_LeaflessA");

    //타일
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Terrain/Ice.bmp", L"Ice");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Terrain/Gravel.bmp", L"Gravel");
    //돌
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Building/Linked/Wall_Atlas_Smooth.bmp", L"Wall_Atlas_Smooth");
    //핏자국
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Mote/Blood.bmp", L"Blood");
    //지네 무기
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Weapon/ChargeBlasterLight.bmp", L"ChargeBlasterLight");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Weapon/Charge_Small.bmp", L"Charge_Small");

    CObj* pObj(nullptr);
    //UI
    pObj = CAbstractFactory<CMenuBtn>::Create();
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pObj);
    pObj = CAbstractFactory<CArcitectBtn>::Create();
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pObj);

    //게임 스피드 버튼
    pObj = CAbstractFactory<CGameSpeedBtn>::Create(WINCX - 224, WINCY - 80);
    pObj->Set_ImgKey(L"TimeSpeedButton_Pause");
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pObj);
    pObj = CAbstractFactory<CGameSpeedBtn>::Create(WINCX - 160, WINCY - 80);
    pObj->Set_ImgKey(L"TimeSpeedButton_Normal");
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pObj);
    pObj = CAbstractFactory<CGameSpeedBtn>::Create(WINCX - 96, WINCY - 80);
    pObj->Set_ImgKey(L"TimeSpeedButton_Fast");
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pObj);
    pObj = CAbstractFactory<CGameSpeedBtn>::Create(WINCX - 32, WINCY - 80);
    pObj->Set_ImgKey(L"TimeSpeedButton_Superfast");
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pObj);

    //맵
    CTileMgr::Get_Instance()->Initialize();

    bool bVisitArray[TILEX * TILEY];
    memset(bVisitArray, false, sizeof(bVisitArray));

    for (int i = 0; i < (TILEX * TILEY) / 10; ++i)
    {
        int iX = int(rand() % TILEX);
        int iY = int(rand() % TILEY);

        //안 나왔던 놈이 나올 때까지 뽑는다.
        while (bVisitArray[iX + TILEX * iY])
        {
            iX = int(rand() % TILEX);
            iY = int(rand() % TILEY);
        }

        bVisitArray[iX + TILEX * iY] = true;

        POS tPos{ float(64 * iX + 32), float(64 * iY + 32) };

        pObj = CAbstractFactory<CSteelWall>::Create(tPos);
        CObjMgr::Get_Instance()->Add_Object(OBJ_WALL, pObj);
        CTileMgr::Get_Instance()->Set_TileOption(tPos, OPT_BLOCKED);
        CTileMgr::Get_Instance()->Set_TileObj(tPos, pObj);
    }

    //림 두마리
    for (int i = 0; i < 2; ++i)
    {
        int iX = int(rand() % TILEX);
        int iY = int(rand() % TILEY);

        //안 나왔던 놈이 나올 때까지 뽑는다.
        while (bVisitArray[iX + TILEX * iY])
        {
            iX = int(rand() % TILEX);
            iY = int(rand() % TILEY);
        }

        bVisitArray[iX + TILEX * iY] = true;

        POS tPos{ float(64 * iX + 32), float(64 * iY + 32) };

        pObj = CAbstractFactory<CRim>::Create(tPos);
        CObjMgr::Get_Instance()->Add_Object(OBJ_RIM, pObj);
    }

    ////지네로봇 두마리
    //for (int i = 0; i < 2; ++i)
    //{
    //    int iX = int(rand() % TILEX);
    //    int iY = int(rand() % TILEY);

    //    //안 나왔던 놈이 나올 때까지 뽑는다.
    //    while (bVisitArray[iX + TILEX * iY])
    //    {
    //        iX = int(rand() % TILEX);
    //        iY = int(rand() % TILEY);
    //    }

    //    bVisitArray[iX + TILEX * iY] = true;

    //    POS tPos{ float(64 * iX + 32), float(64 * iY + 32) };

    //    //지네
    //    pObj = CAbstractFactory<CCentipede>::Create(tPos);
    //    CObjMgr::Get_Instance()->Add_Object(OBJ_MECHANOID, pObj);
    //}

    ////나무
    //for (int i = 0; i < (TILEX * TILEY) / 10; ++i)
    //{
    //    int iX = int(rand() % TILEX);
    //    int iY = int(rand() % TILEY);

    //    //안 나왔던 놈이 나올 때까지 뽑는다.
    //    while (bVisitArray[iX + TILEX * iY])
    //    {
    //        iX = int(rand() % TILEX);
    //        iY = int(rand() % TILEY);
    //    }

    //    bVisitArray[iX + TILEX * iY] = true;

    //    POS tPos{ float(64 * iX + 32), float(64 * iY + 32) };

    //    //지네
    //    pObj = CAbstractFactory<CTree>::Create(tPos);
    //    CObjMgr::Get_Instance()->Add_Object(OBJ_TREE, pObj);
    //}

}

int CColony::Update()
{
    CColonyMgr::Get_Instance()->Update();

    CObjMgr::Get_Instance()->Update();

    CTileMgr::Get_Instance()->Update();

    CTimeMgr::Get_Instance()->Update();

    //몇 초 후 적 생성
    if (!m_bEnemySpawned && CTimeMgr::Get_Instance()->Get_ElapsedTime() > 60.f )
    {

        ////지네로봇 두마리
        for (int i = 0; i < 1; ++i)
        {
            int iX = int(rand() % TILEX);
            int iY = 0;

            POS tPos{ float(64 * iX + 32), float(64 * iY + 32) };

            //지네
            CObj* pObj = CAbstractFactory<CCentipede>::Create(tPos);
            CObjMgr::Get_Instance()->Add_Object(OBJ_MECHANOID, pObj);
        }

        m_bEnemySpawned = true;
    }


    return OBJ_NOEVENT;
}

void CColony::Late_Update()
{
    CObjMgr::Get_Instance()->Late_Update();

    CTileMgr::Get_Instance()->Late_Update();

    CColonyMgr::Get_Instance()->Late_Update();
}

void CColony::Render(HDC hDC)
{
    // 검은 배경 (Black 이미지를 바로 hDC에 그리기)
    HDC hBlackDC = CBmpMgr::Get_Instance()->Find_Image(L"Black");
    BitBlt(hDC, 0, 0, WINCX, WINCY, hBlackDC, 0, 0, SRCCOPY);  // 검은 배경 그리기

    CTileMgr::Get_Instance()->Render(hDC);

    // 객체 매니저 렌더링
    CObjMgr::Get_Instance()->Render(hDC);

    CColonyMgr::Get_Instance()->Render(hDC);

    CTimeMgr::Get_Instance()->Render(hDC);
}

void CColony::Release()
{
    CObjMgr::Get_Instance()->Delete_ID(OBJ_UI);
    CTileMgr::Destroy_Instance();
    CColonyMgr::Destroy_Instance();
    CPathFinder::Destroy_Instance();
    CTimeMgr::Destroy_Instance();
    CSoundMgr::Destroy_Instance();
}
