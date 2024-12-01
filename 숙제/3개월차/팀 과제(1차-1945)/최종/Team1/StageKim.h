#pragma once

#include "StageScene.h"

class StageKim : public StageScene
{
public:
	StageKim();
	virtual ~StageKim();

private:
	virtual void SpawnMonster();
	virtual int	 Update();
	virtual void Late_Update();
	virtual void Render(HDC _hDC);
};

