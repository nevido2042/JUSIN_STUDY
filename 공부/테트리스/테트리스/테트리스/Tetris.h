#pragma once
#include "Object.h"
#include <array>
#include <vector>

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
    void Spawn_Block();
    void Drop_AllBlocks();
    void Check_Full_Line();
private:
    typedef struct tagPosition
    {
        int iX;
        int iY;
        tagPosition()
            :iX(0), iY(0) {};
        tagPosition(int _iX, int _iY)
            :iX(_iX), iY(_iY) {};
        void operator()(int _iX, int _iY)
        {
            iX = _iX;
            iY = _iY;
        }
    }Pos;
private:
    array<array<TileType, Horizontal>, Vertical> m_TileMap;

    Pos m_Controlled_Block_Pos;
    vector<Pos>m_vecControl_Pos;
    bool m_bKeyDown;
};

