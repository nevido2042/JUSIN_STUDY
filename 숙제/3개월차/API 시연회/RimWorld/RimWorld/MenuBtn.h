#pragma once
#include "Button.h"
class CMenuBtn :
    public CButton
{
public:
	CMenuBtn();
	virtual ~CMenuBtn();
public:
	void Initialize()	override;
};

