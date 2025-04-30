#pragma once
#include "Bullet.h"
class BulletOne :public Bullet
{
public:
	BulletOne();
	BulletOne(float _fAngle) { m_fAngle = _fAngle; }
	virtual ~BulletOne();
public:
	virtual void Initialize() override;
	virtual int Update() override;
};

