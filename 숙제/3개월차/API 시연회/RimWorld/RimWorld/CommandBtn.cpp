#include "pch.h"
#include "CommandBtn.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "ArcitectBtn.h"

CCommandBtn::CCommandBtn()
	:m_bActivate(false), m_pDeconstructBtn(nullptr)
{
}

CCommandBtn::~CCommandBtn()
{
	Release();
}

void CCommandBtn::Create_DeconstructBtn()
{
	if (m_pDeconstructBtn == nullptr)
	{
		CObj* pObj = CAbstractFactory<CDeconstructBtn>::Create(m_tInfo.fX + m_tInfo.fCX, m_tInfo.fY);
		CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pObj);
		m_pDeconstructBtn = pObj;
	}
}

void CCommandBtn::Destroy_DeconstructBtn()
{
	if (m_pDeconstructBtn)
	{
		m_pDeconstructBtn->Set_Destroyed();
		m_pDeconstructBtn = nullptr;
	}
}

void CCommandBtn::Set_Activate(bool _bActivate)
{
	if (_bActivate == false)
	{
		m_bActivate = _bActivate;
		Destroy_DeconstructBtn();
	}
}

void CCommandBtn::On_Click()
{
	if (m_bActivate)
	{
		m_bActivate = false;
		Destroy_DeconstructBtn();
	}
	else
	{
		CArcitectBtn* pArcitectBtn = static_cast<CArcitectBtn*>(m_pParent);
		//static_cast<CButton*>(pArcitectBtn->m_pStructureBtn)->Set_Activate(false);

		m_bActivate = true;
		Create_DeconstructBtn();
	}
}

void CCommandBtn::Release()
{
	m_bActivate = false;
	Destroy_DeconstructBtn();
}

void CCommandBtn::Initialize()
{
	Set_ImgKey(L"ButtonSubtleAtlas_Command");

	m_tInfo.fCX = 128.f;
	m_tInfo.fCY = 32.f;

	m_eRenderID = RENDER_UI;
}
