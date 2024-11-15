#pragma once
#include "Obj.h"

class CDungeon abstract :
    public CObj
{
protected:
    CObj* m_pMonster;
public:
    CDungeon();
    virtual ~CDungeon();
public:
    // CObj을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Relase() override;
public:
    // CObj을(를) 통해 상속됨
    void Render() override;
protected:
    virtual void Spawn_Monster() = 0;
public:
    void Start_Battle(CObj* m_pPlayer);
};

