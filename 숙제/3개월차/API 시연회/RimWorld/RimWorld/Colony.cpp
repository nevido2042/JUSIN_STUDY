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
    //��
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Head/Male_Average_Normal_south.bmp", L"Male_Average_Normal_south");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Head/Male_Average_Normal_east.bmp", L"Male_Average_Normal_east");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Head/Male_Average_Normal_north.bmp", L"Male_Average_Normal_north");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Head/Male_Average_Normal_west.bmp", L"Male_Average_Normal_west");
    
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Head/Female_Average_Normal_east.bmp", L"Female_Average_Normal_east");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Head/Female_Average_Normal_north.bmp", L"Female_Average_Normal_north");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Head/Female_Average_Normal_west.bmp", L"Female_Average_Normal_west");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Rim/Head/Female_Average_Normal_south.bmp", L"Female_Average_Normal_south");
    //����
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

    //�Ӹ�
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

    //��Ʈ�׼� ����
    //CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Weapon/BoltActionRifle.bmp", L"BoltActionRifle");
    //CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Weapon/Bullet_Small.bmp", L"Bullet_Small");

    //���� ����
    //CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Weapon/ChargeBlasterLight.bmp", L"ChargeBlasterLight");
    //CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Weapon/Charge_Small.bmp", L"Charge_Small");
    //���� ����
    //CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Weapon/Charge_Small.bmp", L"Charge_Small");

    //Centipede
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Mechanoid/Centipede_east.bmp", L"Centipede_east");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Mechanoid/Centipede_north.bmp", L"Centipede_north");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Mechanoid/Centipede_south.bmp", L"Centipede_south");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Mechanoid/Centipede_west.bmp", L"Centipede_west");
    //����
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Mechanoid/lancer_east.bmp", L"lancer_east");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Mechanoid/lancer_north.bmp", L"lancer_north");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Mechanoid/lancer_south.bmp", L"lancer_south");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Mechanoid/lancer_west.bmp", L"lancer_west");
    //Boomrat
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Animal/Boomrat_east.bmp", L"Boomrat_east");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Animal/Boomrat_north.bmp", L"Boomrat_north");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Animal/Boomrat_south.bmp", L"Boomrat_south");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Pawn/Animal/Boomrat_west.bmp", L"Boomrat_west");
    //����
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Tree/TreePoplar_LeaflessA.bmp", L"TreePoplar_LeaflessA");

    //Ÿ��
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Terrain/Ice.bmp", L"Ice");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Terrain/Gravel.bmp", L"Gravel");
    //ö�� ��Ʋ��
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Building/Linked/Wall_Atlas_Smooth.bmp", L"Wall_Atlas_Smooth");
    //���ڱ�
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Mote/Blood.bmp", L"Blood");
    //����
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Mote/BlastFlame.bmp", L"BlastFlame");
    //�̵� ��ġ
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Mote/FeedbackGoto.bmp", L"FeedbackGoto");
    //���� Ÿ��
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Mote/FeedbackShoot.bmp", L"FeedbackShoot");

    //���ּ�
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Building/Ship/ShipEngine_north.bmp", L"ShipEngine_north");


    //��
    CTileMgr::Get_Instance()->Initialize();

    bool bVisitArray[TILEX * TILEY];
    memset(bVisitArray, false, sizeof(bVisitArray));

    for (int i = 0; i < (TILEX * TILEY) / 10; ++i)
    {
        int iX = int(rand() % TILEX);
        int iY = int(rand() % TILEY);

        //�� ���Դ� ���� ���� ������ �̴´�.
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

    //�� �θ���
    for (int i = 0; i <5; ++i)
    {
        int iX = int(rand() % TILEX);
        int iY = int(rand() % TILEY);

        //�� ���Դ� ���� ���� ������ �̴´�.
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

    //�� �� �� �� ����
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

    //������ �� �̳��� �ٽ� ��������� �����̴�.
    //���� ���� �ݷδϾ��϶��� �����°� �³�?
    //�̱����� ����: �������⼭ �� �ν��Ͻ� ���ϱ� ���� ���Ŀ��� �������� ���� �� �ִ�.
    if (CSceneMgr::Get_Instance()->Get_Scene() == SC_COLONY)
    {
        CTileMgr::Get_Instance()->Late_Update();
    }

    CColonyMgr::Get_Instance()->Late_Update();

    CScrollMgr::Get_Instance()->Late_Update();
}

void CColony::Render(HDC hDC)
{
    // ���� ��� (Black �̹����� �ٷ� hDC�� �׸���)
    HDC hBlackDC = CBmpMgr::Get_Instance()->Find_Image(L"Black");
    BitBlt(hDC, 0, 0, WINCX, WINCY, hBlackDC, 0, 0, SRCCOPY);  // ���� ��� �׸���

    CTileMgr::Get_Instance()->Render(hDC);

    // ��ü �Ŵ��� ������
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
    //�޴� ��ư
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ButtonSubtleAtlas_Menu.bmp", L"ButtonSubtleAtlas_Menu");
    //�ָ޴� ��ư
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/GoToMain.bmp", L"GoToMain");
    //���� ��ư
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ButtonSubtleAtlas_Architect.bmp", L"ButtonSubtleAtlas_Architect");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ButtonSubtleAtlas_Command.bmp", L"ButtonSubtleAtlas_Command");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Deconstruct.bmp", L"Deconstruct");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ButtonSubtleAtlas_Structure.bmp", L"ButtonSubtleAtlas_Structure");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Construct.bmp", L"Construct");
    //���� ���ǵ� ��ư
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/TimeSpeedButton_Normal.bmp", L"TimeSpeedButton_Normal");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/TimeSpeedButton_Fast.bmp", L"TimeSpeedButton_Fast");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/TimeSpeedButton_Superfast.bmp", L"TimeSpeedButton_Superfast");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/TimeSpeedButton_Pause.bmp", L"TimeSpeedButton_Pause");
    //Designations
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Designations/Deconstruct.bmp", L"Deconstruct_mini");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Designations/RockSmooth_MenuIcon.bmp", L"RockSmooth_MenuIcon_mini");
    //�� ����
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/DesButBG.bmp", L"DesButBG");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/DeadColonist.bmp", L"DeadColonist");
    //������â
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/DetailViewBG.bmp", L"DetailViewBG");
    //������ �� ����
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/SelectionBracketWhole.bmp", L"SelectionBracketWhole");
    //ö ������
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Things/Item/Steel_b.bmp", L"Steel_b");
    //�޽����ڽ�
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/MsgBox.bmp", L"MsgBox");
    //�������� ǥ�� Į Draft
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Draft.bmp", L"Draft");

    //���ּ� ��ư
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ShipBtn.bmp", L"ShipBtn");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/LaunchShipBtn.bmp", L"LaunchShipBtn");

    //������â
    CObj* pDetailView = CAbstractFactory<CDetailView>::Create(200, WINCY - 107);
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pDetailView);

    //�� ���� �Ŵ���
    CObj* pRimSlotMgr = CAbstractFactory<CRimSlotMgr>::Create(0, 0);
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pRimSlotMgr);

    float fLongBtnCX = 128.f;
    float fLongBtnCY = 32.f;
    float fShortBtnCX = 64.f;
    float fShortBtnCY = 64.f;

    //���� ��ư
    CObj* pArchitectBtn = CAbstractFactory<CMyButton>::
        Create(fLongBtnCX * 0.5f, WINCY - fLongBtnCY * 0.5f);
    pArchitectBtn->Set_Size(fLongBtnCX, fLongBtnCY);
    pArchitectBtn->Set_Activate(true);
    pArchitectBtn->Set_ImgKey(L"ButtonSubtleAtlas_Architect");
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pArchitectBtn);

    //��� ��ư
    CObj* pCommandBtn = CAbstractFactory<CMyButton>::
        Create(fLongBtnCX * 0.5f, WINCY - fLongBtnCY * 1.5f);
    pCommandBtn->Set_Size(fLongBtnCX, fLongBtnCY);
    pCommandBtn->Set_ImgKey(L"ButtonSubtleAtlas_Command");
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pCommandBtn);
    pArchitectBtn->Get_ChildList()->push_back(pCommandBtn);
    pCommandBtn->Set_Parent(pArchitectBtn);

    //������ ��ư
    CObj* pStructureBtn = CAbstractFactory<CMyButton>::
        Create(fLongBtnCX * 0.5f, WINCY - fLongBtnCY * 2.5f);
    pStructureBtn->Set_Size(fLongBtnCX, fLongBtnCY);
    pStructureBtn->Set_ImgKey(L"ButtonSubtleAtlas_Structure");
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pStructureBtn);
    pArchitectBtn->Get_ChildList()->push_back(pStructureBtn);
    pStructureBtn->Set_Parent(pArchitectBtn);


    //��ü ��ư
    CObj* pDeconstructBtn = CAbstractFactory<CMyButton>::
        Create(fShortBtnCX * 2.5f, WINCY - fShortBtnCY * 1.f);
    pDeconstructBtn->Set_Size(fShortBtnCX, fShortBtnCY);
    pDeconstructBtn->Set_ImgKey(L"Deconstruct");
    pCommandBtn->Get_ChildList()->push_back(pDeconstructBtn);
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pDeconstructBtn);


    //ö�� �Ǽ� ��ư
    CObj* pConstruct = CAbstractFactory<CMyButton>::
        Create(fShortBtnCX * 2.5f, WINCY - fShortBtnCY * 1.f);
    pConstruct->Set_Size(fShortBtnCX, fShortBtnCY);
    pConstruct->Set_ImgKey(L"Construct");
    pStructureBtn->Get_ChildList()->push_back(pConstruct);
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pConstruct);

    ////���ּ� �Ǽ� ��ư
    //CObj* pShip = CAbstractFactory<CMyButton>::
    //    Create(fShortBtnCX * 3.5f, WINCY - fShortBtnCY * 1.f);
    //pShip->Set_Size(fShortBtnCX, fShortBtnCY);
    //pShip->Set_ImgKey(L"ShipBtn");
    //pStructureBtn->Get_ChildList()->push_back(pShip);
    //CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pShip);

    ////���ּ� �߻� ��ư
    //CObj* pLaunchShip = CAbstractFactory<CMyButton>::
    //    Create(fShortBtnCX * 3.5f, WINCY - fShortBtnCY * 1.f);
    //pLaunchShip->Set_Size(fShortBtnCX, fShortBtnCY);
    //pLaunchShip->Set_ImgKey(L"LaunchShipBtn");
    //pCommandBtn->Get_ChildList()->push_back(pLaunchShip);
    //CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pLaunchShip);

    //���� ���ǵ� ��ư
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

    //�޴���ư
    CObj* pMenuBtn = CAbstractFactory<CMyButton>::
        Create(WINCX - 64 * 0.5f, WINCY - 32 * 0.5f);
    pMenuBtn->Set_Size(64, 32);
    pMenuBtn->Set_Activate(true);
    pMenuBtn->Set_ImgKey(L"ButtonSubtleAtlas_Menu");
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pMenuBtn);

    //�ǳ�
    CObj* pMenuBG = CAbstractFactory<CPanel>::
        Create(WINCX - 400 * 0.5f, WINCY - 150 * 0.5f - 32.f);
    pMenuBG->Set_Size(400, 150);
    pMenuBG->Set_Activate(false);
    pMenuBG->Set_ImgKey(L"DetailViewBG");
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pMenuBG);
    pMenuBtn->Get_ChildList()->push_back(pMenuBG);
    pMenuBG->Set_Parent(pMenuBtn);

    //�ָ޴���ư
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
    //������, ��� ��ư�� �����´�.
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

    //���ּ� �Ǽ� ��ư
    CObj* pShip = CAbstractFactory<CMyButton>::
        Create(fShortBtnCX * 3.5f, WINCY - fShortBtnCY * 1.f);
    pShip->Set_Size(fShortBtnCX, fShortBtnCY);
    pShip->Set_ImgKey(L"ShipBtn");
    pStructureBtn->Get_ChildList()->push_back(pShip);
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pShip);

    //���ּ� �߻� ��ư
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
