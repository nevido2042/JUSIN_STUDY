#pragma once
#include "Dungeon.h"
class CNormalDungeon :
    public CDungeon
{

public:
    void Initialize() override;
    // CDungeon��(��) ���� ��ӵ�
    void Spawn_Monster() override;
};

