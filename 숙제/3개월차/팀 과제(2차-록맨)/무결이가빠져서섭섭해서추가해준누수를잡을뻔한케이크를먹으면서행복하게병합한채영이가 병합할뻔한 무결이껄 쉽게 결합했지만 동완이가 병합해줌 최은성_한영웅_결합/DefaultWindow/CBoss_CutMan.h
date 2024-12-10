#pragma once
#include "CObj.h"
class CBoss_CutMan :
    public CObj
{
public:
    enum CUTMAN_ACTION {NONE,IDLE, RUN,SHOOT};

    CBoss_CutMan();
    virtual ~CBoss_CutMan();

    // CObj을(를) 통해 상속됨
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

    void Set_Boss_Ground(bool _bisGround)
    {
        m_bisGround = _bisGround;
    }
    bool Get_Boss_Ground()
    {
        return m_bisGround;
    }

    void Set_Boss_Jump(bool _isjump) { m_bJump = _isjump; }//동완,점프상태설정
    bool GetJump() { return m_bJump; }//동완, 점프상태반환
    void Set_Shoot() { m_bShoot = false; }

private:
    void Update_Animation();
    void Custom_Jumping();

    void Select_Action();

    void Action_Run();
    void Action_Idle();
    void Action_Shoot();

    void Action_Render(HDC hDC);

    ULONGLONG m_ullActionTimer;
    ULONGLONG m_ullCerrentTime;
    ULONGLONG m_ullAnimationFrame;

    bool m_bJump;
    bool m_bisGround;
    float m_fJumpPower;
    float m_fTime;

    int     m_iImagePose;
    int     m_iAnimationX;
    int     m_iAnimationY;

    CUTMAN_ACTION    m_eAction;
    bool    m_bShoot;
    bool    m_bRunAni;

    POINT  m_tAnimationSize;
};