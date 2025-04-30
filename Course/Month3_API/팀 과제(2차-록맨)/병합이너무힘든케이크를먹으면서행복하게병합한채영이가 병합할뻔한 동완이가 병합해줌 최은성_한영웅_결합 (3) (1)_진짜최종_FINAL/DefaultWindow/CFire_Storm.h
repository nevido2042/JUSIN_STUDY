#pragma once
#include "CObj.h"
#include "CBullet.h"

class CFire_Storm : public CBullet
{
public:
	CFire_Storm(){}
	~CFire_Storm(){}

private:


	// CBullet을(를) 통해 상속됨
	void Initialize() override;

	int Update() override;

	void Late_Update() override;

	void Render(HDC hDC) override;

	void Release() override;

	
	CObj* m_pPlayer;
	CObj* m_pBoss;

};

