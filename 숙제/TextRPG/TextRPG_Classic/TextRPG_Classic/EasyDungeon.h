#pragma once
#include "Dungeon.h"
class CEasyDungeon :
    public CDungeon
{
    
public:
    CEasyDungeon();
    virtual ~CEasyDungeon();

public:
    void Initialize() override;

public:
    void Spawn_Monster() override;
};

