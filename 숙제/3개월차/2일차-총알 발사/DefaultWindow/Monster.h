#pragma once
#include "CObj.h"
class CMonster :
    public CObj
{
    // CObj을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Render(HDC hDC) override;
    void Release() override;
public:
    void Add_HP(int _iAmount);
    bool Get_IsDead()
    {
        return m_bDead;
    }
private:
    bool m_bDead;
    bool m_bRight;
    int m_iHP;
};

