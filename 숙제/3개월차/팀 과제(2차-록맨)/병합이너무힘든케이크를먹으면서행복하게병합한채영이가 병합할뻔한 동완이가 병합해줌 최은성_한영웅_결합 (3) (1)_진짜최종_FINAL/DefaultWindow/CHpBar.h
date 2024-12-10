#pragma once
#include "CObj.h"
#include "CUI.h"
#include "CPlayer.h"
class CHpBar : public CUI
{
public:
	CHpBar();
	~CHpBar();
public:
	void Initialize();
	int Update();
	void Late_Update();
	void Render(HDC hdc);
	void Release();

private:
	CObj* m_pPlayer;
	IMAGE m_tNowState;

	IMAGE m_tHp_100;
	IMAGE m_tHp_80;
	IMAGE m_tHp_60;
	IMAGE m_tHp_50;
	IMAGE m_tHp_40;
	IMAGE m_tHp_20;
	IMAGE m_tHp_10;
	IMAGE m_tHp_00;
	
};

