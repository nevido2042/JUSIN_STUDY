#pragma once
#include "Scene.h"

#include "MenuButton.h"
#include "Tile.h"
//#include "ColonyMgr.h" �ʿ� ����?

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
};

