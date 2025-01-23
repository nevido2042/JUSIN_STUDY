#include "pch.h"
#include "CMainGame.h"
#include "CDevice.h"

CMainGame::CMainGame()
{
	
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	if (FAILED(CDevice::Get_Instance()->Init_Device()))
	{
		//AfxMessageBox(L"Init_Device Create Failed");
		return;
	}
}

void CMainGame::Update()
{
}

void CMainGame::Late_Update()
{
}

void CMainGame::Render()
{
	CDevice::Get_Instance()->Render_Begin();
	CDevice::Get_Instance()->Render_End();
}

void CMainGame::Release()
{
}
