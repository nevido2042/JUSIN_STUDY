#pragma once
#include "Scene.h"

#include "MenuButton.h"
#include "Tile.h"
#include "Rim.h"
//#include "ColonyMgr.h" 필요 없나?

class CColony :
    public CScene
{
public:
    CColony();
    virtual ~CColony();

public:
    void Initialize() override;
    int Update() override;
    int Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
};

