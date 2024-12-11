#pragma once
#include "Scene.h"
class CCustomize :
    public CScene
{
public:
    CCustomize();
    virtual ~CCustomize();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
};

