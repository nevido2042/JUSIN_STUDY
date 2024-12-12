#pragma once
#include "Obj.h"

class CColonyMgr
{
public:
	CColonyMgr();
	virtual ~CColonyMgr();
	
public:
	void Set_Target(CObj* _pObj) { m_pTarget = _pObj; }

	void Input_Key();

public:
	void	Initialize();
	int		Update();
	void	Late_Update();
	void	Render(HDC hDC);
	void	Release();


public:
	static CColonyMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CColonyMgr;

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
	static CColonyMgr* m_pInstance;

private:
	CObj* m_pTarget;
};

