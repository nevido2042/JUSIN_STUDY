#pragma once
#include "Obj.h"
class CollisionMgr
{
public:
	static bool	Collision_Rect(Obj* _Dst, Obj* _Src);
	static bool	Collision_Circle(Obj* _Dst, Obj* _Src);
	static void Damage_Rect(list<Obj*> _Dst, list<Obj*> _Src);
	static void Damage_Circle(list<Obj*> _Dst, list<Obj*> _Src);
};
