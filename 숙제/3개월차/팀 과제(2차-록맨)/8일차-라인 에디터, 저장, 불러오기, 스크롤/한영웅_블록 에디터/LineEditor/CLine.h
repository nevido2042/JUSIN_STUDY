#pragma once

#include "Define.h"

class CLine
{
public:
	CLine();
	CLine(LINEPOINT& tLeft, LINEPOINT& tRight);
	CLine(LINE& tInfo);
	~CLine();

public:
	const LINE* Get_Info() { return &m_tInfo; }

public:
	void		Render(HDC hDC);

private:
	LINE		m_tInfo;

};

