#include "pch.h"
#include "EasyMonster.h"

void CEasyMonster::Initialize()
{
	Set_Name("�ʱ� ����");
	Set_MaxHP(5);
	Set_STR(1);

	m_iMoney = 100;

	CPawn::Initialize();
}
