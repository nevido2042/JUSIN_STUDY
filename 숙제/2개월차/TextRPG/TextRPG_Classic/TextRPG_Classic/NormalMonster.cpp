#include "pch.h"
#include "NormalMonster.h"

void CNormalMonster::Initialize()
{
	Set_Name("중급 몬스터");
	Set_MaxHP(15);
	Set_STR(3);

	m_iMoney = 300;

	CPawn::Initialize();
}
