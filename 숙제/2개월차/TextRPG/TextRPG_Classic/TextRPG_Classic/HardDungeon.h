#pragma once
#include "Dungeon.h"
class CHardDungeon :
    public CDungeon
{
public:
    void Initialize() override;
    // CDungeon을(를) 통해 상속됨
    void Spawn_Monster() override;
};

