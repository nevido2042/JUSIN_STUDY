#pragma once
#include "CMonster.h"
class CJumpingMonster :
    public CMonster
{
public:
	CJumpingMonster();
	virtual ~CJumpingMonster();
public:
    virtual void Initialize() override;
    virtual void Render(HDC hDC) override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Release() override;
	void OnCollision(CObj* _pOther, OBJID _eOtherID) override;

private:
	void Fall();
	void Jumping();
	void Jump_Pattern();
private:
	//���� �ӵ�
	float m_fGravity;

	//���� �ӵ�
	//float m_fJumpSpeed;

	//���� ���ΰ�?
	//bool  m_bJump;

	//���� ������ ��� �뵵 Ÿ��
	//float  m_fTime;

	//������ ���� �ð�
	ULONGLONG  m_ullJumpTime;

	//���� �������� Y��ǥ
	float m_fPrevY;

	//�÷��̾�
	CObj* m_pPlayer;
};

