#include "pch.h"
#include "CCollisionMgr.h"
#include "CPlayer.h"  
#include "Boss_FireMan.h"






void CCollisionMgr::Collision_Rect(list<CObj*> _Dst, list<CObj*> _Src)
{
	RECT rc{};// 충돌한걸 저장할 RECT 구조체

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			// 두 객체의 RECT 충돌 여부 확인
			if (IntersectRect(&rc, Dst->Get_Rect(), Src->Get_Rect()))
			{
				// 충돌 발생 시 둘다 조져버리기
				Dst->Set_Dead();
				Src->Set_Dead();
			}
		}
	}

}
 //
// 원 충돌 처리
//_Dst와 _Src 리스트 간의 모든 객체에 대해 원형 충돌 여부를 확인
// - 충돌 발생 시 역시나 둘다 조지기
void CCollisionMgr::Collision_Circle(list<CObj*> _Dst, list<CObj*> _Src)
{
	RECT rc{};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			// 원 충돌 여부 확인
			if (Check_Circle(Dst, Src))
			{
				// 충돌 발생 시 타노스
				Dst->Set_Dead();
				Src->Set_Dead();
			}
		}
	}
}

// 원 충돌 여부 확인 함수
//두 객체의 중심 좌표 간의 거리와 반지름 합을 비교하여 충돌 여부 판단
bool CCollisionMgr::Check_Circle(CObj* _Dst, CObj* _Src)
{
	float fRadius = (_Dst->Get_Info().fCX + _Src->Get_Info().fCX) * 0.5f;//반지름 합

	float fWidth  = abs(_Dst->Get_Info().fX - _Src->Get_Info().fX);// x축 거리
	float fHeight = abs(_Dst->Get_Info().fY - _Src->Get_Info().fY);// y축 거리

	float fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);//중심 사이 거리

	return fRadius >= fDiagonal; // 반지릅의 합이 중심 거리보다 크거나 같을때 타노스
}

//사각형 충돌처리
// - 충돌 발생 시 객체의 위치를 밀어내는 방식으로 처리
//void CCollisionMgr::Collision_RectEx(list<CObj*> _Dst, list<CObj*> _Src)
//{
//	float	fX(0.f), fY(0.f);// ?
//
//	for (auto& Dst : _Dst)
//	{
//		for (auto& Src : _Src)
//		{
//			if (Check_Rect(Dst, Src, &fX, &fY)) 
//			{
//				// 상 하 충돌
//				if (fX > fY)
//				{
//					// 상 충돌
//					if (Dst->Get_Info().fY < Src->Get_Info().fY)
//					{
//						Dst->Set_PosY(-fY);
//					}
//					// 하 충돌
//					else
//					{
//						Dst->Set_PosY(+fY);
//					}
//				}
//				
//				// 좌 우 충돌
//				else
//				{
//					// 좌 충돌
//					if (Dst->Get_Info().fX < Src->Get_Info().fX)
//					{
//						Dst->Set_PosX(-fX);
//					}
//					// 우 충돌
//					else
//					{
//						Dst->Set_PosX(fX);
//					}
//				}
//			}
//		}
//	}
//}
// 
// 
// - 충돌 발생 시 객체의 위치를 밀어내는 방식으로 처리? ( 무결코드 ) 위 주석 코드랑 비교
void CCollisionMgr::Collision_RectEx(list<CObj*> _Dst, list<CObj*> _Src)
{
	float fX(0.f), fY(0.f); // 충돌로 인한 밀림 거리

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_Rect(Dst, Src, &fX, &fY)) // 충돌 여부 확인
			{
				// 좌측 충돌
				if (Dst->Get_Info().fX < Src->Get_Info().fX)
				{
					Dst->Set_PosX(-fX); // x축 위치 조정
				}
				// 우측 충돌
				else
				{
					Dst->Set_PosX(fX); // x축 위치 조정
				}

				Dst->Update_Rect(); 
			}
		}
	}
}

bool CCollisionMgr::Check_Rect(CObj* _Dst, CObj* _Src, float* pX, float* pY)
{
    float fX = abs(_Dst->Get_Info().fX - _Src->Get_Info().fX); // x축 거리
    float fY = abs(_Dst->Get_Info().fY - _Src->Get_Info().fY); // y축 거리

    float fRadiusX = (_Dst->Get_Info().fCX + _Src->Get_Info().fCX) * 0.5f; // x축 반지름 합
    float fRadiusY = (_Dst->Get_Info().fCY + _Src->Get_Info().fCY) * 0.5f; // y축 반지름 합

    if ((fRadiusX >= fX) && (fRadiusY >= fY)) // 충돌 여부 확인
    {
        *pX = fRadiusX - fX; // x축 밀림 거리 계산
        *pY = fRadiusY - fY; // y축 밀림 거리 계산
        return true; // 충돌 발생
    }
    return false; // 충돌 없음
}
//(무결코드) 
// 박스 충돌 처리
// - 플레이어와 박스의 충돌 여부를 확인하고, 충돌 시 점프 상태와 위치를 조정?
void CCollisionMgr::Collision_Box(CObj* _Player, list<CObj*> _Src)
{
	RECT rc{};
	for (auto& Src : _Src)
	{
		if (IntersectRect(&rc, _Player->Get_Rect(), Src->Get_Rect()))//좌우 충돌만 판단
		{
			if (_Player->Get_Info().fX >= Src->Get_Info().fX)
			{
				_Player->Set_PosX(rc.right - rc.left);
			}
			else
			{
				_Player->Set_PosX(-(rc.right - rc.left));
			}
			_Player->Update_Rect();
		}
	}
}
void CCollisionMgr::Collision_Celling(CObj* _pPlayer, list<CObj*> _Src)
{
	float Head = _pPlayer->Get_Info().fY - _pPlayer->Get_Info().fCY * 0.5f;
	float CerrentY(0), ReturnY(0);
	float fDis(0), fMinDis(WINCY);

	for (auto Block : _Src)
	{
		if ((_pPlayer->Get_Info().fX >= Block->Get_Rect()->left - 10 &&
			_pPlayer->Get_Info().fX < Block->Get_Rect()->right + 10))
		{
			CerrentY = Block->Get_Rect()->bottom;
			fDis = fabsf(CerrentY - Head);
			if (CerrentY <= Head && fMinDis > fDis)
			{
				fMinDis = fDis;
				ReturnY = CerrentY;
			}
		}
	}

	if (ReturnY == 0.f)
		_pPlayer->Set_CellingY(0.f);
	else
		_pPlayer->Set_CellingY(ReturnY + _pPlayer->Get_Info().fCY * 0.5f);

}
//보스 전용 // 내일 손볼예정
void CCollisionMgr::Collision_Boss_Box(CObj* _Boss, list<CObj*> _Src)
{
	RECT rc{};
	float Y(0);// y축 위치 조정 값
	float X(0);
	for (auto& Src : _Src)
	{// 네모가 충돌하면 네모가 생기는데 그 네모가 rc에 넣어줌
		if (IntersectRect(&rc, _Boss->Get_Rect(), Src->Get_Rect()))//블럭이랑 충돌시
		{
			Y = (rc.top) - _Boss->Get_Info().fCY * 0.5;
			_Boss->Set_Pos(_Boss->Get_Info().fX, Y);
		/*	dynamic_cast<CBoss_FireMan*>(_Boss)->Set_Boss_Ground(true);
			dynamic_cast<CBoss_FireMan*>(_Boss)->Set_Boss_Jump(true);*/
			_Boss->Update_Rect();
		}

	}
}
//(무결코드)
// 점프 충돌 처리
// - 플레이어가 점프 중 Src랑 충돌하여 땅에 있는 상태를 판단
void CCollisionMgr::Collision_Jump(CObj* _Player, list<CObj*> _Src)
{
	RECT rc{}; 
	CPlayer* _Dst = dynamic_cast<CPlayer*>(_Player);
	float Y(0);
	for (auto& Src : _Src)
	{
		if (IntersectRect(&rc, _Dst->GetColRect(), Src->Get_Rect()))
		{
			_Dst->Set_Ground(true); //\ 땅에 있을 때 트루
			break;
		}
		_Dst->Set_Ground(false);
	}
}
void CCollisionMgr::Collision_Player_Monster(CObj* _pPlayer, list<CObj*> _Monster)
{
	RECT rc({});
	DIRECTION eDir(DIR_NONE);

	for (auto& pMonster : _Monster)
	{
		if (IntersectRect(&rc, _pPlayer->Get_Rect(), pMonster->Get_Rect()))
		{
			if (_pPlayer->Get_Info().fX > pMonster->Get_Info().fX)
				eDir = DIR_LEFT;
			else
				eDir = DIR_RIGHT;

			static_cast<CPlayer*>(_pPlayer)->Set_Damaged(eDir);
		}
	}
}


float CCollisionMgr::Collision_RangeChack(CObj* _pPlayer, CObj* _pMonster)
{
	float fWidth = fabsf(_pMonster->Get_Info().fX - _pPlayer->Get_Info().fX);
	float fHeight = fabsf(_pMonster->Get_Info().fY - _pPlayer->Get_Info().fY);

	float fDistance = sqrtf(fWidth * fWidth + fHeight * fHeight);

	return fDistance;

}

void CCollisionMgr::Collision_Floor(CObj* _pPlayer, list<CObj*> _Src)
{
	float Foot = _pPlayer->Get_Info().fY + _pPlayer->Get_Info().fCY * 0.5f;
	float CerrentY(0), ReturnY(0);
	float fDis(0), fMinDis(WINCY);

	for (auto Block : _Src)
	{
		if ((_pPlayer->Get_Info().fX >= Block->Get_Rect()->left - 10 &&
			_pPlayer->Get_Info().fX < Block->Get_Rect()->right + 10))
		{
			CerrentY = Block->Get_Rect()->top;
			fDis = fabsf(CerrentY - Foot);
			if (CerrentY >= Foot && fMinDis > fDis)
			{
				fMinDis = fDis;
				ReturnY = CerrentY;
			}
		}
	}

	if (ReturnY == 0.f)
		static_cast<CPlayer*>(_pPlayer)->Set_LineY(0.f);
	else
		static_cast<CPlayer*>(_pPlayer)->Set_LineY(ReturnY - _pPlayer->Get_Info().fCY * 0.5f);

}
