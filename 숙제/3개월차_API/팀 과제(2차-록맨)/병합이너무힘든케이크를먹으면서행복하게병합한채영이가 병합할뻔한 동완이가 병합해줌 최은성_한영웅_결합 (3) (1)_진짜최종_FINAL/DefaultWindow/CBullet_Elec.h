#pragma once
#include "CBullet.h"
#include "Define.h"

class CBullet_Elec : public CBullet
{
public:
	CBullet_Elec();
	~CBullet_Elec();

	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	IMAGE m_tNowState;

	IMAGE m_tEelc_Bullet1;
	IMAGE m_tEelc_Bullet2;
	IMAGE m_tEelc_Bullet3;
	IMAGE m_tEelc_Bullet4;
	IMAGE m_tEelc_Bullet5;
	IMAGE m_tEelc_Bullet6;
	IMAGE m_tEelc_Bullet7;
	int iBulletCount;
	BULLET_ID m_eBulletId;
};

