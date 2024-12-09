#pragma once

#include"CObj.h"

enum StateType
{
    IDLE,
    ATTACK,
    CHASE,
};

class CBoss_ElecMan : public CObj
{
public:
    CBoss_ElecMan();
    ~CBoss_ElecMan();

private:
    // CObj��(��) ���� ��ӵ�
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

public:
    float Get_Hp() { return MAX_Hp - Hp_Count; }
    void Set_HP(float _type) { Hp_Count = _type; }
    void Set_Player(CObj* pPlayer) { m_Boss_pPlayer = pPlayer; } // �÷��̾� ��ü ����
    CObj* Create_Bullet(BULLET_ID _type);
private:

    list<CObj*>* m_p_Boss_Bullet;
    INFO HP_INFO;
    CObj* m_Boss_pPlayer; // �÷��̾� ��ü�� ������ ��� ����

private:
    float MAX_Hp; // �⺻ ü��
    float Hp_Count; // ������ �浹�Ҷ����� üũ

private:
    IMAGE    m_tIdle;
    IMAGE   m_tAttack1;
    IMAGE   m_tAttack2;
    IMAGE   m_tNowState;
    IMAGE   m_tChase;

    float m_fAttackRange;
    float m_fChaseRange;
    ULONGLONG m_ulTime;

    StateType m_BossState;

    int m_iCount;
};

