#pragma once
#include "Define.h"

class CUI
{
public:
	CUI();
	~CUI() {}

public:
	virtual void Initialize() PURE;
	virtual int Update() PURE;
	virtual void Late_Update()PURE;
	virtual void Render(HDC hdc)PURE;
	virtual void Release()PURE;

public:
	void Update_Rect();
public:
	INFO m_tInfo;
	RECT m_tRect;
};

