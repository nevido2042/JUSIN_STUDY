#pragma once

#include "CObj.h"

class CMonster : public CObj
{
public:
	CMonster();
	virtual ~CMonster();

public:
	void	Set_Player(CObj* pPlayer) { m_pPlayer = pPlayer; }

public:
	void Initialize() override;
	void Update() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	CObj*			m_pPlayer;
	float           m_fAngle;
	float			m_fOrbitAngle;
	D3DXVECTOR3     m_vPoint[3];
	D3DXVECTOR3     m_vOriginPoint[3];
};

