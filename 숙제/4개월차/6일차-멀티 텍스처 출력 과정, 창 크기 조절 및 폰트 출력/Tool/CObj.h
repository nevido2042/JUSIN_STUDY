#pragma once
#include "CDevice.h"

class CObj
{
public:
	CObj();
	virtual ~CObj();
public:
	INFO&	Get_Info();
	void	Set_Pos(float _fX, float _fY, float _fZ);
public:
	virtual void Initialize();
	virtual void Render(CDevice* _pDevice);
protected:
	INFO			m_tInfo;
	const TEXINFO*	m_pTexInfo;
};

