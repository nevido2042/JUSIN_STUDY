#pragma once
#include "CBullet.h"
class CCannonBullet :
    public CBullet
{
public:
    CCannonBullet();
    virtual ~CCannonBullet();
public:
    virtual int Update() override;
    virtual void Initialize() override;

private:
    float m_fTime;
};

