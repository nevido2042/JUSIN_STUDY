#pragma once
#include "Obj.h"
#include "Player.h"
#include "Dungeon.h"
#include "Shop.h"

class CMainGame
{
	CPlayer* m_pPlayer;
	CDungeon* m_pDungeon;
	CShop* m_pShop;
public:
	CMainGame();
	~CMainGame();
public:
	void Initialize();
	void Release();
private:
	void Select_Class();
	void Select_Menu();
	void Select_Dungeon();


};

