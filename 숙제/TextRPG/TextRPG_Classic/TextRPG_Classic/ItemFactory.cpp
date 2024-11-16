#include "pch.h"
#include "ItemFactory.h"

void CItemFactory::Initialize()
{
	Set_Name("아이템 공장");
}

void CItemFactory::Update()
{
}

void CItemFactory::Relase()
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

	return nullptr;
}
