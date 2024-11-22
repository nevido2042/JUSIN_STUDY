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

void CTetris::Start_Game()
{
    // ���� �ð� ����
    auto iStart_Time = std::chrono::high_resolution_clock::now();
    
    Update();
    while (true)
    {
        auto iCurrent_Time = chrono::high_resolution_clock::now();
        auto iElapsed = chrono::duration_cast<chrono::milliseconds>(iCurrent_Time - iStart_Time);

        if (iElapsed.count() >= 1000)
        { // 1��(1000ms) ���
            iStart_Time = chrono::high_resolution_clock::now();
            Update();
        }

        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
        {
            return; // ���α׷� ����
        }
        
        //Sleep(10);
    }
}

void CTetris::Render()
{
    //Ÿ�� �ʱ׸���
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
                cout << "��";
                break;
            case BORDER:
                cout << "��";
            default:
                break;
            }
        }
        cout << endl;
    }
    //ESC ��������
    cout << "�����Ϸ��� ESC�� ��������." << endl;
}
