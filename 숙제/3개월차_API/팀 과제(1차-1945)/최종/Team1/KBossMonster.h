#pragma once
#include "MonsterShoot.h"

class KBossMonster : public MonsterShoot
{
public:
	KBossMonster();
	virtual ~KBossMonster() { Release(); }

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Release() override;
	virtual void Render(HDC _hdc) override;


private:
	int m_iPattern; // �����ϰ� ���� ������
	ULONGLONG m_ulPatternTime; // ���� ���� �ð�
	float m_fDistance; // ������ �Ÿ� ����

public:
	
	void		Shoot_Sun(); // ������
	void		Shoot_Accel(); // �÷��̾� �ִ� �������� ����� + ����
	void		Shoot_Sprial(); // ����ó��
	void		Shoot_Rain(); // ������ �Ʒ��� 
	void		Shoot_Sun_Screw();
	void		Calc_Angle(float fX, float fY); // ����ź ������ �� ��� return�ϰ� �ؼ� setAngle�� �־�����°� ������?
};

