#include "pch.h"
#include "Button.h"

CButton::CButton()
	:m_iDrawID(0)
{
}

CButton::~CButton()
{
	Release();
}

void CButton::Initialize()
{
}

int CButton::Update()
{
	return OBJ_NOEVENT;
}

void CButton::Late_Update()
{

}

void CButton::Render(HDC hDC)
{
}

void CButton::Release()
{
}
