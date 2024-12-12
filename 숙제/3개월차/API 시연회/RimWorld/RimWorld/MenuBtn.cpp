#include "pch.h"
#include "MenuBtn.h"
#include "BmpMgr.h"
#include "KeyMgr.h"

CMenuBtn::CMenuBtn()
{
}

CMenuBtn::~CMenuBtn()
{
	Release();
}

void CMenuBtn::Initialize()
{
	Set_ImgKey(L"ButtonSubtleAtlas_Menu");

	m_tInfo.fCX = 64.f;
	m_tInfo.fCY = 32.f;

	m_tInfo.fX = WINCX - m_tInfo.fCX * 0.5f;
	m_tInfo.fY = WINCY - m_tInfo.fCY * 0.5f;
}
