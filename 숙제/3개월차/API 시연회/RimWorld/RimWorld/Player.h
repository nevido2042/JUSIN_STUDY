#pragma once
#include "Obj.h"
#include "Shield.h"
#include "ScrewBullet.h"
#include "GuideBullet.h"

class CPlayer :    public CObj
{
public:
    CPlayer();
    virtual ~CPlayer();

public:
    virtual void Initialize() override;
    virtual int Update() override;
    virtual int Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;

private:
    void        Key_Input();
    void        Jumping();
    void        Offset();

    CObj*       Create_Shield();

private:
    POINT               m_tPosin;

    bool                m_bJump;

    float               m_fJumpPower;
    float               m_fTime;



};

