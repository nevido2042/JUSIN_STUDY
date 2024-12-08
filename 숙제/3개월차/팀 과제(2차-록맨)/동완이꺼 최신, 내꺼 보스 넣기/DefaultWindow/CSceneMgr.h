#pragma once
#include "Define.h"
#include "CScene.h"

class CSceneMgr
{
private:
	CSceneMgr();
	~CSceneMgr();
public:

	static CSceneMgr* Get_Instance() // �̱��� Ư���� ���� �ѹ��� �����ϴ� ���
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSceneMgr;

		return m_pInstance;
	}
	static void	Destroy_Instance() // ��ü ����
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
	void Set_Scene(SCENE_TYPE eType);  // �� ��ȯ
	void Initialize(); // �ʱ�ȭ
	void Update(); // ������Ʈ
	void Render(HDC hDC); // ����

private:
	static CSceneMgr* m_pInstance;  // ��ü ���� �ּ�
	CScene* m_arrScene[(UINT)SCENE_TYPE::SCENE_END]; // ����
	CScene* m_pCurScene; // �����




};

