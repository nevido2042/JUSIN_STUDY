#pragma once

#include "Button.h"

class CMyButton : public CObj
{
public:
	CMyButton();
	virtual ~CMyButton();
public:
	virtual void Set_Activate(bool _bActivate) { m_bActivate = true; }
public:
	void	Initialize()	override;
	int		Update() override;
	void	Late_Update()	override;
	void	Render(HDC hDC) override;
	void	Release()		override;




};

