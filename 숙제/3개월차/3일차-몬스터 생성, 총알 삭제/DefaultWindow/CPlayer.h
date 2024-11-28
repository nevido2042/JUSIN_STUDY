#pragma once
#include "CObj.h"

class CPlayer :    public CObj
{
public:
    CPlayer();
    virtual ~CPlayer();

public:
    void        Set_Bullet(list<CObj*>* pBullet) { m_pBullet = pBullet; }
 
public:
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;

private:
    void        Key_Input();
    CObj* Create_Bullet(DIRECTION eDir);
    
    //플레이어를 화면안에 가두는 함수
    void Clamp_PlayerToScreen();
private:
    list<CObj*>*        m_pBullet;



};

