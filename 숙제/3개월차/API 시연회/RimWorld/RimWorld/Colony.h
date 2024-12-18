#pragma once
#include "Scene.h"

#include "ColonyMgr.h"

#include "MenuBtn.h"
#include "GameSpeedBtn.h"

#include "Tile.h"
#include "Rim.h"
#include "ArcitectBtn.h"
#include "SteelWall.h"
#include "Centipede.h"
#include "Tree.h"
#include "Boomrat.h"

class CColony :
    public CScene
{
public:
    CColony();
    virtual ~CColony();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

    bool m_bEnemySpawned;
};

