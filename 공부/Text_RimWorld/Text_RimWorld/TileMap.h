#pragma once
#include "Object.h"
#include <array>
#include "Tile.h"
#include "Define.h"

class CRim;

#define MapSize 10
using namespace std;

class CTileMap :
    public CObject
{
public:
    CTileMap();
    virtual ~CTileMap();
public:
    // CObject을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Release() override;
public:
    void Change_Tile(Pos _Pos, CTile::Type _Type);
    void Spawn_Rim();
private:
    void Render();
    void GotoXY(Pos _Pos);
    void Print_Tile(Pos _Pos, CTile::Type _Type);
    void Visible_Cursor(bool _bool);

private:
    array<array<CTile, MapSize>, MapSize> m_TileMap;
    array<array<CTile, MapSize>, MapSize> m_TileMap_Buffer;
};

