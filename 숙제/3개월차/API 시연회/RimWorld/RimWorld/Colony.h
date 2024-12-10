#pragma once
#include "Scene.h"

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

