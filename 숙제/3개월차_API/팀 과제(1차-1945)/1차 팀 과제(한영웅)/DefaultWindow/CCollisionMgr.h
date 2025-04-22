#pragma once

#include "CObj.h"
#include "Define.h"

class CCollisionMgr
{
public:
	static void	Collision_Rect(list<CObj*> _Dst, list<CObj*> _Src);
	static void	Collision_Circle(list<CObj*> _Dst, list<CObj*> _Src);
	//bool Check_Collision(����1, ����2, ������1, ������2)
	static bool Check_Circle(CObj* _Dst, CObj* _Src);

};

