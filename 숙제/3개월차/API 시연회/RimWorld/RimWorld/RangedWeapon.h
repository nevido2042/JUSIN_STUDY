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
    //�߻� �ӵ�
    ULONGLONG   m_ullFireRate;
    //������ �߻� �ð�
    ULONGLONG   m_ullLastFireTime;
    //�߻�ü
    CObj*       m_pProjectile;
    //���� �����Ÿ�
    float       m_fRange;
};

