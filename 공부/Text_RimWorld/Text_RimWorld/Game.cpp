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
    cout << endl;	cout << "�����Ϸ��� �ƹ�Ű�� ��������." << endl;

    bool bIsStart(false);
    while (true)
    {
        for (int iKey = 0; iKey < 256; ++iKey)
        {
            if (GetKeyState(iKey) & 0x8000)
            { // Ű�� ���ȴ��� Ȯ��
                bIsStart = true;
                break;
            }
            //Sleep(10); // CPU ��뷮 ���̱�
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

        //esc ����
        if (GetKeyState(VK_ESCAPE) & 0x8000)
        {
            break;
        }
    }
}
