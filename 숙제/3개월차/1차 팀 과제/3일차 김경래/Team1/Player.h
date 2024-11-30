#pragma once
#include "Obj.h"
class Player : public Obj
{
public:
	Player();
	~Player();
public:
	void        Set_Bullet(list<Obj*>* pBullet) { m_BulletList = pBullet; }
	void		Set_Bullet_Level(int _iLevel) { m_iBulletLevel = _iLevel; }
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;
private:
	void Key_Input();
	void Calc_Angle();
	Obj* Create_Bullet(float radian);
	Obj* Create_Screw_Bullet(float radian);
	
private:
	list<Obj*>* m_BulletList;
	POINT m_tPosin;
	int m_iBulletLevel;
	int m_iFireRate; //총알 발사 속도
	int m_iTick; 
	int m_iStatus; // 무적 상태 만들어서 데미지 일정 간격으로 들어오게 
};

