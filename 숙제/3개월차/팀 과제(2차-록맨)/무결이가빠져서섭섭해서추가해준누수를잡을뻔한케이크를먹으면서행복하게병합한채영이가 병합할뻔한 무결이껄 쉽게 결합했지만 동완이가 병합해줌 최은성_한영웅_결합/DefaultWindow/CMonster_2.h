#pragma once
#include "CMonster.h"

enum MonsterState
{
    MONSTER_SEARCH,
    MONSTER_READY,
    MONSTER_ATTACK
};



class CMonster_2 : public CMonster
{
public:
	CMonster_2();
	~CMonster_2();

public:

    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;


private:
    HDC		hMemDC; // ��Ʈ�� ������������ �̸�����
    float fGravity;
    CObj* m_pPlayer;

    MonsterState m_eMonsterState;

};

