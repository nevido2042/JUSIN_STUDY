#include "pch.h"
#include "ArcitectBtn.h"

CArcitectBtn::CArcitectBtn()
{
}

CArcitectBtn::~CArcitectBtn()
{
	Release();
}

void CArcitectBtn::Initialize()
{
	Set_ImgKey(L"ButtonSubtleAtlas_Architect");

	m_tInfo.fCX = 128.f;
	m_tInfo.fCY = 32.f;
	
	m_tInfo.fX = m_tInfo.fCX * 0.5f;
	m_tInfo.fY = WINCY - m_tInfo.fCY * 0.5f;

	m_eRender = RENDER_UI;
}
