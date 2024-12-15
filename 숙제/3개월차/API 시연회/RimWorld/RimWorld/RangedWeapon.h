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


    //발사 후 지난 시간
    float       m_fElapsedTimeSinceShot;

    //장전 속도
    float       m_fReloadSpeed;

    //발사 속도(프레임)
    float       m_fFireRate;

    //발사체
    CObj*       m_pProjectile;
    //무기 사정거리
    float       m_fRange;
};

