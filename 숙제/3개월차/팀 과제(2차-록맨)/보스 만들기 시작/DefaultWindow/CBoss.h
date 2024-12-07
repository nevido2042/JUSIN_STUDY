#pragma once

#include"CObj.h"

class CBoss : public CObj
{
public:
	CBoss();
	~CBoss();

private:
// CObj��(��) ���� ��ӵ�
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

    void     Key_Input();

    float Get_Hp() { return MAX_Hp - Hp_Count; }
    void Set_HP(float _type) { Hp_Count = _type; }
    void Set_Player(CObj* pPlayer) { m_Boss_pPlayer = pPlayer; } // �÷��̾� ��ü ����
    CObj* Create_Bullet(int _type);
private:



    list<CObj*>* m_p_Boss_Bullet;
    unsigned long long m_ullLast_Fire;
    INFO HP_INFO;
    CObj* m_Boss_pPlayer; // �÷��̾� ��ü�� ������ ��� ����

private:
    int Bullet_X;
    float m_fTurretAngle; // ������ ���� ȸ������
    float m_fTurretSpeed; // ������ ȸ�� �ӵ�
    POINT turretEnd;
    float MAX_Hp; // �⺻ ü��
    float Hp_Count; // ������ �浹�Ҷ����� üũ

};

