#pragma once
#include "Obj.h"
#include "Shop.h"

class CMainGame
{
	CObj* m_pPlayer;
	CObj* m_pDungeon;
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

