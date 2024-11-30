#pragma once
#include "Obj.h"
class Bullet :public Obj
{
public:
	Bullet();
	Bullet(float _fAngle) : m_iType(BM_END){ m_fAngle = _fAngle; }
	~Bullet();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;

private:
	int m_iType;

public:
	void Set_Type(int _iType) { m_iType = _iType; }

};

