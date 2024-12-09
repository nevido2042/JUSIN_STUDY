#pragma once
#include "CMonster.h"
class CSilver_Mob1 :    // 떠다니면서 플레이어를 쫓아옴
    public CMonster
{
public:
    CSilver_Mob1();
    virtual ~CSilver_Mob1() override;

public:
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;

private:
    void Update_Animation();
    void Set_Angle(INFO* _pTarget);

    ULONGLONG m_ullTime;
    ULONGLONG m_ullCerrentTime;

    int     m_iImagePose;
    int     m_iAnimationX;
    int     m_iAnimationY;
};

