#pragma once
#include "CObj.h"
#include "CBullet.h"

class CFire_Player_Bullet : public CBullet
{
public:
	CFire_Player_Bullet();
	~CFire_Player_Bullet();

private:


	// CBullet��(��) ���� ��ӵ�
	void Initialize() override;

	int Update() override;

	void Late_Update() override;

	void Render(HDC hDC) override;

	void Release() override;

private:

	CObj* m_pPlayer;
	
	HDC  hMemDC;
};

