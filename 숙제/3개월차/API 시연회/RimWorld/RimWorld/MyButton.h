#pragma once

#include "Button.h"

class CMyButton : public CButton
{
public:
	CMyButton();
	virtual ~CMyButton();

public:
	void Initialize()	override;
	void Late_Update()	override;
	void Render(HDC hDC) override;
	void Release()		override;

};

