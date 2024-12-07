#pragma once

#include "CObj.h"
#include "Define.h"

class CCollisionMgr
{
public:
    // �簢�� �浹 ó�� �浹 �߻� �� ���� Ÿ�뽺
    static void Collision_Rect(list<CObj*> _Dst, list<CObj*> _Src);

    // �� �浹 ó��, �浹 �߻� �� ���� Ÿ�뽺
    static void Collision_Circle(list<CObj*> _Dst, list<CObj*> _Src);

    // �� �浹 ���� Ȯ��
    static bool Check_Circle(CObj* _Dst, CObj* _Src);

    // �簢�� �浹 ó��: �浹 �� ��ġ ����
    static void Collision_RectEx(list<CObj*> _Dst, list<CObj*> _Src);

    // �簢�� �浹 ���� Ȯ��
    static bool Check_Rect(CObj* _Dst, CObj* _Src, float* pX, float* pY);

    // �÷��̾�� �ٸ� ��ü ���� �ڽ� �浹 ó��? (�����ڵ�)
    static void Collision_Box(CObj* _Player, list<CObj*> _Src);

    // ���� �� �浹 ó��: �÷��̾�� ��ü �� �浹�� ���� ���� ���� ����? (�����ڵ�)
    static void Collision_Jump(CObj* _Player, list<CObj*> _Src);

    // �÷��̾ �������� �ǻ��Ҷ�
    static void Collision_Player_Monster(CObj* _pPlayer, list<CObj*> _Monster);
};

