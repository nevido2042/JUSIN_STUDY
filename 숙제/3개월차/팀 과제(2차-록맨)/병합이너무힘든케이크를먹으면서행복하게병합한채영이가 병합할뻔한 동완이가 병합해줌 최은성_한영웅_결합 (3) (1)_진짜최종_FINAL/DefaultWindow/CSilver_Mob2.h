#pragma once
#include "CMonster.h"
class CSilver_Mob2 : public CMonster  // �����ٸ鼭 �÷��̾ �Ѿƿ�
{
public:
    CSilver_Mob2();
    virtual ~CSilver_Mob2() override;

public:
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;
    virtual void Jumping() override;
    virtual void	Jump_Pattern() override;

private:
    void Update_Animation();

    ULONGLONG m_ullTime;
    ULONGLONG m_ullCerrentTime;

    int     m_iAnimationX;
    int     m_iAnimationY;
    int     m_iInterY;

    POINT   m_tStartPoint;
};