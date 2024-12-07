#pragma once

#include"CObj.h"

class CBoss_IceMan : public CObj
{
public:
	CBoss_IceMan();
	virtual ~CBoss_IceMan();
public:
	// CObj을(를) 통해 상속됨
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
private:
	//중력
	void Fall();

private:
	//보스 패턴
	void Jump();
	void Fire();

private:
	float m_fAccel;
	float m_fGravity;
};

