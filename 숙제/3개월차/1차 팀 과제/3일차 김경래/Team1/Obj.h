#pragma once
#include "Define.h"
class Obj
{
public:
	Obj();
	virtual ~Obj();
public:
	const RECT* Get_Rect() { return &m_tRect; }
	INFO Get_Info() { return m_tInfo; }
	void		Set_Pos(float _fX, float _fY)
	{
		m_tInfo.fX = _fX;
		m_tInfo.fY = _fY;
	}
	
public:
	virtual void Initialize() PURE;
	virtual int Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC _hdc) PURE;
	virtual void Release() PURE;
public:
	void		Update_Rect();
	void Set_Dead() { m_bDead = true; }
	void		Set_Hp(int _iHp) { m_iHp = _iHp; }
	void		Set_Damage(int _iDamage) { m_iDamage = m_iDamage; }
	int			Get_Hp() { return m_iHp; }
	int			Get_Damage() { return m_iDamage; }
	void		Set_Target(Obj* _Target) { m_pTarget = _Target; }
	Obj*		Get_Target(){ return m_pTarget; }
protected:
	INFO		m_tInfo;
	RECT		m_tRect;


	float		m_fAngle;
	float		m_fSpeed;
	float		m_fDistance;

	bool		m_bDead;
	int			m_iHp;
	int			m_iDamage;
	Obj*		m_pTarget;
};

