#pragma once

#include "Define.h"
#include "CPlayer.h"

class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	void		Initialize();
	void		Update();
	void		Render();
	void		Release();

private:
	HDC			m_hDC;

	CObj*		m_pPlayer;
	list<CObj*>	m_BulletList;
	list<CObj*>	m_MonsterList;

	DWORD		m_dwTime;
	int			m_iFPS;
	TCHAR		m_szFPS[16];

};

// 1. W, A, S, D 키를 누를 경우 각 방향을 향하는 미사일 발사하기
// 2. 전체 화면보다 가로 100, 세로 100 작은 사각형을 그리고 그 범위를 벗어난 미사일 소멸시키기
// 3. 작은 사각형 안에서 좌로 또는 우로 향하는 몬스터를 구현하기
// 4. 발사한 총알과 몬스터가 충돌할 경우 삭제해라

// BOOL IntersectRect(충돌한 렉트에 의해 발생한 렉트의 정보를 저장할 주소, 렉트 주소1, 렉트 주소2)