#include "pch.h"
#include "HardDungeon.h"
#include "HardMonster.h"

void CHardDungeon::Initialize()
{
	strcpy_s(m_szName, "고급 던전");

	Spawn_Monster();
}

void CHardDungeon::Spawn_Monster()
{
	m_pMonster = new CHardMonster();
	m_pMonster->Initialize();
}
