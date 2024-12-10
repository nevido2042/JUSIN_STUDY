#pragma once
#include "CMonster.h"
class CGut_Monster2 : public CMonster
{
public:
    CGut_Monster2();
    virtual ~CGut_Monster2() { Release(); }

public:
    void Initialize() override;
    void Initialize_Animation();
    int  Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

private:

private:
    int Bullet_X;
    float MAX_Hp; // 기본 체력
    float Hp_Count; // 보스에 충돌할때마다 체크

    bool                m_bJump; // 점프체크
    float               m_fJumpPower;//점프속도
    float               m_fTime; // 시간
    bool                m_bisGround;

    ////////////////////////


    bool m_Boss_Ground;

    ////////
    enum MONSTATE { STATE_IDLE, STATE_ATTACK, STATE_END };
    MONSTATE    m_enState;
    UINT        m_iStateCooldown;
    DIRECTION   m_enMovingDir;
    float       m_fAttackRange;


};

