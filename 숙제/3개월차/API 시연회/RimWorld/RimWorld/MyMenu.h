#pragma once
#include "Scene.h"
#include "MyButton.h"


class CMyMenu :   public CScene
{
public:
	CMyMenu();
	virtual ~CMyMenu();

public:
	void Initialize() override;
	int Update() override;
	int Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
};

