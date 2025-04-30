#pragma once

#include"CObj.h"

#include "Define.h"

class CBoss_ElecMan : public CObj
{
public:
    CBoss_ElecMan();
    ~CBoss_ElecMan();

private:
    // CObj을(를) 통해 상속됨
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

public:
    void Set_Player(CObj* pPlayer) { m_Boss_pPlayer = pPlayer; } // 플레이어 객체 설정
    CObj* Create_Bullet(BULLET_ID _type);

private:
    list<CObj*>* m_p_Boss_Bullet;
    CObj* m_Boss_pPlayer;

private:
    IMAGE    m_tIdle;
    IMAGE   m_tAttack1;
    IMAGE   m_tAttack2;
    IMAGE   m_tNowState;
    IMAGE   m_tChase1;
    IMAGE   m_tChase2;
    IMAGE   m_tJump;

    float m_fAttackRange;
    float m_fChaseRange;
    ULONGLONG m_ulTime;

    BEHAVOR_STATE m_BossState;

    int m_iCount;
};


