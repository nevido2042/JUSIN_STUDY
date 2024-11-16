#include "pch.h"
#include "Item.h"

CItem::CItem()
	:m_iPrice(0)
{
}

CItem::~CItem()
{
}

void CItem::Initialize()
{
}

void CItem::Update()
{
}

void CItem::Relase()
{
}

void CItem::Render()
{
	cout << m_szName << endl;
}
