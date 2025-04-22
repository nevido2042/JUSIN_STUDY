#pragma once
#include "CObj.h"
#include "CShield.h"
#include "CScrewBullet.h"
#include "CGuideBullet.h"
#include "Path.h"

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

private:
    void        Key_Input();
    CObj*       Create_Shield();
    bool        Follow_Path();

private:
    POINT       m_tPosin;

    CObj*       m_pPath;
    bool        m_bFollowingPath;
    list<POINT>::iterator m_IterTargetPoint;
    //float       m_fSlope; //필요 없음

    // CObj을(를) 통해 상속됨
    void Start() override;

};

