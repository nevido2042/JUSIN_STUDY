#pragma once

#include "Define.h"

class CBox
{
public:
	CBox();
	CBox(LINEPOINT _tCenter, float _fSize);
	CBox(BOX& _tInfo);
	~CBox();

public:
	const BOX* Get_Info() { return &m_tInfo; }

public:
	void		Render(HDC _hDC);

private:
	BOX		m_tInfo;

};

