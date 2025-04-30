#pragma once

#include "CObj.h"

class CBullet : public CObj
{
public:
	CBullet();
	virtual ~CBullet();

public:	
	virtual void Initialize()	 override;
	virtual void Update()		 override;
	virtual void Render(HDC hDC) override;
	virtual void Release()		 override;
};

