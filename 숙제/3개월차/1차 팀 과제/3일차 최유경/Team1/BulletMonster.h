#pragma once
#include "Monster.h"
class BulletMonster :
    public Monster
{
public:
    BulletMonster();
    virtual ~BulletMonster();
public:
    void        Set_BulletList(list<Obj*>* _pBulletList) { m_pBulletList = _pBulletList; }
    void        CreateBullet();
public:
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;

private:
    list<Obj*>* m_pBulletList;
    ULONGLONG m_ullLastShot;
};

