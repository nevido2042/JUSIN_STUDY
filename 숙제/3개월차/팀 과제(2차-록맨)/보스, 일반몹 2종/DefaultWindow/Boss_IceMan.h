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
	void OnCollision(CObj* _pOther, OBJID _eOtherID) override;
private:
	//�߷�
	void Fall();

private:
	//���� ����
	void Jumping();
	void Fire();

private:
	//���� �ӵ�
	float m_fGravity;

	//���� �ӵ�
	float m_fJumpSpeed;

	//���� ���ΰ�?
	bool  m_bJump;

	//���� ������ ��� �뵵 Ÿ��
	float  m_fTime;

	//������ ���� �ð�
	ULONGLONG  m_ullJumpTime;

	//������ �߻� �ð�
	ULONGLONG  m_ullFireTime;
	
	//���� �������� Y��ǥ
	float m_fPrevY;

	//�÷��̾�
	CObj* m_pPlayer;
};

