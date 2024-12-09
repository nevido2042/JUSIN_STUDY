#pragma once
#include "CBullet.h"

class CBullet_Monter_Elec :
    public CBullet
{
public:
    CBullet_Monter_Elec();
    ~CBullet_Monter_Elec();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    IMAGE m_tAttack1;
    IMAGE m_tAttack2;
    IMAGE m_tNowState;

 

    float m_iDirection;
    int iBulletCount;
   
};

