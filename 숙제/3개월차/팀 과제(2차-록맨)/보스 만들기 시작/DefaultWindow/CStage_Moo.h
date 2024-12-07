#pragma once

#include "CScene.h"

class CStage_Moo : public CScene
{
public:
	CStage_Moo();
	~CStage_Moo();

	virtual void Initialize()override;  // 동적할당및초기화

	virtual void Update()override; // 실시간 정보

	virtual void LateUpdate()override;

	virtual void Release()override; // 동적할당해제

	virtual void Render(HDC _hDC)override;

	virtual void Exit()override;// 해당 씬에서 나갈때 호출

private:


};

