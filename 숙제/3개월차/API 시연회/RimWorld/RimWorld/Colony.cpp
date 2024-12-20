#include "pch.h"
#include "Colony.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "TileMgr.h"
#include "PathFinder.h"
#include "TimeMgr.h"
#include "SoundMgr.h"

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
    CSoundMgr::Get_Instance()->PlayBGM(L"Alpaca.wav", 0.3f);

    CTimeMgr::Get_Instance()->Initialize();



    CObj* pObj;

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
    //CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Weapon/BoltActionRifle.bmp", L"BoltActionRifle");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Weapon/Bullet_Small.bmp", L"Bullet_Small");

    //지네 무기
    //CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Weapon/ChargeBlasterLight.bmp", L"ChargeBlasterLight");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Weapon/Charge_Small.bmp", L"Charge_Small");

    //Centipede
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Mechanoid/Centipede_east.bmp", L"Centipede_east");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Mechanoid/Centipede_north.bmp", L"Centipede_north");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Mechanoid/Centipede_south.bmp", L"Centipede_south");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Mechanoid/Centipede_west.bmp", L"Centipede_west");
    //Boomrat
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Animal/Boomrat_east.bmp", L"Boomrat_east");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Animal/Boomrat_north.bmp", L"Boomrat_north");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Animal/Boomrat_south.bmp", L"Boomrat_south");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Animal/Boomrat_west.bmp", L"Boomrat_west");
    //나무
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Tree/TreePoplar_LeaflessA.bmp", L"TreePoplar_LeaflessA");

    //타일
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Terrain/Ice.bmp", L"Ice");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Terrain/Gravel.bmp", L"Gravel");
    //돌
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Building/Linked/Wall_Atlas_Smooth.bmp", L"Wall_Atlas_Smooth");
    //핏자국
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Mote/Blood.bmp", L"Blood");

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

    Create_UI();
}

int CColony::Update()
{
    CColonyMgr::Get_Instance()->Update();

    CObjMgr::Get_Instance()->Update();

    CTileMgr::Get_Instance()->Update();

    CTimeMgr::Get_Instance()->Update();

    //몇 초 후 적 생성
    if (!m_bEnemySpawned && CTimeMgr::Get_Instance()->Get_ElapsedTime() > 30.f )
    {

        ////지네로봇
        for (int i = 0; i < 1; ++i)
        {
            int iX = int(rand() % TILEX);
            int iY = 0;

            POS tPos{ float(64 * iX + 32), float(64 * iY + 32) };

            //지네
            CObj* pObj = CAbstractFactory<CCentipede>::Create(tPos);
            CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, pObj);
        }

        ////폭탄쥐
        for (int i = 0; i < 10; ++i)
        {
            int iX = int(rand() % TILEX);
            int iY = 0;

            POS tPos{ float(64 * iX + 32), float(64 * iY + 32) };

            CObj* pObj = CAbstractFactory<CBoomrat>::Create(tPos);
            CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, pObj);
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
}

void CColony::Create_UI()
{
    //UI
    //메뉴 버튼
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ButtonSubtleAtlas_Menu.bmp", L"ButtonSubtleAtlas_Menu");
    //구상 버튼
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ButtonSubtleAtlas_Architect.bmp", L"ButtonSubtleAtlas_Architect");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ButtonSubtleAtlas_Command.bmp", L"ButtonSubtleAtlas_Command");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Deconstruct.bmp", L"Deconstruct");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ButtonSubtleAtlas_Structure.bmp", L"ButtonSubtleAtlas_Structure");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Construct.bmp", L"Construct");
    //게임 스피드 버튼
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/TimeSpeedButton_Normal.bmp", L"TimeSpeedButton_Normal");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/TimeSpeedButton_Fast.bmp", L"TimeSpeedButton_Fast");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/TimeSpeedButton_Superfast.bmp", L"TimeSpeedButton_Superfast");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/TimeSpeedButton_Pause.bmp", L"TimeSpeedButton_Pause");
    //Designations
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Designations/Deconstruct.bmp", L"Deconstruct_mini");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Designations/RockSmooth_MenuIcon.bmp", L"RockSmooth_MenuIcon_mini");
    //림 슬롯
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/DesButBG.bmp", L"DesButBG");
    //상세정보창
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/DetailViewBG.bmp", L"DetailViewBG");

    //상세정보창
    CObj* pDetailView = CAbstractFactory<CDetailView>::Create(200, WINCY - 107);
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pDetailView);

    //림 슬롯 매니저
    CObj* pRimSlotMgr = CAbstractFactory<CRimSlotMgr>::Create(0, 0);
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pRimSlotMgr);

    float fLongBtnCX = 128.f;
    float fLongBtnCY = 32.f;
    float fShortBtnCX = 64.f;
    float fShortBtnCY = 64.f;
    //구상 버튼

    CObj* pArchitectBtn = CAbstractFactory<CMyButton>::
        Create(fLongBtnCX * 0.5f, WINCY - fLongBtnCY * 0.5f);
    pArchitectBtn->Set_Size(fLongBtnCX, fLongBtnCY);
    pArchitectBtn->Set_Activate(true);
    pArchitectBtn->Set_ImgKey(L"ButtonSubtleAtlas_Architect");
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pArchitectBtn);

    //명령 버튼
    CObj* pCommandBtn = CAbstractFactory<CMyButton>::
        Create(fLongBtnCX * 0.5f, WINCY - fLongBtnCY * 1.5f);
    pCommandBtn->Set_Size(fLongBtnCX, fLongBtnCY);
    pCommandBtn->Set_ImgKey(L"ButtonSubtleAtlas_Command");
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pCommandBtn);
    pArchitectBtn->Get_ChildList()->push_back(pCommandBtn);
    pCommandBtn->Set_Parent(pArchitectBtn);

    //구조물 버튼
    CObj* pStructureBtn = CAbstractFactory<CMyButton>::
        Create(fLongBtnCX * 0.5f, WINCY - fLongBtnCY * 2.5f);
    pStructureBtn->Set_Size(fLongBtnCX, fLongBtnCY);
    pStructureBtn->Set_ImgKey(L"ButtonSubtleAtlas_Structure");
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pStructureBtn);
    pArchitectBtn->Get_ChildList()->push_back(pStructureBtn);
    pStructureBtn->Set_Parent(pArchitectBtn);


    //해체 버튼
    CObj* pDeconstructBtn = CAbstractFactory<CMyButton>::
        Create(fShortBtnCX * 2.5f, WINCY - fShortBtnCY * 1.f);
    pDeconstructBtn->Set_Size(fShortBtnCX, fShortBtnCY);
    pDeconstructBtn->Set_ImgKey(L"Deconstruct");
    pCommandBtn->Get_ChildList()->push_back(pDeconstructBtn);
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pDeconstructBtn);


    //철벽 건설 버튼
    CObj* pConstruct = CAbstractFactory<CMyButton>::
        Create(fShortBtnCX * 2.5f, WINCY - fShortBtnCY * 1.f);
    pConstruct->Set_Size(fShortBtnCX, fShortBtnCY);
    pConstruct->Set_ImgKey(L"Construct");
    pStructureBtn->Get_ChildList()->push_back(pConstruct);
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pConstruct);

    //게임 스피드 버튼
    CObj* pObj(nullptr);
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

    //UI
    //pObj = CAbstractFactory<CMenuBtn>::Create();
    //CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pObj);
    //pObj = CAbstractFactory<CArcitectBtn>::Create();
    //CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pObj);
}
