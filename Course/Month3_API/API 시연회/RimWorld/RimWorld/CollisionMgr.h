#pragma once

#include "Obj.h"
#include "Define.h"

class CCollisionMgr
{
public:
	static void	Collision_Rect(list<CObj*> _Dst, list<CObj*> _Src);

	static void	Collision_Circle(list<CObj*> _Dst, list<CObj*> _Src);
	static bool	Check_Circle(CObj* _Dst, CObj* _Src);

	static void	Collision_RectEx(list<CObj*> _Dst, list<CObj*> _Src);
	static bool	Check_Rect(CObj* _Dst, CObj* _Src, float* pX, float* pY);

	//�Ʒ��� ���� �� �ڵ�
	static void	Collision_Circle(OBJID _eDst, OBJID _eSrc);
};

