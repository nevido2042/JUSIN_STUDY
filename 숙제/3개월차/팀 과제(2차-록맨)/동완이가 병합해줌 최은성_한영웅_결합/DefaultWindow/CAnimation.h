#pragma once
#include "Define.h"
#include "CObj.h"

class CAnimation
{
public:
	CAnimation() : m_iCurFrame(0), m_pTarget(nullptr), m_ullTime(GetTickCount64()){}
	~CAnimation() { Release(); }

public:
	void	Update();
	void	Late_Update();
	void	Initialize();
	void	Render(HDC _hDC);
	void	Release();

public:
	void	Set_Target(CObj* _Target) { m_pTarget = _Target; }
	void	Set_hMemDC(HDC	_hMemDC) { m_hMemDC = _hMemDC; }
	void	Set_Vector(ANINFO _AnInfo);

public:
	//ANINFO	Get_AnInfo() const { return m_tAnInfo; };
	void	Create_Animation();


private:
	vector<ANINFO>	m_vecAnInfo;
	CObj*			m_pTarget;
	HDC				m_hMemDC;
	int				m_iCurFrame;
	ULONGLONG		m_ullTime;
};

