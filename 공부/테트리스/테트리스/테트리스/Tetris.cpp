#include "pch.h"
#include "Tetris.h"

void CTetris::Initialize()
{
    for (int v = 0; v < Vertical; ++v)
    {
        for (int h = 0; h < Horizontal; ++h)
        {
            m_TileMap[v][h] = NONE;
        }
    }

    for (int v = 0; v < Vertical; ++v)
    {
        m_TileMap[v][0] = BORDER;
        m_TileMap[v][Horizontal - 1] = BORDER;
    }

    for (int h = 0; h < Horizontal; ++h)
    {
        m_TileMap[Vertical - 1][h] = BORDER;
    }

	Print_InputAnyKey_StartToPlay();
}

void CTetris::Update()
{
    system("cls");
    Render();
}

void CTetris::Release()
{
}

void CTetris::Print_InputAnyKey_StartToPlay()
{
    cout << " TTTTT  EEEEE  TTTTT  RRRR    III  SSSSS " << endl;
    cout << "   T    E       T    R   R    I   S      " << endl;
    cout << "   T    EEEE    T    RRRR     I   SSSSS  " << endl;
    cout << "   T    E       T    R  R     I      S   " << endl;
    cout << "   T    EEEEE   T    R   R   III  SSSSS " << endl;
    cout << endl;
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

void CTetris::Start_Game()
{
    // 현재 시간 저장
    auto iStart_Time = std::chrono::high_resolution_clock::now();
    
    Update();
    while (true)
    {
        auto iCurrent_Time = chrono::high_resolution_clock::now();
        auto iElapsed = chrono::duration_cast<chrono::milliseconds>(iCurrent_Time - iStart_Time);

        if (iElapsed.count() >= 1000)
        { // 1초(1000ms) 경과
            iStart_Time = chrono::high_resolution_clock::now();
            Update();
        }

        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
        {
            return; // 프로그램 종료
        }
        
        //Sleep(10);
    }
}

void CTetris::Render()
{
    //타일 맵그리기
    for (int v = 0; v < Vertical; ++v)
    {
        for (int h = 0; h < Horizontal; ++h)
        {
            switch (m_TileMap[v][h])
            {
            case NONE:
                cout << "  ";
                break;
            case BLOCK:
                cout << "■";
                break;
            case BORDER:
                cout << "□";
            default:
                break;
            }
        }
        cout << endl;
    }
    //ESC 게임종료
    cout << "종료하려면 ESC를 누르세요." << endl;
}
