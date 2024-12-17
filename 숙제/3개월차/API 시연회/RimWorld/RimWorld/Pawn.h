#pragma once
#include "Obj.h"
#include "Node.h"

#include "HealthBar.h"

class CPawn :
    public CObj
{
public:
    CPawn();
    virtual ~CPawn();
public:
    void Move_To(POS _Pos);
    void Take_Damage(float _fDamage);
public:
    float Get_HP() { return m_fHP; }
    float Get_MaxHP() { return m_fMaxHP; }
    float Get_IsDead() { return m_bDead; }
    float Get_TargetAngle() { return m_fTargetAngle; }
    bool  Get_IsAttack() { return m_bAttack; }
    bool  Get_IsNavigating() { return m_bNavigating; }
    
    void  Set_IsAttack(bool _bAttack) { m_bAttack = _bAttack; }
    void  RequestNavStop() { m_bNavStopRequested = true; }
protected:
    virtual void Dead();//�׾��� �� �� �ൿ

protected:
    void            Navigate();
    void            Calculate_MoveDir();
    void            Measure_Target();
    bool            IsWithinRange();//�����Ÿ��� �ִ°�?
    bool            IsCanSeeTarget();//��밡 ���̴°�?(Ÿ�ٰ� �ڽŻ��̿� ��ֹ��� ������ �Ⱥ��̴� ��)
    CObj*           Get_ObstacleToTarget();
    int             Get_ReachableToTarget();//Ÿ�ٱ��� ��������� ���� �� ���ִ� Ÿ�� ã��
    int             Find_NearestReachableTile(int iIndexX, int iIndexY);

public:
    void    Initialize() override;
    int     Update() override;
    void    Late_Update() override;
    void    Render(HDC hDC) PURE;
    void    Release() override;
    void    OnCollision(OBJID _eID, CObj* _pOther) override;

protected:
    //��ã�� ����
    list<CNode*>    m_NodeList;
    bool            m_bNavigating;
    bool            m_bNavStopRequested;//��ã�� ���� ��û�� ��� �Դ°�?
    //���� ���� ��(���� ������ ��ġ)
    POS             m_tPrevPos;
    //ü�� ����
    float           m_fHP;
    float           m_fMaxHP;
    bool            m_bDead;
    //���Ÿ� ����
    CObj*           m_pRangedWeapon;
    
    //Ÿ�ٰ��� �Ÿ�
    float           m_fTargetDist;
    //Ÿ�ٰ��� ����
    float           m_fTargetAngle;
    //���� �� �ΰ�?
    bool            m_bAttack;
};
