#pragma once

#include"CObj.h"

class CBoss : public CObj
{
public:
	CBoss();
	~CBoss();

private:
// CObj을(를) 통해 상속됨
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

    void     Key_Input();

    float Get_Hp() { return MAX_Hp - Hp_Count; }
    void Set_HP(float _type) { Hp_Count = _type; }
    void Set_Player(CObj* pPlayer) { m_Boss_pPlayer = pPlayer; } // 플레이어 객체 설정
    CObj* Create_Bullet(int _type);
private:



    list<CObj*>* m_p_Boss_Bullet;
    unsigned long long m_ullLast_Fire;
    INFO HP_INFO;
    CObj* m_Boss_pPlayer; // 플레이어 객체를 저장할 멤버 변수

private:
    int Bullet_X;
    float m_fTurretAngle; // 포신의 현재 회전각도
    float m_fTurretSpeed; // 포신의 회전 속도
    POINT turretEnd;
    float MAX_Hp; // 기본 체력
    float Hp_Count; // 보스에 충돌할때마다 체크

};

