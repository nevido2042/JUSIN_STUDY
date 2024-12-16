#pragma once

#include "Define.h"

class CObj
{
public:
	CObj();
	virtual ~CObj();
public:
	static float	Calculate_Dist(CObj* _pDst, CObj* _pSrc);
public:
	INFO			Get_Info() { return m_tInfo; }
	const RECT*		Get_Rect() { return &m_tRect; }
	float			Get_Angle() { return m_fAngle; }
	CObj*		Get_Target() { return m_pTarget; }

	void		Set_Pos(float _fX, float _fY)
	{
		m_tInfo.fX = _fX;
		m_tInfo.fY = _fY;
	}

	const RENDERID Get_GroupID() const { return m_eRenderID; }

	void		Move_PosX(float _fX) { m_tInfo.fX += _fX; }
	void		Move_PosY(float _fY) { m_tInfo.fY += _fY; }

	void		Set_Direction(DIRECTION eDir) { m_eDir = eDir; }
	void		Set_Destroyed() { m_bDestroyed = true; }
	void		Set_Angle(float _fAngle) { m_fAngle = _fAngle; }

	void		Set_Target(CObj* pTarget) { m_pTarget = pTarget; }
	bool		Get_Destroyed() { return m_bDestroyed; }

	void		Set_ImgKey(const TCHAR* pImgKey) { m_pImgKey = pImgKey; }

public:
	virtual void		Initialize()PURE;
	virtual int			Update()PURE;
	virtual void		Late_Update()PURE;
	virtual void		Render(HDC hDC)PURE;
	virtual void		Release()PURE;

public:
	//충돌 했을 때, 충돌한 오브젝트의, OBJID와 충돌한 객체를 가져온다
	virtual	void		OnCollision(OBJID _eID, CObj* _pOther);

public:
	void		Update_Rect();
	void		Move_Frame();

protected:
	INFO		m_tInfo;
	RECT		m_tRect;
	FRAME		m_tFrame;

	float		m_fSpeed;
	float		m_fAngle;
	float		m_fDistance;

	bool		m_bDestroyed;
	DIRECTION	m_eDir;
	RENDERID	m_eRenderID;

	CObj*		m_pTarget;

	const TCHAR*		m_pImgKey;
	int					m_iDrawID;
};

