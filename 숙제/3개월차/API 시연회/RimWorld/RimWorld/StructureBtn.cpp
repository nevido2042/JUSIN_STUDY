#include "pch.h"
#include "StructureBtn.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "ArcitectBtn.h"

CStructureBtn::CStructureBtn()
	:m_bActivate(false), m_pSteelBtn(nullptr)
{
}

CStructureBtn::~CStructureBtn()
{
	Release();
}

void CStructureBtn::Create_SteelWallBtn()
{
	if (m_pSteelBtn == nullptr)
	{
		CObj* pObj = CAbstractFactory<CSteelWallBtn>::Create(m_tInfo.fX + m_tInfo.fCX, m_tInfo.fY);
		CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pObj);
		m_pSteelBtn = pObj;
	}
}

void CStructureBtn::Destroy_SteelWallBtn()
{
	if (m_pSteelBtn)
	{
		m_pSteelBtn->Set_Destroyed();
		m_pSteelBtn = nullptr;
	}
}

void CStructureBtn::Initialize()
{
	Set_ImgKey(L"ButtonSubtleAtlas_Structure");

	m_tInfo.fCX = 128.f;
	m_tInfo.fCY = 32.f;

	m_eRenderID = RENDER_UI;
}

void CStructureBtn::On_Click()
{
	if (m_bActivate)
	{
		m_bActivate = false;
		Destroy_SteelWallBtn();
	}
	else
	{
		CArcitectBtn* pArcitectBtn = static_cast<CArcitectBtn*>(m_pParent);
		//static_cast<CButton*>(pArcitectBtn->m_pCommandBtn)->Set_Activate(false);

		m_bActivate = true;
		Create_SteelWallBtn();
	}
}

void CStructureBtn::Release()
{
	m_bActivate = false;
	Destroy_SteelWallBtn();
}

void CStructureBtn::Set_Activate(bool _bActivate)
{
	if (_bActivate == false)
	{
		m_bActivate = _bActivate;
		Destroy_SteelWallBtn();
	}
}
