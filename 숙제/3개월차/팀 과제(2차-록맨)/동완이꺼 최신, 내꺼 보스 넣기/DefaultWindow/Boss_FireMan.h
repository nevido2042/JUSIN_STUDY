#pragma once

#include"CObj.h"


enum StateType // 
{
    IDLE,
    ATTACK,
    CHASE,
};

class CBoss_FireMan : public CObj
{
public:
    CBoss_FireMan();
	~CBoss_FireMan();

private:
// CObj��(��) ���� ��ӵ�
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

    void     Key_Input();


public:
    float Get_Hp() { return MAX_Hp - Hp_Count; }
    void Set_HP(float _type) { Hp_Count = _type; }
    void Set_Player(CObj* pPlayer) { m_Boss_pPlayer = pPlayer; } // �÷��̾� ��ü ����
    CObj* Create_Bullet(int _type);

private:
    void Set_Boss_Ground(bool _bisGround)
    {
        m_Boss_Ground = _bisGround;

    }
    bool Get_Boss_Ground() {    return m_Boss_Ground; }
    void Set_Boss_Jump(bool _isjump) { m_bJump = _isjump; }//����,�������¼���
    bool GetJump() { return m_bJump; }//����, �������¹�ȯ
    void Jumping();




private:
    list<CObj*>* m_p_Boss_Bullet;
    INFO HP_INFO;
 
    CObj* m_Boss_pPlayer; // �÷��̾� ��ü�� ������ ��� ����
private:
    float MAX_Hp; // �⺻ ü��
    float Hp_Count; // ������ �浹�Ҷ����� üũ

private: // �������ڵ�
    IMAGE    m_tIdle;
    IMAGE   m_tAttack1;
    IMAGE   m_tAttack2;
    IMAGE   m_tNowState;
    IMAGE   m_tChase;
    bool m_Boss_Ground;
private:














    unsigned long long m_ullLast_Fire;
    int Bullet_X;
 

    bool                m_bJump; // ����üũ
    float               m_fJumpPower;//�����ӵ�
    float               m_fTime; // �ð�

    ////////////////////////
       bool m_bMovingLeft;
       int m_fJumpCooldown;
  

};

