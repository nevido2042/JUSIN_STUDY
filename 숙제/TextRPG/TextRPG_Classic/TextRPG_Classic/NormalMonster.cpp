#include "pch.h"
#include "NormalMonster.h"

void CNormalMonster::Initialize()
{
	Set_Name("�߱� ����");
	Set_MaxHP(15);
	Set_STR(3);

	m_iMoney = 300;

	CPawn::Initialize();
}
