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

	void		Set_Size(float _fCX, float _fCY)
	{
		m_tInfo.fCX = _fCX;
		m_tInfo.fCY = _fCY;
	}

	void		Set_PosX(float _fX) { m_tInfo.fX += _fX; }
	void		Set_PosY(float _fY) { m_tInfo.fY += _fY; }

	void		Set_Direction(DIRECTION eDir) { m_eDir = eDir; }
	void		Set_Dead() { m_bDead = true; }
	void		Set_Angle(float _fAngle) { m_fAngle = _fAngle; }

	void		Set_Target(CObj* pTarget) { m_pTarget = pTarget; }
	bool		Get_Dead() { return m_bDead; }

	bool		Get_Ground() { return m_bGround; }
	void		Set_Ground(bool _b) { m_bGround = _b; }

public:
	virtual void		Initialize()PURE;
	virtual int			Update()PURE;
	virtual void		Late_Update()PURE;
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

	bool		m_bGround;
};

