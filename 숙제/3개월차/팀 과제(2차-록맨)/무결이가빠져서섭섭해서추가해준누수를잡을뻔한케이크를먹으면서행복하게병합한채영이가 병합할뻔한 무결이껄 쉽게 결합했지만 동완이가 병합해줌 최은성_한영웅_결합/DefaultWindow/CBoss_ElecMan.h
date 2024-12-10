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
    float Get_Hp() { return MAX_Hp - Hp_Count; }
    void Set_HP(float _type) { Hp_Count = _type; }
    void Set_Player(CObj* pPlayer) { m_Boss_pPlayer = pPlayer; } // 플레이어 객체 설정
    CObj* Create_Bullet(BULLET_ID _type);
private:

    list<CObj*>* m_p_Boss_Bullet;
    INFO HP_INFO;
    CObj* m_Boss_pPlayer; // 플레이어 객체를 저장할 멤버 변수

private:
    float MAX_Hp; // 기본 체력
    float Hp_Count; // 보스에 충돌할때마다 체크

private:
    IMAGE    m_tIdle;
    IMAGE   m_tAttack1;
    IMAGE   m_tAttack2;
    IMAGE   m_tNowState;
    IMAGE   m_tChase;

    float m_fAttackRange;
    float m_fChaseRange;
    ULONGLONG m_ulTime;

    BEHAVOR_STATE m_BossState;

    int m_iCount;
};

