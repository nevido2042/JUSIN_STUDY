#pragma once

#include "Define.h"

class CObj
{
public:
	CObj();
	virtual ~CObj();

public:
	INFO			Get_Info() { return m_tInfo; }
	INFO* Get_pInfo() { return &m_tInfo; }
	const RECT* Get_Rect() { return &m_tRect; }

	const POINT& Get_Point() { return { (long)m_tInfo.fX, (long)m_tInfo.fY }; }
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

	void		Set_OBJID(OBJID eOBJID) { m_eOBJID = eOBJID; }
	OBJID		Get_OBJID() { return m_eOBJID; }

public:
	virtual void		Initialize()PURE;
	virtual int			Update()PURE;
	virtual void		Late_Update()PURE;
	virtual void		Render(HDC hDC)PURE;
	virtual void		Release()PURE;
	virtual void OnCollision(CObj* _pOther, OBJID _eOtherID) {}

public:
	void		Update_Rect();
	void Set_LineY(const float& _fY) { m_fLineY = _fY; }
	void Set_CellingY(const float& _fY) { m_fCellingY = _fY; }

protected:
	void		Jumping();


	INFO		m_tInfo;
	RECT		m_tRect;

	float		m_fSpeed;
	float		m_fAngle;
	float		m_fDistance;

	bool		m_bDead;
	DIRECTION	m_eDir;

	CObj* m_pTarget;

	//파일 로드할 때 어떤 오브젝트인지 판단 할 수 있도록
	OBJID		m_eOBJID;

	float               m_fLineY; // 바닥충돌용 Y좌표
	float               m_fCellingY; // 천장충돌용 Y 좌표
	bool                m_bJump;
	float               m_fJumpPower;
	float               m_fTime;

	bool		m_bGround;
};

