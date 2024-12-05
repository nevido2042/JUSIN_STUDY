#pragma once

#include "Define.h"

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

	DWORD		m_dwTime;
	int			m_iFPS;
	TCHAR		m_szFPS[16];

};

// 디자인 패턴 : 코드의 구조들을 일정한 형태로 만들어 재사용하기 편리하게 만든 패턴
// https://refactoring.guru/ko/design-patterns

//1. 추상 팩토리 패턴 : 생성 패턴의 한 방법으로 인스턴스를 만드는 절차를 추상화시킨 패턴
//2. 반복자 패턴 : 내부 표현 방식은 공개하지 않고 순차적인 접근 방법을 제공하는 패턴
//3. 중재자(메디에이터) 패턴 : 상호작용하는 객체들이 서로 복잡한 관계를 맺고 있을 경우 별도의 형식으로 정의하여 중재하는 객체를 두는 패턴
//4. 싱글톤 패턴 : 단, 하나의 인스턴스(객체)를 생성하여 사용하는 패턴, 인스턴스가 필요할 때 똑같은 형식의 인스턴스를 만드는 것이 아니라 기존의 인스턴스를 불러와서 사용하는 패턴