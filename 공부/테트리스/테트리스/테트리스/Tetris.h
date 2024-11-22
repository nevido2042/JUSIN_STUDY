#pragma once
#include "Object.h"
#include <array>

#define Horizontal 5
#define Vertical 10

class CTetris :
    public CObject
{
private:
    enum TileType :char { NONE = 0, BLOCK, BORDER };
public:
    // CObject을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Release() override;
private:
    void Print_InputAnyKey_StartToPlay();
    void Start_Game();
    void Render();
private:
    array<array<TileType, Horizontal>, Vertical> m_TileMap;
};

