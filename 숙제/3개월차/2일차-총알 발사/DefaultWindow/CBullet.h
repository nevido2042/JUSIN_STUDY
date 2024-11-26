#pragma once

#include "CObj.h"

class CBullet : public CObj
{
public:
	enum Direction { UP = 0, LEFT, DOWN, RIGHT };
public:
	CBullet();
	virtual ~CBullet();

public:	
	virtual void Initialize()	 override;
	virtual void Update()		 override;
	virtual void Render(HDC hDC) override;
	virtual void Release()		 override;
public:
	void Set_Direction(Direction _Direction)
	{
		m_Direction = _Direction;
	}
private:
	Direction m_Direction;
};

