#pragma once
#include "CObj.h"

class CPlayer :    public CObj
{
public:
    CPlayer();
    virtual ~CPlayer();

public:
    void        Set_Bullet(list<CObj*>* pBullet) { m_pBulletList = pBullet; }
 
public:
    void Initialize() override;
    void Update() override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    void        Key_Input();
    CObj*       Create_Bullet();

private:
    list<CObj*>*        m_pBulletList;
    ULONGLONG m_ullLast_Fire;

};

