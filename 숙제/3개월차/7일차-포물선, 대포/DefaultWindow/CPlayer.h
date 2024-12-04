#pragma once
#include "CObj.h"
#include "CShield.h"
#include "CScrewBullet.h"
#include "CGuideBullet.h"
#include "CCannonBullet.h"

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

private:
    POINT               m_tPosin;



};

