#include "pch.h"
#include "Colony.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "TileMgr.h"
#include "PathFinder.h"
#include "TimeMgr.h"
#include "SoundMgr.h"
#include "EffectMgr.h"
#include "SceneMgr.h"
#include "ScrollMgr.h"

CColony::CColony()
    :/*m_bEnemySpawned(false),*/ m_fSpawnTime(0.f), m_iWaveIndex(0)//, m_bShipBtnActive(false)
{
    ZeroMemory(&WaveFuncs, sizeof(WaveFuncs));
}

CColony::~CColony()
{
    Release();
}

void CColony::Initialize()
{
    CSoundMgr::Get_Instance()->PlayBGM(L"Alpaca.wav", 0.3f);

    CTimeMgr::Get_Instance()->Initialize();

    WaveFuncs[0] = &CColony::Spawn_Wave1;
    WaveFuncs[1] = &CColony::Spawn_Wave2;
    WaveFuncs[2] = &CColony::Spawn_Wave3;

    m_fSpawnTime = 120.f;

    CObj* pObj;

    //Rim
    //얼굴
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Head/Male_Average_Normal_south.bmp", L"Male_Average_Normal_south");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Head/Male_Average_Normal_east.bmp", L"Male_Average_Normal_east");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Head/Male_Average_Normal_north.bmp", L"Male_Average_Normal_north");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Head/Male_Average_Normal_west.bmp", L"Male_Average_Normal_west");
    
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Head/Female_Average_Normal_east.bmp", L"Female_Average_Normal_east");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Head/Female_Average_Normal_north.bmp", L"Female_Average_Normal_north");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Head/Female_Average_Normal_west.bmp", L"Female_Average_Normal_west");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Head/Female_Average_Normal_south.bmp", L"Female_Average_Normal_south");
    //몸통
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Body/Naked_Male_south.bmp", L"Naked_Male_south");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Body/Naked_Male_north.bmp", L"Naked_Male_north");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Body/Naked_Male_east.bmp", L"Naked_Male_east");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Body/Naked_Male_west.bmp", L"Naked_Male_west");

    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Body/Naked_Female_south.bmp", L"Naked_Female_south");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Body/Naked_Female_north.bmp", L"Naked_Female_north");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Body/Naked_Female_east.bmp", L"Naked_Female_east");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Body/Naked_Female_west.bmp", L"Naked_Female_west");
    
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Body/Naked_Fat_south.bmp", L"Naked_Fat_south");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Body/Naked_Fat_north.bmp", L"Naked_Fat_north");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Body/Naked_Fat_east.bmp", L"Naked_Fat_east");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Body/Naked_Fat_west.bmp", L"Naked_Fat_west");

    //머리
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Hair/Afro_south.bmp", L"Afro_south");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Hair/Afro_east.bmp", L"Afro_east");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Hair/Afro_north.bmp", L"Afro_north");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Hair/Afro_west.bmp", L"Afro_west");

    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Hair/Decent_south.bmp", L"Decent_south");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Hair/Decent_east.bmp", L"Decent_east");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Hair/Decent_north.bmp", L"Decent_north");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Hair/Decent_west.bmp", L"Decent_west");

    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Hair/Pigtails_south.bmp", L"Pigtails_south");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Hair/Pigtails_east.bmp", L"Pigtails_east");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Hair/Pigtails_north.bmp", L"Pigtails_north");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Hair/Pigtails_west.bmp", L"Pigtails_west");

    //볼트액션 소총
    //CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Weapon/BoltActionRifle.bmp", L"BoltActionRifle");
    //CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Weapon/Bullet_Small.bmp", L"Bullet_Small");

    //지네 무기
    //CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Weapon/ChargeBlasterLight.bmp", L"ChargeBlasterLight");
    //CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Weapon/Charge_Small.bmp", L"Charge_Small");
    //랜서 무기
    //CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Weapon/Charge_Small.bmp", L"Charge_Small");

    //Centipede
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Mechanoid/Centipede_east.bmp", L"Centipede_east");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Mechanoid/Centipede_north.bmp", L"Centipede_north");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Mechanoid/Centipede_south.bmp", L"Centipede_south");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Mechanoid/Centipede_west.bmp", L"Centipede_west");
    //랜서
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Mechanoid/lancer_east.bmp", L"lancer_east");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Mechanoid/lancer_north.bmp", L"lancer_north");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Mechanoid/lancer_south.bmp", L"lancer_south");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Mechanoid/lancer_west.bmp", L"lancer_west");
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
    //철벽 아틀라스
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Building/Linked/Wall_Atlas_Smooth.bmp", L"Wall_Atlas_Smooth");
    //핏자국
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Mote/Blood.bmp", L"Blood");
    //폭발
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Mote/BlastFlame.bmp", L"BlastFlame");
    //이동 위치
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Mote/FeedbackGoto.bmp", L"FeedbackGoto");
    //공격 타겟
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Mote/FeedbackShoot.bmp", L"FeedbackShoot");

    //우주선
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Building/Ship/ShipEngine_north.bmp", L"ShipEngine_north");


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

        POS tPos{ 64 * iX + 32, 64 * iY + 32 };

        pObj = CAbstractFactory<CSteelWall>::Create(tPos);
        CObjMgr::Get_Instance()->Add_Object(OBJ_WALL, pObj);
        CTileMgr::Get_Instance()->Set_TileOption(tPos, OPT_BLOCKED);
        CTileMgr::Get_Instance()->Set_TileObj(tPos, pObj);
    }

    //림 두마리
    for (int i = 0; i <5; ++i)
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

        POS tPos{ 64 * iX + 32, 64 * iY + 32 };

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
    if (/*!m_bEnemySpawned &&*/ CTimeMgr::Get_Instance()->Get_ElapsedTime() > m_fSpawnTime)
    {
        CSoundMgr::Get_Instance()->StopSound(SOUND_EVENT);
        CSoundMgr::Get_Instance()->PlaySound(L"LetterArriveBadUrgent.wav", SOUND_EVENT, .5f);
        
        (this->*WaveFuncs[m_iWaveIndex++])();// Spawn_Wave();
        
        if (m_iWaveIndex > 2)
        {
            m_iWaveIndex = 0;
        }

        m_fSpawnTime += 60;
        /*m_bEnemySpawned = true;*/
    }


    return OBJ_NOEVENT;
}

void CColony::Late_Update()
{
    CObjMgr::Get_Instance()->Late_Update();

    //릴리즈 후 이놈이 다시 만들어져서 말썽이다.
    //현재 씬이 콜로니씬일때만 돌리는게 맞나?
    //싱글톤의 문제: 여기저기서 겟 인스턴스 쓰니까 삭제 이후에도 언제든지 생길 수 있다.
    if (CSceneMgr::Get_Instance()->Get_Scene() == SC_COLONY)
    {
        CTileMgr::Get_Instance()->Late_Update();
    }

    CColonyMgr::Get_Instance()->Late_Update();

    CScrollMgr::Get_Instance()->Late_Update();
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
    CObjMgr::Get_Instance()->Delete_All();
    CSoundMgr::Get_Instance()->StopAll();

    CTileMgr::Destroy_Instance();
    CColonyMgr::Destroy_Instance();
    CPathFinder::Destroy_Instance();
    CTimeMgr::Destroy_Instance();
    CEffectMgr::Destroy_Instance();
}

void CColony::Create_UI()
{
    //UI
    //메뉴 버튼
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ButtonSubtleAtlas_Menu.bmp", L"ButtonSubtleAtlas_Menu");
    //주메뉴 버튼
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/GoToMain.bmp", L"GoToMain");
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
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/DeadColonist.bmp", L"DeadColonist");
    //상세정보창
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/DetailViewBG.bmp", L"DetailViewBG");
    //선택한 것 강조
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/SelectionBracketWhole.bmp", L"SelectionBracketWhole");
    //철 아이템
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Item/Steel_b.bmp", L"Steel_b");
    //메시지박스
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/MsgBox.bmp", L"MsgBox");
    //소집상태 표시 칼 Draft
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Draft.bmp", L"Draft");

    //우주선 버튼
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ShipBtn.bmp", L"ShipBtn");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/LaunchShipBtn.bmp", L"LaunchShipBtn");

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

    ////우주선 건설 버튼
    //CObj* pShip = CAbstractFactory<CMyButton>::
    //    Create(fShortBtnCX * 3.5f, WINCY - fShortBtnCY * 1.f);
    //pShip->Set_Size(fShortBtnCX, fShortBtnCY);
    //pShip->Set_ImgKey(L"ShipBtn");
    //pStructureBtn->Get_ChildList()->push_back(pShip);
    //CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pShip);

    ////우주선 발사 버튼
    //CObj* pLaunchShip = CAbstractFactory<CMyButton>::
    //    Create(fShortBtnCX * 3.5f, WINCY - fShortBtnCY * 1.f);
    //pLaunchShip->Set_Size(fShortBtnCX, fShortBtnCY);
    //pLaunchShip->Set_ImgKey(L"LaunchShipBtn");
    //pCommandBtn->Get_ChildList()->push_back(pLaunchShip);
    //CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pLaunchShip);

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

    //메뉴버튼
    CObj* pMenuBtn = CAbstractFactory<CMyButton>::
        Create(WINCX - 64 * 0.5f, WINCY - 32 * 0.5f);
    pMenuBtn->Set_Size(64, 32);
    pMenuBtn->Set_Activate(true);
    pMenuBtn->Set_ImgKey(L"ButtonSubtleAtlas_Menu");
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pMenuBtn);

    //판넬
    CObj* pMenuBG = CAbstractFactory<CPanel>::
        Create(WINCX - 400 * 0.5f, WINCY - 150 * 0.5f - 32.f);
    pMenuBG->Set_Size(400, 150);
    pMenuBG->Set_Activate(false);
    pMenuBG->Set_ImgKey(L"DetailViewBG");
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pMenuBG);
    pMenuBtn->Get_ChildList()->push_back(pMenuBG);
    pMenuBG->Set_Parent(pMenuBtn);

    //주메뉴버튼
    CObj* pGoToMainBtn = CAbstractFactory<CMyButton>::
        Create(WINCX - 325, WINCY -124);
    pGoToMainBtn->Set_Size(128, 64);
    pGoToMainBtn->Set_Activate(false);
    pGoToMainBtn->Set_ImgKey(L"GoToMain");
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pGoToMainBtn);
    pMenuBtn->Get_ChildList()->push_back(pGoToMainBtn);
    pGoToMainBtn->Set_Parent(pMenuBtn);

    //Create_ShipBtn();
}

void CColony::Create_ShipBtn()
{
    float fShortBtnCX = 64.f;
    float fShortBtnCY = 64.f;

    CObj* pStructureBtn(nullptr);
    CObj* pCommandBtn(nullptr);
    //구조물, 명령 버튼을 가져온다.
    list<CObj*> UIList = CObjMgr::Get_Instance()->Get_List()[OBJ_UI];

    for (CObj* pUI : UIList)
    {
        if (!lstrcmp(L"ButtonSubtleAtlas_Structure", pUI->Get_ImgKey()))
        {
            pStructureBtn = pUI;
        }
        else if (!lstrcmp(L"ButtonSubtleAtlas_Command", pUI->Get_ImgKey()))
        {
            pCommandBtn = pUI;
        }
    }

    if (!pStructureBtn || !pCommandBtn)
    {
        return;
    }

    //우주선 건설 버튼
    CObj* pShip = CAbstractFactory<CMyButton>::
        Create(fShortBtnCX * 3.5f, WINCY - fShortBtnCY * 1.f);
    pShip->Set_Size(fShortBtnCX, fShortBtnCY);
    pShip->Set_ImgKey(L"ShipBtn");
    pStructureBtn->Get_ChildList()->push_back(pShip);
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pShip);

    //우주선 발사 버튼
    CObj* pLaunchShip = CAbstractFactory<CMyButton>::
        Create(fShortBtnCX * 3.5f, WINCY - fShortBtnCY * 1.f);
    pLaunchShip->Set_Size(fShortBtnCX, fShortBtnCY);
    pLaunchShip->Set_ImgKey(L"LaunchShipBtn");
    pCommandBtn->Get_ChildList()->push_back(pLaunchShip);
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pLaunchShip);
}

void CColony::Spawn_Wave1()
{
    for (int i = 0; i < 1; ++i)
    {
        Spawn_Random<CBoomrat>();
    }
    for (int i = 0; i < 1; ++i)
    {
        Spawn_Random<CLancer>();
    }
    
}

void CColony::Spawn_Wave2()
{
    for (int i = 0; i < 2; ++i)
    {
        Spawn_Random<CBoomrat>();
    }
    for (int i = 0; i < 2; ++i)
    {
        Spawn_Random<CLancer>();
    }
}

void CColony::Spawn_Wave3()
{
    for (int i = 0; i < 3; ++i)
    {
        Spawn_Random<CBoomrat>();
    }
    for (int i = 0; i < 3; ++i)
    {
        Spawn_Random<CLancer>();
    }
    for (int i = 0; i < 1; ++i)
    {
        Spawn_Random<CCentipede>();
    }
}
