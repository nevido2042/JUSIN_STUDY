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
    float MAX_Hp; // �⺻ ü��
    float Hp_Count; // ������ �浹�Ҷ����� üũ

    bool                m_bJump; // ����üũ
    float               m_fJumpPower;//�����ӵ�
    float               m_fTime; // �ð�
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

