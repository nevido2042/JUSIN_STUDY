#pragma once
#include "Scene.h"

#include "ColonyMgr.h"
#include "Tile.h"
#include "Rim.h"
#include "SteelWall.h"
#include "Centipede.h"
#include "Lancer.h"
#include "Tree.h"
#include "Boomrat.h"
#include "MyButton.h"
#include "GameSpeedBtn.h"
#include "RimSlotMgr.h"
#include "DetailView.h"

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
private:
    void Create_UI();

    bool m_bEnemySpawned;
};

