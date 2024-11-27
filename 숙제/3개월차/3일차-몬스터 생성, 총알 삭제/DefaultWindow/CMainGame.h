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

// ������ ���� : �ڵ��� �������� ������ ���·� ����� �����ϱ� ���ϰ� ���� ����
// https://refactoring.guru/ko/design-patterns

//1. �߻� ���丮 ���� : ���� ������ �� ������� �ν��Ͻ��� ����� ������ �߻�ȭ��Ų ����
//2. �ݺ��� ���� : ���� ǥ�� ����� �������� �ʰ� �������� ���� ����� �����ϴ� ����


// ���� 1. ��Ÿ����� ������ �о��.(C^2 = A^2 + B^2)
// ���� 2. ��Ÿ����� ������ �̿��� �� �浹�� ���Ϳ� �Ѿ��� �浹ó���� �����ض�.