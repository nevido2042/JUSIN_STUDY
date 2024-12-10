#pragma once

#include "Define.h"

class CScrollMgr
{
private:
	CScrollMgr();
	~CScrollMgr();


public:
	float			Get_ScrollX() { return m_fScrollX; }
	float			Get_ScrollY() { return m_fScrollY; }

	void			Set_ScrollX(float _fX) { m_fScrollX += _fX; }
	void			Set_ScrollY(float _fY) { m_fScrollY += _fY; }
	void			SET_DIR(const DIRECTION& _Dir);//방향걸정
	// 플레이어가 맵 밖에 나가면 플레이어 위치 부르는기능
	const DIRECTION& Get_Dir() { return m_eDir; } // 스크롤방향

	bool			Update();

public:
	static CScrollMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CScrollMgr;

		return m_pInstance;
	}

	static void	Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CScrollMgr* m_pInstance;

	float			    m_fScrollX;
	float			    m_fScrollY;
	////////////////////////////////은성이형 코드
	DIRECTION			m_eDir; // 스크롤방향
	float				m_fCheck; // 스크롤이 움직일때
	// 스크롤이 다 움직였는지 체크하는 변수
	// 스크롤이 움직이는동안 모든 오브젝트는 정지
	float				m_fScrollSpeed; // 스크롤 속도
};


