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
	strcpy_s(m_szName, "�ʱ� ����");

	Spawn_Monster();
}

void CEasyDungeon::Spawn_Monster()
{
	m_pMonster = new CEasyMonster;
	m_pMonster->Initialize();
}
