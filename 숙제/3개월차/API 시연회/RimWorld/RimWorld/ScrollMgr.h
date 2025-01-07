#pragma once

#include "Define.h"

class CScrollMgr
{
private:
	CScrollMgr();
	~CScrollMgr();

public:
	void Scroll_Lock();

public:
	float			Get_ScrollX() { return /*m_fScrollX*/0; }
	float			Get_ScrollY() { return /*m_fScrollY*/0; }

	void			Move_ScrollX(float _fX) { m_bMoveLerp = false; m_fScrollX += _fX; }
	void			Move_ScrollY(float _fY) { m_bMoveLerp = false; m_fScrollY += _fY; }

	void			Set_ScrollX(float _fX) { m_fScrollX = _fX; }
	void			Set_ScrollY(float _fY) { m_fScrollY = _fY; }

	void			Late_Update();
	void			Move_To_Lerp(float _fX, float _fY);

public:
	static CScrollMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CScrollMgr;

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
	static CScrollMgr* m_pInstance;

	float			    m_fScrollX;
	float			    m_fScrollY;

	bool				m_bMoveLerp;
	float				m_fMoveX;
	float				m_fMoveY;
};

