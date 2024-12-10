#include "pch.h"
#include "CLineMgr.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"

CLineMgr* CLineMgr::m_pInstance = nullptr;

CLineMgr::CLineMgr()
{
	ZeroMemory(m_tLinePoint, sizeof(m_tLinePoint));
}

CLineMgr::~CLineMgr()
{
	Release();
}

void CLineMgr::Add_Line(const RECT& _rc)
{
	//LINEPOINT LinePoint1 = { _rc.left,_rc.top };
	//LINEPOINT LinePoint2 = { _rc.right,_rc.top };
	//m_Linelist_Floor.push_back(new CLine(LinePoint1, LinePoint2)); // 拉何盒 扼牢积己

	//LinePoint1 = { _rc.left,_rc.bottom };
	//LinePoint2 = { _rc.left,_rc.top };
}

void CLineMgr::Release()
{
	for_each(m_Linelist_Floor.begin(), m_Linelist_Floor.end(), Safe_Delete<CLine*>);
	for_each(m_Linelist_Ceiling.begin(), m_Linelist_Ceiling.end(), Safe_Delete<CLine*>);

	m_Linelist_Floor.clear();
	m_Linelist_Ceiling.clear();
}