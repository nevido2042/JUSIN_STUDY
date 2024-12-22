#pragma once
#include "Define.h"

class CEffectMgr
{
public:
	CEffectMgr();
	~CEffectMgr();
public:
	void Create_Effect(POS _tPos, float _CX, float _CY, const TCHAR* _pImgKey, float _fTime);

public:
	static CEffectMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CEffectMgr;

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
	static CEffectMgr* m_pInstance;
};

