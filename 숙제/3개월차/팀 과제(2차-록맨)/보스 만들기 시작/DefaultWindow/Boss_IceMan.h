#pragma once

#include"CObj.h"

class CBoss_IceMan : public CObj
{
public:
	CBoss_IceMan();
	virtual ~CBoss_IceMan();
public:
	// CObj��(��) ���� ��ӵ�
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
private:
	//�߷�
	void Fall();

private:
	//���� ����
	void Jump();
	void Fire();

private:
	float m_fAccel;
	float m_fGravity;
};

