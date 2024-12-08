#pragma once

#include "CObj.h"

enum Check_Bullet { Default, Skill_1, Skill_2 };

class CBullet : public CObj
{
public:
	CBullet();
	virtual ~CBullet();

public:	
	virtual void Initialize()	 override;
	virtual int Update()		 override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release()		 override;

	void Default_Bullet();
	void Skill_F1_Bullet();
	void Skill_F2_Bullet();


	Check_Bullet Check_Bul;

	CObj* m_Bu_Monster;

	float m_fAngleSpeed;
	float f_Bullet_Ang;
	
};

