#pragma once

#include "CObj.h"

enum Check_Bullet { Default, Skill_1, Skill_2 };

class CBullet abstract : public CObj
{
public:
	CBullet();
	virtual ~CBullet();

public:	
	virtual void Initialize()PURE;
	virtual int Update()PURE;
	virtual void Late_Update()PURE;
	virtual void Render(HDC hDC)PURE;
	virtual void Release()PURE;

	void Default_Bullet();
	void Skill_F1_Bullet();
	void Skill_F2_Bullet();

	BULLET_ID m_eBulletId; // ¹«°á
	Check_Bullet Check_Bul;

	CObj* m_Bu_Monster;

	float m_fAngleSpeed;
	float f_Bullet_Ang;
	
};

