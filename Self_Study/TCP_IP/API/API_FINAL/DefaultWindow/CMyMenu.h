#pragma once
#include "CScene.h"
#include "CMyButton.h"


class CMyMenu :   public CScene
{
public:
	CMyMenu();
	virtual ~CMyMenu();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
};

