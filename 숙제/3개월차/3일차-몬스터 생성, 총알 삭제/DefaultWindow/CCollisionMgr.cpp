#include "pch.h"
#include "CCollisionMgr.h"
#include "math.h"

void CCollisionMgr::Check_Collision(list<CObj*> _Dst, list<CObj*> _Src)
{
	//RECT rc{};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			//여기서 피타고라스
			// 숙제 2. 피타고라스의 정리를 이용한 원 충돌로 몬스터와 총알을 충돌처리를 구현해라.
			//bool Check_Collision(중점1, 중점2, 반지름1, 반지름2)
			/*if (IntersectRect(&rc, Dst->Get_Rect(), Src->Get_Rect()))
			{
				Dst->Set_Dead();
				Src->Set_Dead();
			}*/
			if (Check_Collision_ByRadius(
				Dst->Get_Info().fX, Dst->Get_Info().fY,
				Src->Get_Info().fX, Src->Get_Info().fY,
				(Dst->Get_Rect()->right - Dst->Get_Rect()->left) * 0.5f,
				(Src->Get_Rect()->right - Src->Get_Rect()->left) * 0.5f))
			{
				Dst->Set_Dead();
				Src->Set_Dead();
			}
		}
	}

}

bool CCollisionMgr::Check_Collision_ByRadius(float _fX1, float _fY1, float _fX2, float _fY2, LONG _fR1, LONG _fR2)
{
	if (pow((_fX2 - _fX1), 2) + pow((_fY2 - _fY1), 2) <= pow((_fR2 + _fR1), 2))
	{
		return true;
	}
	else
	{
		return false;
	}
}
