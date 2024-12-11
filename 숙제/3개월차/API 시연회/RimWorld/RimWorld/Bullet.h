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
	virtual int Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release()		 override;

	// CObj��(��) ���� ��ӵ�
	
};

