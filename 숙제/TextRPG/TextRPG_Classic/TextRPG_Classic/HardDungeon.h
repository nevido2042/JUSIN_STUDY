#pragma once
#include "Dungeon.h"
class CHardDungeon :
    public CDungeon
{
public:
    void Initialize() override;
    // CDungeon��(��) ���� ��ӵ�
    void Spawn_Monster() override;
};

