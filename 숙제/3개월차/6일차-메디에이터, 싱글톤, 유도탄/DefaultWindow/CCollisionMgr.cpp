#include "pch.h"
#include "CCollisionMgr.h"

void CCollisionMgr::Collision_Rect(list<CObj*> _Dst, list<CObj*> _Src)
{
	RECT rc{};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, Dst->Get_Rect(), Src->Get_Rect()))
			{
				Dst->Set_Dead();
				Src->Set_Dead();
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
				Dst->Set_Dead();
				Src->Set_Dead();
			}
		}
	}
}

void CCollisionMgr::Collision_CircleEx(list<CObj*> _Dst, list<CObj*> _Src)
{
	float	fR(0.f);

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_Circle(Dst, Src, &fR))
			{
				//dst가 움직임
				//dst가 소스의 반대방향으로 fR만큼 후퇴
				
				float fWidth(0.f), fHeight(0.f), fDiagonal(0.f), fRadian(0.f), fAngle(0.f);

				fWidth = Dst->Get_Info().fX - Src->Get_Info().fX;
				fHeight = Dst->Get_Info().fY - Src->Get_Info().fY;
				fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

				fRadian = acosf(fWidth / fDiagonal);
				fAngle = fRadian * (180 / PI);

				if (Dst->Get_Info().fY < Src->Get_Info().fY)
				{
					fAngle = 360.f - fAngle;
				}

				Dst->Set_PosX(fR * cosf(fAngle * PI / 180.f));
				Dst->Set_PosY(fR * sinf(fAngle * PI / 180.f));
			}
		}
	}
}

bool CCollisionMgr::Check_Circle(CObj* _Dst, CObj* _Src, float* pR)
{
	float fRadius = (_Dst->Get_Info().fCX + _Src->Get_Info().fCX) * 0.5f;

	float fWidth = abs(_Dst->Get_Info().fX - _Src->Get_Info().fX);
	float fHeight = abs(_Dst->Get_Info().fY - _Src->Get_Info().fY);

	float fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

	if (fRadius >= fDiagonal)
	{
		*pR = fRadius - fDiagonal;
		return true;
	}
	else
	{
		false;
	}
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
						Dst->Set_PosY(-fY);
					}
					// 하 충돌
					else
					{
						Dst->Set_PosY(+fY);
					}
				}
				
				// 좌 우 충돌
				else
				{
					// 좌 충돌
					if (Dst->Get_Info().fX < Src->Get_Info().fX)
					{
						Dst->Set_PosX(-fX);
					}
					// 우 충돌
					else
					{
						Dst->Set_PosX(fX);
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
