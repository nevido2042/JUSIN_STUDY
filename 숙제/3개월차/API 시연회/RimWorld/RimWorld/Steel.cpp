#include "pch.h"
#include "Steel.h"

void CSteel::Initialize()
{
	CItem::Initialize();

	Set_ImgKey(L"Steel_b");
	Set_Size(64.f, 64.f);
	m_eRenderID = RENDER_GAMEOBJECT;
}
