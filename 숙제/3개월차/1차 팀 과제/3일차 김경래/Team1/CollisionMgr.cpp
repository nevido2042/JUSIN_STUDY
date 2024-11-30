#include "pch.h"
#include "CollisionMgr.h"

bool CollisionMgr::Collision_Rect(Obj* _Dst, Obj* _Src)
{
	RECT rc{};

	
	return IntersectRect(&rc, _Dst->Get_Rect(), _Src->Get_Rect());
}

bool CollisionMgr::Collision_Circle(Obj* _Dst, Obj* _Src)
{
	float ax = _Dst->Get_Info().fX - _Src->Get_Info().fX;
	float ay = _Dst->Get_Info().fY - _Src->Get_Info().fY;
	float getVec = sqrtf((ax * ax) + (ay * ay));

	return (getVec < (_Dst->Get_Info().fCX * 0.5f) + _Src->Get_Info().fCX * 0.5f);
				
}

void CollisionMgr::Damage_Rect(list<Obj*> _Dst, list<Obj*> _Src)
{
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			
			if (Collision_Rect(Dst,Src)) 
			{
				Dst->Set_Hp(Dst->Get_Hp() - Src->Get_Damage());
				Src->Set_Hp(Src->Get_Hp() - Dst->Get_Damage());

			}
		}
	}
}

void CollisionMgr::Damage_Circle(list<Obj*> _Dst, list<Obj*> _Src)
{
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Collision_Circle(Dst, Src))
			{
				Dst->Set_Hp(Dst->Get_Hp() - Src->Get_Damage());
				Src->Set_Hp(Src->Get_Hp() - Dst->Get_Damage());

			}
		}
	}
}