#include "pch.h"
#include "ItemFactory.h"

void CItemFactory::Initialize()
{
	Set_Name("아이템 공장");
}

void CItemFactory::Update()
{
}

void CItemFactory::Release()
{
}

void CItemFactory::Render()
{
}

CItem* CItemFactory::Create_Item(const char* _szName)
{
	if (!strcmp(_szName, "class CEasySword"))
	{
		return new CEasySword();
	}
	else if (!strcmp(_szName, "class CNormalSword"))
	{
		return new CNormalSword();
	}
	else if (!strcmp(_szName, "class CHardSword"))
	{
		return new CHardSword();
	}
	else if (!strcmp(_szName, "class CEasyArmor"))
	{
		return new CEasyArmor();
	}
	else if (!strcmp(_szName, "class CNormalArmor"))
	{
		return new CNormalArmor();
	}
	else if (!strcmp(_szName, "class CHardArmor"))
	{
		return new CHardArmor();
	}

	return nullptr;
}
