#pragma once
#include "Obj.h"
class Monster :public Obj
{
public: 
	Monster();
	~Monster();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;
private:
	int m_dir;
	list<Obj*> m_BulletList;
public:
	Obj* Create_Bullet();
	//void Make_Pattern(); ÃÑ ¿©·¯ ¹æ¹ýÀ¸·Î ½î°Ô²û
};

