#include "pch.h"
#include "Rim.h"

CRim::CRim(CTileMap* _pTileMap)
	:m_pTileMap(_pTileMap), m_Pos(0, 0)
{
}

void CRim::Initialize()
{
	Move_Pos(Pos(0, 0));
    Last_Move_Time = std::chrono::high_resolution_clock::now();
}

void CRim::Update()
{
    auto iCurrent_Time = chrono::high_resolution_clock::now();
    auto iElapsed = chrono::duration_cast<chrono::milliseconds>(iCurrent_Time - Last_Move_Time);
    if (iElapsed.count() >= 1000)
    { // 1ÃÊ(1000ms) °æ°ú
        Move_Pos(Pos(rand() % MapSize, rand() % MapSize));
        Last_Move_Time = chrono::high_resolution_clock::now();
    }
	
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
