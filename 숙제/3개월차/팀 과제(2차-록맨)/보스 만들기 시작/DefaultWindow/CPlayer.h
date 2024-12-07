#pragma once
#include "CObj.h"
#include "CShield.h"
#include "CScrewBullet.h"
#include "CGuideBullet.h"

class CPlayer :    public CObj
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

    void Create_Bullet(); // ���� �ڵ�
    void Set_Damaged(const DIRECTION& _eDir); // �����ڵ�

public:
    void SetHeight(float _height) { m_pGroundHeight = _height; }//����

private:
    void        Key_Input();
    void        Jumping();
    void        Offset();

    void        CustomJumping(); // ����

public:
    RECT* GetColRect() // ����,�÷��̾��� ���� üũ�� �簢�� ��ȯ
    {
        return &m_CollisionRect;
    }
    INFO* GetColBox() // ����,�浹 �ڽ� ��ȯ
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
    void SetJump(bool _isjump) { m_bJump = _isjump; }//����,�������¼���
    bool GetJump() { return m_bJump; }//����, �������¹�ȯ

private:
    POINT               m_tPosin;
    bool                m_bJump;
    float               m_fJumpPower;
    float               m_fTime;

public: // ���Შ �״�� �ϱ� �ߴµ� ���߿� �����̺����� �ٲٰ� ���ؾ��ҵ�

    INFO                m_CollisionBox; // ����,�浹 �ڽ� ����
    RECT                m_CollisionRect; // ����,�浹 �簢�� ����
    bool                m_bisGround; //����,�÷��̾��� ���� üũ�� 
    float              m_pGroundHeight;//����,���� ����

    ///////////////////////////////////////////////

    int                 m_iScroll_Move;

    bool                m_bDamaged;
    ULONGLONG           m_ullDamaged_Time;


    bool                m_bBlink;
    ULONGLONG           m_ullBlink;

    int                 m_iBullet_Cooltime;
    ULONGLONG           m_ullLast_Bullet;

    INT                 m_iHp;

    BULLET_ID           m_eBullet_ID;
};

