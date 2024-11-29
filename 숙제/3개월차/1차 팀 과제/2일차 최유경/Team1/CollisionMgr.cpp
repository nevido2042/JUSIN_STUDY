#include "pch.h"
#include "CollisionMgr.h"

void CollisionMgr::Collision_Rect(list<Obj*> _Dst, list<Obj*> _Src)
{
	RECT rc{};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, Dst->Get_Rect(), Src->Get_Rect()))
			{
				Dst->Add_HP(-1);
				Src->Add_HP(-1);
				//Dst->Set_Dead();
				//Src->Set_Dead();
			}
		}
	}
}

void CollisionMgr::Collision_Circle(list<Obj*> _Dst, list<Obj*> _Src)
{
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			float ax = Dst->Get_Info().fX - Src->Get_Info().fX;
			float ay = Dst->Get_Info().fY - Src->Get_Info().fY;
			float getVec = (float)sqrt((ax * ax) + (ay * ay));
			if (getVec < (Dst->Get_Info().fCX * 0.5f) + Src->Get_Info().fCX * 0.5f) {

				Dst->Add_HP(-1);
				Src->Add_HP(-1);

				//Dst->Set_Dead();
				//Src->Set_Dead();
			}
		}
	}
}
