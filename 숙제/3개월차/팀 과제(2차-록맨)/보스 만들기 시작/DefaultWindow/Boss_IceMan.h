#pragma once

#include"CObj.h"

class CBoss_IceMan : public CObj
{
public:
	CBoss_IceMan();
	virtual ~CBoss_IceMan();
public:
	// CObj을(를) 통해 상속됨
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
private:
	//중력
	void Fall();

private:
	//보스 패턴
	void Jumping();
	void Fire();

private:
	//낙하 속도
	float m_fGravity;

	//점프 속도
	float m_fJumpSpeed;

	//점프 중인가?
	bool  m_bJump;

	//점프 포물선 계산 용도 타임
	float  m_fTime;

	//마지막 점프 시간
	ULONGLONG  m_ullJumpTime;

	//마지막 발사 시간
	ULONGLONG  m_ullFireTime;
	
	//이전 프레임의 Y좌표
	float m_fPrevY;

	//플레이어
	CObj* m_pPlayer;
};

