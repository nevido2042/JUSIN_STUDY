#pragma once

#include "Bullet.h"

class ScrewBullet : public Bullet
{
public:
	ScrewBullet(): m_fRotation(0) {}
	ScrewBullet(float _fAngle) : m_fRotation(0) { m_fAngle = _fAngle; }
	virtual ~ScrewBullet() {}
	

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _hdc) override;
	void Release() override;
	

private:
	float m_fRotation;
	

};

