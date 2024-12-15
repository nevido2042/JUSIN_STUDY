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


    //�߻� �� ���� �ð�
    float       m_fElapsedTimeSinceShot;

    //���� �ӵ�
    float       m_fReloadSpeed;

    //�߻� �ӵ�(������)
    float       m_fFireRate;

    //�߻�ü
    CObj*       m_pProjectile;
    //���� �����Ÿ�
    float       m_fRange;
};

