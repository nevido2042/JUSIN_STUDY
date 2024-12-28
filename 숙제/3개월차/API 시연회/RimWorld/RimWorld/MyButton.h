#pragma once
#include "Obj.h"

class CMyButton : public CObj
{
public:
	CMyButton();
	virtual ~CMyButton();
public:
	virtual void Set_Activate(bool _bActivate) override { m_bActivate = _bActivate; }
public:
	void	Initialize()	override;
	int		Update() override;
	void	Late_Update()	override;
	void	Render(HDC hDC) override;
	void	Release()		override;
protected:
	bool	m_bOnHovered;




};

