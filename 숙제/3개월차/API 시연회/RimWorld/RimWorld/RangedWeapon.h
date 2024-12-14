#pragma once
#include "Obj.h"
class CRangedWeapon abstract:
    public CObj
{
public:
    CRangedWeapon();
    virtual ~CRangedWeapon();

public:
    virtual void    Fire();
protected:
    bool            Try_Fire();

public:
    float           Get_Range() { return m_fRange; }
protected:
    void            Follow_Pawn();
public:
    void            Initialize() override;
    int             Update() override;
    void            Late_Update() override;
    virtual void    Render(HDC hDC) PURE;
    void            Release() override;

protected:
    //�߻� �ӵ�(������)
    int   m_iFireRate;
    //������ �߻� �ð�(������)
    int   m_iLastFireTime;
    //�߻�ü
    CObj*       m_pProjectile;
    //���� �����Ÿ�
    float       m_fRange;
};

