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

//Set Scene을 레이트업데이트에서 하니까
//맵이 바뀌면서 오브젝트리스트가 사라졌다가
//오브젝트가 추가 되면서 이터레이터가 깨진다.
//Initialize말고 Start라는 애를 새로 넣어서...
//Start에서 오브젝트를 생성하게 할까?
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
