#pragma once
#include "Object.h"
#include "TileMap.h"
#include <chrono>

class CRim :
    public CObject
{
public:
    CRim() = delete;
    CRim(CTileMap* _pTileMap);
public:
    // CObject을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Release() override;
public:
    void Move_Pos(Pos _Pos);
private:
    Pos m_Pos;
    CTileMap* m_pTileMap;
    std::chrono::steady_clock::time_point Last_Move_Time;
};

