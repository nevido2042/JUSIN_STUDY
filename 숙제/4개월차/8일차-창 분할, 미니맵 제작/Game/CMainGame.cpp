#include "pch.h"
#include "CMainGame.h"
#include "CDevice.h"
#include "AbstractFactory.h"

CMainGame::CMainGame()
	:m_pPlayer(nullptr)
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

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(
		L"../Texture/Stage/Player/Stand/AKIHA_AKI00_000.png",
		TEX_SINGLE, L"Player")))
	{
		//AfxMessageBox(L"Terrain Texture Insert Failed");
		return;
	}


	m_pPlayer = CAbstractFactory<CPlayer>::Create();
	m_pPlayer->Initialize();
}

void CMainGame::Update()
{
	m_pPlayer->Update();
}

void CMainGame::Late_Update()
{
}

void CMainGame::Render()
{
	CDevice::Get_Instance()->Render_Begin();

	m_pPlayer->Render();

	CDevice::Get_Instance()->Render_End();
}

void CMainGame::Release()
{
}
