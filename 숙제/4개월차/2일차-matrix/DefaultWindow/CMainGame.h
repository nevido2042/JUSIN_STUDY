#pragma once

#include "Define.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "Tank.h"
#include "Square.h"

class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	void		Initialize();
	void		Update();
	void		Render();
	void		Release();

private:
	HDC			m_hDC;

	CObj*		m_pPlayer;
	CObj*		m_pMonster;

	DWORD		m_dwTime;
	int			m_iFPS;
	TCHAR		m_szFPS[16];

};

