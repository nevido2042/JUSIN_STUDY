#pragma once
#include "Bullet.h"
class ScrewBullet :
    public Bullet
{
public:
	ScrewBullet();
	ScrewBullet(float _fAngle);
	virtual ~ScrewBullet();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;

private:
	POINT m_tCenter;
	float m_fRotSpeed;
	float m_fRotAngle;
	bool m_bStart;
};

