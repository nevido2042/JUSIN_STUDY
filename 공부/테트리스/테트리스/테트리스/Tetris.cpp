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
    //꽉찬 라인이 있으면 NONE 으로 변경
    Check_Full_Line();
    //키 입력 받을 수 있도록
    m_bKeyDown = false;
    //모든 블럭 내리기
    Drop_AllBlocks();
    //출력
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

    Spawn_Block();
    Update();
    while (true)
    {
        //입력
        if (GetAsyncKeyState(VK_LEFT) & 0x8000 && !m_bKeyDown)
        {
            m_bKeyDown = true;

            //왼쪽에 경계선이 없어야하고, 유효한 타일 맵이어야한다.
            if (m_TileMap[m_Controlled_Block_Pos.iY][m_Controlled_Block_Pos.iX - 1] != BORDER
                && m_Controlled_Block_Pos.iX - 1 >= 0)
            {
                m_TileMap[m_Controlled_Block_Pos.iY][m_Controlled_Block_Pos.iX] = NONE;
                m_TileMap[m_Controlled_Block_Pos.iY][m_Controlled_Block_Pos.iX - 1] = BLOCK;
                m_Controlled_Block_Pos = Pos(m_Controlled_Block_Pos.iX - 1, m_Controlled_Block_Pos.iY);
            }
        }


        if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && !m_bKeyDown)
        {
            m_bKeyDown = true;

            //왼쪽에 경계선이 없어야하고, 유효한 타일 맵이어야한다.
            if (m_TileMap[m_Controlled_Block_Pos.iY][m_Controlled_Block_Pos.iX + 1] != BORDER
                && m_Controlled_Block_Pos.iX < Horizontal)
            {
                m_TileMap[m_Controlled_Block_Pos.iY][m_Controlled_Block_Pos.iX] = NONE;
                m_TileMap[m_Controlled_Block_Pos.iY][m_Controlled_Block_Pos.iX + 1] = BLOCK;
                m_Controlled_Block_Pos = Pos(m_Controlled_Block_Pos.iX + 1, m_Controlled_Block_Pos.iY);
            }
        }
        /*else
        {
            if (bKeyDown)
            {
                bKeyDown = false;
            }
        }*/

        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
        {
            return; // 프로그램 종료
        }

        auto iCurrent_Time = chrono::high_resolution_clock::now();
        auto iElapsed = chrono::duration_cast<chrono::milliseconds>(iCurrent_Time - iStart_Time);

        if (iElapsed.count() >= 100)
        { // 1초(1000ms) 경과
            iStart_Time = chrono::high_resolution_clock::now();
            Update();
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

void CTetris::Spawn_Block()
{
    m_Controlled_Block_Pos(Horizontal / 2, 0);
    m_TileMap[0][Horizontal / 2] = BLOCK;
}

void CTetris::Drop_AllBlocks()
{
    int iBlockCount(0);
    list<int> VList;
    list<int> HList;

    for (int v = 0; v < Vertical; ++v)
    {
        for (int h = 0; h < Horizontal; ++h)
        {
            //블럭이고, v+1이 유효하고, 아래가 블럭이 아니면, 경계선도 아니라면
            if (m_TileMap[v][h] == BLOCK && v + 1 < Vertical && m_TileMap[v + 1][h] != BLOCK && m_TileMap[v + 1][h] != BORDER)
            {
                VList.push_back(v);
                HList.push_back(h);
                ++iBlockCount;
            }
        }
    }
    //블럭 타일 모두 내리기
    for (int i = 0; i < iBlockCount; ++i)
    {
        //컨트롤 블럭의 위치도 변경
        if (m_Controlled_Block_Pos.iX == HList.front() && m_Controlled_Block_Pos.iY == VList.front())
        {
            ++m_Controlled_Block_Pos.iY;

            //땅에 닿으면
            if (m_TileMap[m_Controlled_Block_Pos.iY + 1][m_Controlled_Block_Pos.iX] == BLOCK
                || m_TileMap[m_Controlled_Block_Pos.iY + 1][m_Controlled_Block_Pos.iX] == BORDER)
            {
                Spawn_Block();
            }
        }

        m_TileMap[VList.front()][HList.front()] = NONE;
        m_TileMap[VList.front() + 1][HList.front()] = BLOCK;
        VList.pop_front();
        HList.pop_front();
    }

}

void CTetris::Check_Full_Line()
{
    for (int v = 0; v < Vertical - 1; ++v)
    {
        bool bFull(true);
        for (int h = 1; h < Horizontal - 1; ++h)
        {
            if (m_TileMap[v][h] != BLOCK)
            {
                bFull = false;
                break;
            }
        }
        if (bFull)
        {
            for (int h = 1; h < Horizontal - 1; ++h)
            {
                m_TileMap[v][h] = NONE;
            }
        }
    }
}
