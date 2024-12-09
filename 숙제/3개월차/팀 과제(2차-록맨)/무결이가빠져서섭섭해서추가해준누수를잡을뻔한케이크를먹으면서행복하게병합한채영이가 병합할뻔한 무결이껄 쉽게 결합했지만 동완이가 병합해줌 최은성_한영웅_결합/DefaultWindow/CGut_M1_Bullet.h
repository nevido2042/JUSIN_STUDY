#pragma once
#include "CBullet.h"
class CGut_M1_Bullet : public CBullet
{
public:
	CGut_M1_Bullet() :m_fJumpPower(0), m_fTime(0), m_iHp(0) {}
	virtual ~CGut_M1_Bullet() { Release(); }

public:
	void Initialize() override;
	void Initialize_Animation();
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
private:
	float	m_fJumpPower;
	float	m_fTime;
	int		m_iHp;
};

