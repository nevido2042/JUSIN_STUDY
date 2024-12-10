#pragma once
#include "CUI.h"
#include "CObj.h"
#include "CMonster.h"

class CBossHpBar : public CUI
{
public:
	CBossHpBar();
	CBossHpBar(CObj* _Boss);
	~CBossHpBar();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hdc) override;
	void Release() override;
public:
	void SetBossMonster(CObj* Boss)
	{
		m_pBossMonster = Boss;

	}

private:
	IMAGE m_tHp_100;
	IMAGE m_tHp_80;
	IMAGE m_tHp_60;
	IMAGE m_tHp_50;
	IMAGE m_tHp_40;
	IMAGE m_tHp_20;
	IMAGE m_tHp_10;
	IMAGE m_tHp_00;
	IMAGE m_tCurHp;

	const WCHAR* m_keyName;
	CObj* m_pBossMonster;
};

