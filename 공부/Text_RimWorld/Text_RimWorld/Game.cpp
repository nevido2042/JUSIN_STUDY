#include "pch.h"
#include "Game.h"
#include "TileMap.h"

CGame::CGame()
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
}

void CGame::Print_InputAnyKey_StartToPlay()
{
    cout << endl;	cout << "�����Ϸ��� �ƹ�Ű�� ��������." << endl;

    // ���� �ð� ����
    //auto iStart_Time = std::chrono::high_resolution_clock::now();
    bool bIsStart(false);
    while (true)
    {
        //auto iCurrent_Time = chrono::high_resolution_clock::now();
        //auto iElapsed = chrono::duration_cast<chrono::milliseconds>(iCurrent_Time - iStart_Time);
        //if (iElapsed.count() < 1000)
        //{ 
        //    continue;
        //}
        //else
        //{// 1��(1000ms) ���
        //    iStart_Time = chrono::high_resolution_clock::now();
        //}

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

    CTileMap TileMap;
    TileMap.Initialize();

    while (true)
    {
        TileMap.Update();

        if (GetKeyState(VK_ESCAPE) & 0x8000)
        { // Ű�� ���ȴ��� Ȯ��
            break;
        }
    }
}
