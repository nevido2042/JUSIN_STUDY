#pragma once
#include "CMonster.h"
class CEggMonster : public CMonster
{
public:
	CEggMonster();
	~CEggMonster();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	float GetDistance();

private:
	void Patrol();
	CObj* Create_Bullet(float _y);

private:
	BEHAVOR_STATE m_eState;
	IMAGEINFO m_tMove;
	IMAGEINFO m_tAttack1;
	IMAGEINFO m_tAttack2;
	IMAGEINFO m_tNowState;

	float m_fDinstance;
	float m_iDirection;

private:
	float m_fMinY;
	float m_fMaxY;
	int m_iCount;
	list<CObj*>* m_pBulletList;

};

