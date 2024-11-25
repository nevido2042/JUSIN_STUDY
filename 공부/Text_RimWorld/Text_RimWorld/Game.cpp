#include "pch.h"
#include "Game.h"
#include "TileMap.h"
#include "Rim.h"

CGame::CGame()
    :m_pObjectManager(nullptr), m_pTileMap(nullptr)
{
}

CGame::~CGame()
{
    Release();
}

void CGame::Initialize()
{
    Print_InputAnyKey_StartToPlay();
}

void CGame::Update()
{

}

void CGame::Release()
{
    Safe_Delete(m_pObjectManager);
    //Safe_Delete(m_pTileMap);
}

void CGame::Print_InputAnyKey_StartToPlay()
{
    cout << endl;	cout << "시작하려면 아무키나 누르세요." << endl;

    bool bIsStart(false);
    while (true)
    {
        for (int iKey = 0; iKey < 256; ++iKey)
        {
            if (GetKeyState(iKey) & 0x8000)
            { // 키가 눌렸는지 확인
                bIsStart = true;
                break;
            }
            //Sleep(10); // CPU 사용량 줄이기
        }
        if (bIsStart)
        {
            break;
        }
    }
    Start_Game();
}

void CGame::Start_Game()
{
    system("cls");

    m_pObjectManager = new CObjectManager;
    m_pObjectManager->Initialize();
    m_pObjectManager->Add_Object(m_pTileMap = new CTileMap());
    m_pObjectManager->Add_Object(new CRim(m_pTileMap));

    while (true)
    {
        m_pObjectManager->Update();

        //esc 종료
        if (GetKeyState(VK_ESCAPE) & 0x8000)
        {
            break;
        }
    }
}
