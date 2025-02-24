#pragma once
#include "CScene.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CMouse.h"
#include "CNetwork.h"


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

