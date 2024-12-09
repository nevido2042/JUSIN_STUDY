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

    void    isGround(); // ���߿� ���浹�� ȣ��Ǵ°ŷ� �ٲ����

private:
    void Update_Animation();
    void Set_Angle(INFO* _pTarget);

    ULONGLONG m_ullTime;
    ULONGLONG m_ullCerrentTime;

    int     m_iAnimationX;
    int     m_iAnimationY;

    float   m_fTime;

    POINT   m_tStartPoint;

    bool    m_bJump;
};