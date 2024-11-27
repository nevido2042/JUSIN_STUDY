#pragma once

#include "CObj.h"
#include "Define.h"

class CCollisionMgr
{
public:
	static void	Check_Collision(list<CObj*> _Dst, list<CObj*> _Src);
	//bool Check_Collision(����1, ����2, ������1, ������2)
	static bool Check_Collision_ByRadius(float _fX1, float _fY1, float _fX2, float _fY2, LONG _fR1, LONG _fR2);

};

