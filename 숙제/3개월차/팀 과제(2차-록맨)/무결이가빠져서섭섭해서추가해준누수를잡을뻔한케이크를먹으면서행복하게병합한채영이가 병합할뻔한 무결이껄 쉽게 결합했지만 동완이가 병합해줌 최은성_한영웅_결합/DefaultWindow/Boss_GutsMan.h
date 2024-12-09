#pragma once
#include "CObj.h"
class CBoss_GutsMan : public CObj
{
public:
	CBoss_GutsMan();
	~CBoss_GutsMan() { Release(); }

public:
	void Initialize() override;
    void Initialize_Animation();
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

    CObj* Create_Bullet();


private:
    INFO HP_INFO;
    int Bullet_X;
    float MAX_Hp; // �⺻ ü��
    float Hp_Count; // ������ �浹�Ҷ����� üũ

    bool                m_bJump; // ����üũ
    float               m_fJumpPower;//�����ӵ�
    float               m_fTime; // �ð�
    bool                m_bisGround; 

    enum GUTSTATE{ STATE_IDLE, STATE_MOVE, STATE_HOLD, STATE_ATTACK, STATE_JUMP, STATE_END};
    GUTSTATE    m_enState;
    UINT         m_iStateCooldown;
    void        Attack();
    void        Jump();
    DIRECTION   m_enMovingDir;


};

