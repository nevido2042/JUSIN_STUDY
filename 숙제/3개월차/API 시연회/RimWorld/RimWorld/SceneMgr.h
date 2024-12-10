#pragma once

#include "Scene.h"

#include "Logo.h"
#include "MyMenu.h"
#include "Stage.h"

class CSceneMgr
{
private:
	CSceneMgr();
	~CSceneMgr();

public:
	void		Set_Scene(SCENEID eID);
	int			Update();
	void		Late_Update();
	void		Render(HDC hDC);
	void		Release();

public:
	static CSceneMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSceneMgr;

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
	static CSceneMgr*	m_pInstance;
	CScene*				m_pScene;

	SCENEID				m_eCurScene;
	SCENEID				m_ePreScene;

};

// ���� ����(state) : FSM�� ������� �ϴ� ��ü�� ���¸� ǥ���ϴ� ����, �ڽ��� ���� �� �ִ� ������ ������ ���¸� �����ٴ� ���信 �ٰ��Ͽ�
// ���� ���� ���� �� �ݵ�� �� �ϳ��� ���¸��� ���ϵ��� ����� ����