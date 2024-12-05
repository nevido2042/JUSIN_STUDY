#pragma once

#include "Define.h"

class CBox
{
public:
	CBox();
	CBox(LINEPOINT& _tLeft, LINEPOINT& _tTop, LINEPOINT& _tRight, LINEPOINT& _tBottom);
	CBox(BOX& tInfo);
	~CBox();

public:
	const BOX* Get_Info() { return &m_tInfo; }

public:
	void		Render(HDC hDC);

private:
	BOX		m_tInfo;

};

