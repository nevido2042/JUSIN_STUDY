#pragma once
#include "BulletOne.h"
class MonsterBullet :
    public BulletOne
{
public:
    MonsterBullet(float _fAngle) { m_fAngle = _fAngle; }
public:
    virtual void Render(HDC _hDC) override;

};

