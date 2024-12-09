#pragma once
#include "CAnimation.h"

class CAnimMgr
{
private:
	CAnimMgr();
	~CAnimMgr() { Release(); }

public:
	void	Update();
	void	Late_Update();
	void	Render(HDC _hDc, const TCHAR* _pImgKey);
	void	Release();
	void	Insert_Animation(const TCHAR* pImgKey, CAnimation* _pAnim);

public:
	static CAnimMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CAnimMgr;

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
	map<const TCHAR*, CAnimation*> m_mapAnim;
	static	CAnimMgr* m_pInstance;

};

