#pragma once
#include "Object.h"
#include "ObjectManager.h"
#include "TileMap.h"

class CGame :
    public CObject
{
public:
    CGame();
    virtual ~CGame();

    // CObject을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Render() override;
    void Release() override;

public:
    void Print_InputAnyKey_StartToPlay();
    void Start_Game();
private:
    CObjectManager* m_pObjectManager;
    CTileMap* m_pTileMap;


};

