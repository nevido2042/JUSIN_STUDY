#pragma once
#include "Define.h"
#include "CScene.h"

class CScene;


class CSceneMgr
{
private:
	CSceneMgr();
	~CSceneMgr();
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
	void Set_Scene(SCENE_TYPE eType); 
	void Initialize();
	void Update();
	void Render(HDC hDC);

private:
	static CSceneMgr* m_pInstance;
	CScene* m_arrScene[(UINT)SCENE_TYPE::SCENE_END]; // ¸ðµç¾À
	CScene* m_pCurScene; // ÇöÀç¾À




};

