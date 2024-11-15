#include "pch.h"
#include "NormalDungeon.h"
#include "NormalMonster.h"

void CNormalDungeon::Initialize()
{
	strcpy_s(m_szName, "중급 던전");

	Spawn_Monster();
}

void CNormalDungeon::Spawn_Monster()
{
	m_pMonster = new CNormalMonster();
	m_pMonster->Initialize();
}
