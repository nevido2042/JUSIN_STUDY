#pragma once
#include "Obj.h"

class CColonyMgr
{
public:
	enum MODE { MODE_SELECT, MODE_DECONSTRUCT, MODE_END };
private:
	CColonyMgr();
	virtual ~CColonyMgr();
public:
	void	Change_Mode(MODE _eMode);
	void	Emplace_DeconstructSet(CObj* _pObj);
public:
	void	Set_Target(CObj* _pObj) { m_pTarget = _pObj; }
	CObj*	Get_Target() { return m_pTarget; }
	MODE	Get_Mode() { return m_eMode; }
private:
	void	Input_Key();

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
	CObj*	m_pTarget;
	MODE	m_eMode;

	//해체 리스트 //맵을 쓰면? 셋?
	set<CObj*> m_DeconstructSet;
};

