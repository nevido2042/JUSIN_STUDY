#pragma once
#include "CMonster.h"

class CRepeat_Monster : public CMonster
{
public:
	CRepeat_Monster();
	CRepeat_Monster(float _x, float _y, float MinX, float MaxX);
	~CRepeat_Monster();

	// CMonster을(를) 통해 상속됨
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	void SetPartrolRange(float _minX, float _maxX)
	{
		m_fMinX = _minX;
		m_fMaxX = _maxX;
	}

	void Patrol();

private:
	BEHAVOR_STATE m_eState;
	IMAGE m_tPartol;
	IMAGE m_tAttack;
	IMAGE m_tNowState;

	float m_fDinstance;
	float m_iDirection;

private:
	float m_fMinX;
	float m_fMaxX;
};

