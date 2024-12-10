#pragma once
#include "Scene.h"
#include "Player.h"
#include "Monster.h"
#include "Mouse.h"

class CStage : public CScene
{
public:
	CStage();
	virtual ~CStage();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
};

