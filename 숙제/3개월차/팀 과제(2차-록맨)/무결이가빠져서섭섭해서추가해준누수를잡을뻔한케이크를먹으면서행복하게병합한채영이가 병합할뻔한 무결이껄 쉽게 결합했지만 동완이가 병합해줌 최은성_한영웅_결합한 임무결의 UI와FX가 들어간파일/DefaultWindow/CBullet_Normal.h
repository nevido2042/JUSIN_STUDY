#pragma once
#include "CBullet.h"

class CBullet_Normal : public CBullet
{
public:
	CBullet_Normal();
	~CBullet_Normal();


private:

	// CBullet��(��) ���� ��ӵ�
	void Initialize() override;

	int Update() override;

	void Late_Update() override;

	void Render(HDC hDC) override;

	void Release() override;

};

