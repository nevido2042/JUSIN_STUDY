#pragma once
#include "Define.h"
#include "CUI.h"

class CUIMgr
{
private:
	CUIMgr();
	~CUIMgr();

public:
	void Add_UI(UI_TYPE etype, CUI* pUI);
	int Update();
	void Late_Update();
	void Render(HDC hDC);
	void Release();

public:
	static CUIMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CUIMgr;

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
	
	static CUIMgr* m_pInstance;
	list<CUI*> m_UIList[UI_END];
};