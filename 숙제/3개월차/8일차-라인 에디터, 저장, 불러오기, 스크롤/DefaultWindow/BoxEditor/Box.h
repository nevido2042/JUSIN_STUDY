#pragma once

#include "Define.h"

class CBox
{
public:
	CBox();
	CBox(LINEPOINT& _tLT, LINEPOINT& _tRT, LINEPOINT& _tLB, LINEPOINT& _tRB);
	CBox(BOX& _tInfo);
	~CBox();

public:
	const BOX* Get_Info() { return &m_tInfo; }

public:
	void		Render(HDC _hDC);

private:
	BOX		m_tInfo;

};

