#pragma once
#include "CBullet.h"
#include "Define.h"

class CRolling_Cutter :
    public CBullet
{
public:
    CRolling_Cutter();
    virtual ~CRolling_Cutter();

    virtual void Initialize();
    virtual int Update();
    virtual void Late_Update();
    virtual void Render(HDC hDC);
    virtual void Release();

    void Set_Caster(CObj* _pCaster) { m_pCaster = _pCaster;}
protected :

    ULONGLONG m_ullCerrentTime;
    ULONGLONG m_ullTime;
    ULONGLONG m_ullAnimationTime;
    CObj*     m_pCaster;


    int     m_iAnimationFrame;
    int     m_iImagePose;
    int     m_iAnimationX;
    int     m_iAnimationY;
    bool    m_bCurve;

    void Update_Animation();
    void Set_Angle(INFO* _pTarget);
};

