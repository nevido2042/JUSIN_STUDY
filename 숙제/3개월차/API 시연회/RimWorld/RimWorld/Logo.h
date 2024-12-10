#pragma once
#include "Scene.h"
class CLogo :  public CScene
{
public:
	CLogo();
	virtual ~CLogo();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
};

