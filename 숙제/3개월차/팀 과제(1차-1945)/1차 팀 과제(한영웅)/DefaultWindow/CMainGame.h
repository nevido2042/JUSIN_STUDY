#pragma once

#include "Define.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "Satellite.h"

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

	ULONGLONG		m_ullMonsterSpawnTime;
	

};

// ������ ���� : �ڵ��� �������� ������ ���·� ����� �����ϱ� ���ϰ� ���� ����
// https://refactoring.guru/ko/design-patterns

//1. �߻� ���丮 ���� : ���� ������ �� ������� �ν��Ͻ��� ����� ������ �߻�ȭ��Ų ����
//2. �ݺ��� ���� : ���� ǥ�� ����� �������� �ʰ� �������� ���� ����� �����ϴ� ����


// ���� 1. ������ �ٶ󺸴� �������� �Ѿ��� �߻��϶�.
// ���� 2. �÷��̾ �߽����� �����ϴ� ������ �����϶�.