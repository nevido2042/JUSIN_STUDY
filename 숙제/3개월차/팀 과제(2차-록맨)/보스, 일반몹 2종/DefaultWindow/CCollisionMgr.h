#pragma once

#include "CObj.h"
#include "Define.h"

class CCollisionMgr
{
public:
    // 사각형 충돌 처리 충돌 발생 시 양쪽 타노스
    static void Collision_Rect(list<CObj*> _Dst, list<CObj*> _Src);

    // 원 충돌 처리, 충돌 발생 시 양쪽 타노스
    static void Collision_Circle(list<CObj*> _Dst, list<CObj*> _Src);

    // 원 충돌 여부 확인
    static bool Check_Circle(CObj* _Dst, CObj* _Src);

    //선생님 코드
    static void Collision_RectEx_Base(OBJID _Dst, OBJID _Src);

    // 사각형 충돌 처리: 충돌 후 위치 조정
    static void Collision_RectEx(list<CObj*> _Dst, list<CObj*> _Src);

    // 사각형 충돌 여부 확인
    static bool Check_Rect(CObj* _Dst, CObj* _Src, float* pX, float* pY);

    // 플레이어와 다른 객체 간의 박스 충돌 처리? (무결코드)
    static void Collision_Box(CObj* _Player, list<CObj*> _Src);

    // 점프 중 충돌 처리: 플레이어와 객체 간 충돌로 인해 점프 여부 결정? (무결코드)
    static void Collision_Jump(CObj* _Player, list<CObj*> _Src);

    // 플레이어가 몬스터한테 뽀뽀할때
    static void Collision_Player_Monster(CObj* _pPlayer, list<CObj*> _Monster);
};

