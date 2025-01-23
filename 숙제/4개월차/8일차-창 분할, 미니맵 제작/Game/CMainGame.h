#pragma once
#include "CPlayer.h"
#include "CTextureMgr.h"

class CMainGame
{
public:
	CMainGame();
	~CMainGame();
public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render();
	void Release();
private:
	CObj*	m_pPlayer;
};

