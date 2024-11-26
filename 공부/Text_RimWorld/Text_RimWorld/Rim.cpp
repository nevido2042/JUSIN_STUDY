#include "pch.h"
#include "Rim.h"

CRim::CRim(CTileMap* _pTileMap, string _Name)
    :m_pTileMap(_pTileMap), m_Pos(0, 0), m_Name(_Name)
{
}

void CRim::Initialize()
{
	Move_Pos(Pos(0, 0));
    m_Last_Move_Time = std::chrono::high_resolution_clock::now();
}

void CRim::Update()
{
    auto iCurrent_Time = chrono::high_resolution_clock::now();
    auto iElapsed = chrono::duration_cast<chrono::milliseconds>(iCurrent_Time - m_Last_Move_Time);
    if (iElapsed.count() >= 1000)
    { // 1��(1000ms) ���
        Wander();
    }
}

void CRim::Render()
{
}

void CRim::Release()
{
}

void CRim::Move_Pos(Pos _Pos)
{
	m_pTileMap->Change_Tile(m_Pos, CTile::NONE);
	m_pTileMap->Change_Tile(_Pos, CTile::RIM);

	m_Pos = _Pos;
}

void CRim::Print_State(int _i)
{
    GotoXY(Pos(MapSize * 2, _i));
    cout << m_Name << ": ��ȸ ��";
    GotoXY(Pos(0, 0));
}

void CRim::Wander()
{
    //8���� (12�ú���)
    array<Pos, 8> Directions
    { Pos(0,-1),Pos(1,-1),Pos(1,0),Pos(1,1),Pos(0,1),Pos(-1,1),Pos(-1,0),Pos(-1,-1) };

    vector<Pos> vecMovablePos;
    //���� ��ġ ���� �� �� �ִ� Ÿ�� Ž��
    for (int i = 0; i < 8; ++i)
    {
        Pos CheckPos(m_Pos.iX + Directions[i].iX, m_Pos.iY + Directions[i].iY);
        if (CheckPos.iX <0 || CheckPos.iX > MapSize - 1 || CheckPos.iY <0 || CheckPos.iY > MapSize - 1)
        {
            continue;
        }
        //�迭�� �ְ�
        vecMovablePos.push_back(CheckPos);
    }

    //���� ��ġ �̰�
    Pos RandomPos = vecMovablePos.at(rand() % vecMovablePos.size());

    //�̵�
    Move_Pos(RandomPos);
    m_Last_Move_Time = chrono::high_resolution_clock::now();
}
