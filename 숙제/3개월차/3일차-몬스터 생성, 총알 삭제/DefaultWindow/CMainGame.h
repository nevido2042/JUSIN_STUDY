#pragma once

#include "Define.h"
#include "CPlayer.h"
#include "CMonster.h"

class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	void		Initialize();
	void		Update();
	void		Late_Update();
	void		Render();
	void		Release();

private:
	HDC			m_hDC;

	list<CObj*>		m_ObjList[OBJ_END];

	ULONGLONG		m_ullTime;
	int			m_iFPS;
	TCHAR		m_szFPS[16];

};

// 디자인 패턴 : 코드의 구조들을 일정한 형태로 만들어 재사용하기 편리하게 만든 패턴
// https://refactoring.guru/ko/design-patterns

//1. 추상 팩토리 패턴 : 생성 패턴의 한 방법으로 인스턴스를 만드는 절차를 추상화시킨 패턴
//2. 반복자 패턴 : 내부 표현 방식은 공개하지 않고 순차적인 접근 방법을 제공하는 패턴


// 숙제 1. 피타고라스의 정리를 읽어라.(C^2 = A^2 + B^2)
// 숙제 2. 피타고라스의 정리를 이용한 원 충돌로 몬스터와 총알을 충돌처리를 구현해라.