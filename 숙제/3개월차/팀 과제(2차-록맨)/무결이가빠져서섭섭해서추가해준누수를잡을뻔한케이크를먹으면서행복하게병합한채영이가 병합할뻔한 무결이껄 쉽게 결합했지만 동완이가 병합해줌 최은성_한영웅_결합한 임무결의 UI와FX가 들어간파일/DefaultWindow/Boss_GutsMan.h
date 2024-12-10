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
    float Hp_Count; // 보스에 충돌할때마다 체크

    bool                m_bJump; // 점프체크
    float               m_fJumpPower;//점프속도
    float               m_fTime; // 시간
    bool                m_bisGround; 

    enum GUTSTATE{ STATE_IDLE, STATE_MOVE, STATE_HOLD, STATE_ATTACK, STATE_JUMP, STATE_END};
    GUTSTATE    m_enState;
    UINT         m_iStateCooldown;
    void        Attack();
    void        Jump();
    DIRECTION   m_enMovingDir;


};

