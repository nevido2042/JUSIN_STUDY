#pragma once
#include "CObj.h"
#include "CShield.h"
#include "CScrewBullet.h"
#include "CGuideBullet.h"

class CPlayer : public CObj
{
public:
    CPlayer();
    virtual ~CPlayer();

public:
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;
    void Initialize_Animation();
    void Create_Bullet(); // 은성 코드
    void Set_Damaged(const DIRECTION& _eDir); // 은성코드
    void Set_Fallen(const DIRECTION& _eDir); // 은성코드

public:
    void SetHeight(float _height) { m_pGroundHeight = _height; }//무결

private:
    void        Key_Input();
    void        Offset();

    //void        CustomJumping(); // 무결

public:
    RECT* GetColRect() // 무결,플레이어의 지면 체크용 사각형 반환
    {
        return &m_CollisionRect;
    }
    INFO* GetColBox() // 무결,충돌 박스 반환
    {
        return &m_CollisionBox;
    }
    void Set_Ground(bool _bisGround)
    {

        m_bisGround = _bisGround;

    }
    bool Get_Ground()
    {

        return m_bisGround;

    }
    void Set_fY(float _type)
    {
        m_tInfo.fY = _type;
    }
    void SetJump(bool _isjump) { m_bJump = _isjump; }//무결,점프상태설정
    bool GetJump() { return m_bJump; }//무결, 점프상태반환
    void Set_BulId(const BULLET_ID& _ID) { m_eBullet_ID = _ID; }

private:
    POINT               m_tPosin;
  

public: // 무결꺼 그대로 하긴 했는데 나중에 프라이빗으로 바꾸고 겟해야할듯

    INFO                m_CollisionBox; // 무결,충돌 박스 정보
    RECT                m_CollisionRect; // 무결,충돌 사각형 정보
    bool                m_bisGround; //무결,플레이어의 지면 체크용 
    float              m_pGroundHeight;//무결,지면 높이

    ///////////////////////////////////////////////
private:
    int                 m_iScroll_Move;

    bool                m_bDamaged;
    ULONGLONG           m_ullDamaged_Time;

    bool                m_bBlink;
    ULONGLONG           m_ullBlink;

    int                 m_iBullet_Cooltime;
    ULONGLONG           m_ullLast_Bullet;

    BULLET_ID           m_eBullet_ID;

    int                 m_iBulletCount[BUL_END];
};