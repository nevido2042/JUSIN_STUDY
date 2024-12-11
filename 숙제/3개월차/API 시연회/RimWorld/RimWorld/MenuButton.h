#pragma once
#include "Button.h"
class CMenuButton :
    public CButton
{
public:
	CMenuButton();
	virtual ~CMenuButton();
public:
	void Initialize()	override;
	int Update()		override;
	int Late_Update()	override;
	void Render(HDC hDC) override;
	void Release()		override;
};

