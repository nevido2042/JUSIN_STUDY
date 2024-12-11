#pragma once

#include "Define.h"

class CObj
{
public:
	CObj();
	virtual ~CObj();

public:
	INFO			Get_Info() { return m_tInfo; }
	const RECT*		Get_Rect() { return &m_tRect; }

	void		Set_Pos(float _fX, float _fY)
	{
		m_tInfo.fX = _fX;
		m_tInfo.fY = _fY;
	}

	void		Move_PosX(float _fX) { m_tInfo.fX += _fX; }
	void		Move_PosY(float _fY) { m_tInfo.fY += _fY; }

	void		Set_Direction(DIRECTION eDir) { m_eDir = eDir; }
	void		Set_Dead() { m_bDead = true; }
	void		Set_Angle(float _fAngle) { m_fAngle = _fAngle; }

	void		Set_Target(CObj* pTarget) { m_pTarget = pTarget; }
	bool		Get_Dead() { return m_bDead; }

	void		Set_ImgKey(const TCHAR* pImgKey) { m_pImgKey = pImgKey; }

public:
	virtual void		Initialize()PURE;
	virtual int			Update()PURE;
	virtual int		Late_Update()PURE;
	virtual void		Render(HDC hDC)PURE;
	virtual void		Release()PURE;

public:
	void		Update_Rect();

protected:
	INFO		m_tInfo;
	RECT		m_tRect;

	float		m_fSpeed;
	float		m_fAngle;
	float		m_fDistance;

	bool		m_bDead;
	DIRECTION	m_eDir;

	CObj*		m_pTarget;

	const TCHAR*		m_pImgKey;
};

