#include "pch.h"
#include "HardMonster.h"

void CHardMonster::Initialize()
{
	Set_Name("고급 몬스터");
	Set_MaxHP(25);
	Set_STR(5);

	m_iMoney = 500;

	CPawn::Initialize();
}
