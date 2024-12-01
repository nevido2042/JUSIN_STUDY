#pragma once
#include "Obj.h"

class Monster :public Obj
{
public: 
	Monster();
	virtual ~Monster();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;
protected:
	int m_iDir;
	ULONGLONG m_ulTime;
};

