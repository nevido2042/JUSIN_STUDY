#include "pch.h"
#include "ArcitectBtn.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"

CArcitectBtn::CArcitectBtn()
	:m_bActivate(false), m_pCommandBtn(nullptr)
{
}

CArcitectBtn::~CArcitectBtn()
{
	Release();
}

void CArcitectBtn::Create_CommandBtn()
{
	if (m_pCommandBtn == nullptr)
	{
		CObj* pObj = CAbstractFactory<CCommandBtn>::Create(m_tInfo.fX, m_tInfo.fY - m_tInfo.fCY);
		CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pObj);
		m_pCommandBtn = pObj;
	}
}

void CArcitectBtn::Destroy_CommandBtn()
{
	if (m_pCommandBtn)
	{
		m_pCommandBtn->Set_Destroyed();
		m_pCommandBtn = nullptr;
	}
}

void CArcitectBtn::On_Click()
{
	if (m_bActivate)
	{
		m_bActivate = false;
		Destroy_CommandBtn();
	}
	else
	{
		m_bActivate = true;
		Create_CommandBtn();
	}
}

void CArcitectBtn::Initialize()
{
	Set_ImgKey(L"ButtonSubtleAtlas_Architect");

	m_tInfo.fCX = 128.f;
	m_tInfo.fCY = 32.f;
	
	m_tInfo.fX = m_tInfo.fCX * 0.5f;
	m_tInfo.fY = WINCY - m_tInfo.fCY * 0.5f;

	m_eRenderID = RENDER_UI;
}
