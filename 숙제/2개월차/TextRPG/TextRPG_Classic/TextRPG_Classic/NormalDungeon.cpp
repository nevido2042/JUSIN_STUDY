#include "pch.h"
#include "NormalDungeon.h"
#include "NormalMonster.h"

void CNormalDungeon::Initialize()
{
	strcpy_s(m_szName, "�߱� ����");

	Spawn_Monster();
}

void CNormalDungeon::Spawn_Monster()
{
	m_pMonster = new CNormalMonster();
	m_pMonster->Initialize();
}
