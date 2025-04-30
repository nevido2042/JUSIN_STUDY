#include "pch.h"
#include "EasyDungeon.h"
#include "EasyMonster.h"

CEasyDungeon::CEasyDungeon()
{
}

CEasyDungeon::~CEasyDungeon()
{
}

void CEasyDungeon::Initialize()
{
	strcpy_s(m_szName, "초급 던전");

	Spawn_Monster();
}

void CEasyDungeon::Spawn_Monster()
{
	m_pMonster = new CEasyMonster;
	m_pMonster->Initialize();
}
