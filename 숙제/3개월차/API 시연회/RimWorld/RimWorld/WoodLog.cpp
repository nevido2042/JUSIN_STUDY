#include "pch.h"
#include "WoodLog.h"

void CWoodLog::Initialize()
{
	CItem::Initialize();

	Set_ImgKey(L"WoodLog_b");
	Set_Size(64.f, 64.f);
}
