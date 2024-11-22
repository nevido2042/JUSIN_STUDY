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
    //���� ������ ������ NONE ���� ����
    Check_Full_Line();
    //Ű �Է� ���� �� �ֵ���
    m_bKeyDown = false;
    //��� �� ������
    Drop_AllBlocks();
    //���
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

    Spawn_Block();
    Update();
    while (true)
    {
        //�Է�
        if (GetAsyncKeyState(VK_LEFT) & 0x8000 && !m_bKeyDown)
        {
            m_bKeyDown = true;

            //���ʿ� ��輱�� ������ϰ�, ��ȿ�� Ÿ�� ���̾���Ѵ�.
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

            //���ʿ� ��輱�� ������ϰ�, ��ȿ�� Ÿ�� ���̾���Ѵ�.
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
            return; // ���α׷� ����
        }

        auto iCurrent_Time = chrono::high_resolution_clock::now();
        auto iElapsed = chrono::duration_cast<chrono::milliseconds>(iCurrent_Time - iStart_Time);

        if (iElapsed.count() >= 100)
        { // 1��(1000ms) ���
            iStart_Time = chrono::high_resolution_clock::now();
            Update();
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
            //���̰�, v+1�� ��ȿ�ϰ�, �Ʒ��� ���� �ƴϸ�, ��輱�� �ƴ϶��
            if (m_TileMap[v][h] == BLOCK && v + 1 < Vertical && m_TileMap[v + 1][h] != BLOCK && m_TileMap[v + 1][h] != BORDER)
            {
                VList.push_back(v);
                HList.push_back(h);
                ++iBlockCount;
            }
        }
    }
    //�� Ÿ�� ��� ������
    for (int i = 0; i < iBlockCount; ++i)
    {
        //��Ʈ�� ���� ��ġ�� ����
        if (m_Controlled_Block_Pos.iX == HList.front() && m_Controlled_Block_Pos.iY == VList.front())
        {
            ++m_Controlled_Block_Pos.iY;

            //���� ������
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
