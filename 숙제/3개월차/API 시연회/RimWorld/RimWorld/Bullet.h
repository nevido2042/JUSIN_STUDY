#pragma once

#include "Obj.h"

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

	// CObj을(를) 통해 상속됨
	
};

