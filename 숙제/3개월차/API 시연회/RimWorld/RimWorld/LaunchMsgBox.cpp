#include "pch.h"
#include "LaunchMsgBox.h"
#include "ObjMgr.h"
#include "Pawn.h"

CLaunchMsgBox::CLaunchMsgBox()
	:m_iRimCount(0)
{
}

CLaunchMsgBox::~CLaunchMsgBox()
{
	Release();
}

void CLaunchMsgBox::Initialize()
{
	CMsgBox::Initialize();

	list<CObj*> RimList = CObjMgr::Get_Instance()->Get_List()[OBJ_RIM];

	for (CObj* pObj : RimList)
	{
		if (CPawn* pPawn = dynamic_cast<CPawn*>(pObj))
		{
			if (pPawn->Get_State() == CPawn::BOARDING)
			{
				++m_iRimCount;
			}
		}
	}
}

void CLaunchMsgBox::Render(HDC hDC)
{
	CMsgBox::Render(hDC);

	wchar_t buffer[50];
	wsprintf(buffer, L"탈출 한 림: %d", m_iRimCount);
	// 문자열 출력 (유니코드)
	TextOutW(hDC, int(m_tRect.left + 10), int(m_tRect.top + 10), buffer, lstrlenW(buffer));
}
