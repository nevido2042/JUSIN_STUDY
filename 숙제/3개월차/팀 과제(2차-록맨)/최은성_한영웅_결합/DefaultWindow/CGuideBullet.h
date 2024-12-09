#pragma once
#include "CObj.h"
class CGuideBullet :  public CObj
{
public:
	CGuideBullet();
	virtual ~CGuideBullet();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
};

