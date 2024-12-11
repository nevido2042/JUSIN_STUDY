#include "pch.h"
#include "SceneMgr.h"

CSceneMgr* CSceneMgr::m_pInstance = nullptr;

CSceneMgr::CSceneMgr() : m_pScene(nullptr), 
m_eCurScene(SC_LOGO), m_ePreScene(SC_END)
{
}

CSceneMgr::~CSceneMgr()
{
	Release();
}

//Set Scene�� ����Ʈ������Ʈ���� �ϴϱ�
//���� �ٲ�鼭 ������Ʈ����Ʈ�� ������ٰ�
//������Ʈ�� �߰� �Ǹ鼭 ���ͷ����Ͱ� ������.
//Initialize���� Start��� �ָ� ���� �־...
//Start���� ������Ʈ�� �����ϰ� �ұ�?
void CSceneMgr::Set_Scene(SCENEID eID)
{
	m_eCurScene = eID;

	if (m_ePreScene != m_eCurScene)
	{
		Safe_Delete(m_pScene);

		switch (m_eCurScene)
		{
		case SC_LOGO:
			m_pScene = new CLogo;
			break;

		case SC_MENU:
			m_pScene = new CMyMenu;
			break;

		case SC_CUSTOMIZE:
			m_pScene = new CCustomize;
			break;

		case SC_COLONY:
			m_pScene = new CColony;
			break;
		
		case SC_EDIT:
			//m_pScene = new CLogo;
			break;
		}

		m_pScene->Initialize();

		m_ePreScene = m_eCurScene;
	}
}

int CSceneMgr::Update()
{
	m_pScene->Update();

	return OBJ_NOEVENT;
}

void CSceneMgr::Late_Update()
{
	m_pScene->Late_Update();
}

void CSceneMgr::Render(HDC hDC)
{
	m_pScene->Render(hDC);
}

void CSceneMgr::Release()
{
	Safe_Delete(m_pScene);
}
