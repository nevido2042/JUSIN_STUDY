#pragma once
#include "Obj.h"
class Bullet :public Obj
{
public:
	Bullet();
	Bullet(float _fAngle) : m_iType(BM_END) { m_fAngle = _fAngle; }
	virtual ~Bullet();
public:
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;
public:

	virtual void Initialize() PURE;
	virtual int Update() PURE;

public:
	void	Set_Type(BULLETMAKER _iType) { m_iType = _iType; }
	BULLETMAKER		Get_Type() { return m_iType; }
protected:
	BULLETMAKER		m_iType; // ���Ͱ� ����� �÷��̾ ����� ����
};

