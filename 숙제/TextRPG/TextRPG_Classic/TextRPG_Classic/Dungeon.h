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
    // CObj��(��) ���� ��ӵ�
    void Initialize() override;
    void Update() override;
    void Relase() override;
public:
    // CObj��(��) ���� ��ӵ�
    void Render() override;
protected:
    virtual void Spawn_Monster() = 0;
public:
    void Start_Battle(CObj* m_pPlayer);
};

