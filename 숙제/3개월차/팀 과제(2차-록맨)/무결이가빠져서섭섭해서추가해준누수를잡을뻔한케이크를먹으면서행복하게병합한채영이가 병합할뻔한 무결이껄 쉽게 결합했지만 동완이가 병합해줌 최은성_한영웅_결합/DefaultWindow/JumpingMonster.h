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
	//낙하 속도
	float m_fGravity;

	//점프 속도
	//float m_fJumpSpeed;

	//점프 중인가?
	//bool  m_bJump;

	//점프 포물선 계산 용도 타임
	//float  m_fTime;

	//마지막 점프 시간
	ULONGLONG  m_ullJumpTime;

	//이전 프레임의 Y좌표
	float m_fPrevY;

	//플레이어
	CObj* m_pPlayer;
};

