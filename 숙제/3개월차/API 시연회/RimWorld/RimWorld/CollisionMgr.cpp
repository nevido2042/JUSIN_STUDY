#include "pch.h"
#include "CollisionMgr.h"
#include "ObjMgr.h"

void CCollisionMgr::Collision_Rect(list<CObj*> _Dst, list<CObj*> _Src)
{
	RECT rc{};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, Dst->Get_Rect(), Src->Get_Rect()))
			{
				//Dst->OnCollision();
				Src->Set_Destroyed();
			}
		}
	}

}

void CCollisionMgr::Collision_Circle(list<CObj*> _Dst, list<CObj*> _Src)
{
	RECT rc{};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_Circle(Dst, Src))
			{
				Dst->Set_Destroyed();
				Src->Set_Destroyed();
			}
		}
	}
}

void CCollisionMgr::Collision_Circle(OBJID _eDst, OBJID _eSrc)
{
	RECT rc{};

	/*for (auto& Dst : CObjMgr::Get_Instance()->Get_List()[_eDst]);
	{
		for (CObj* Src : CObjMgr::Get_Instance()->Get_List()[_eSrc])
		{
			if (Check_Circle(Dst, Src))
			{
				Dst->Set_Destroyed();
				Src->Set_Destroyed();
			}
		}
	}*/
}

bool CCollisionMgr::Check_Circle(CObj* _Dst, CObj* _Src)
{
	float fRadius = (_Dst->Get_Info().fCX + _Src->Get_Info().fCX) * 0.5f;

	float fWidth  = abs(_Dst->Get_Info().fX - _Src->Get_Info().fX);
	float fHeight = abs(_Dst->Get_Info().fY - _Src->Get_Info().fY);

	float fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

	return fRadius >= fDiagonal;
}

void CCollisionMgr::Collision_RectEx(list<CObj*> _Dst, list<CObj*> _Src)
{
	float	fX(0.f), fY(0.f);

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_Rect(Dst, Src, &fX, &fY))
			{
				// 상 하 충돌
				if (fX > fY)
				{
					// 상 충돌
					if (Dst->Get_Info().fY < Src->Get_Info().fY)
					{
						Dst->Move_PosY(-fY);
					}
					// 하 충돌
					else
					{
						Dst->Move_PosY(+fY);
					}
				}
				
				// 좌 우 충돌
				else
				{
					// 좌 충돌
					if (Dst->Get_Info().fX < Src->Get_Info().fX)
					{
						Dst->Move_PosX(-fX);
					}
					// 우 충돌
					else
					{
						Dst->Move_PosX(fX);
					}
				}
			}
		}
	}
}

bool CCollisionMgr::Check_Rect(CObj* _Dst, CObj* _Src, float* pX, float* pY)
{

	float		fX = abs(_Dst->Get_Info().fX - _Src->Get_Info().fX);
	float		fY = abs(_Dst->Get_Info().fY - _Src->Get_Info().fY);

	float		fRadiusX = (_Dst->Get_Info().fCX + _Src->Get_Info().fCX) * 0.5f;
	float		fRadiusY = (_Dst->Get_Info().fCY + _Src->Get_Info().fCY) * 0.5f;

	if ((fRadiusX >= fX) && (fRadiusY >= fY))
	{
		*pX = fRadiusX - fX;
		*pY = fRadiusY - fY;
		return true;
	}

	return false;
}
