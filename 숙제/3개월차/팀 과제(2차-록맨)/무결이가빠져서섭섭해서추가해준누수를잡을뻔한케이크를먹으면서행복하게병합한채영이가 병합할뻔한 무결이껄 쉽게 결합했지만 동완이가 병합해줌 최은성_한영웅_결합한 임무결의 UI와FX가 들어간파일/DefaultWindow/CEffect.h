#pragma once
#include "CObj.h"
class CEffect : public CObj
{
public:
	CEffect();
	~CEffect();
public:
	

private:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	int iCount;

	IMAGEINFO m_tCutState;
	IMAGEINFO m_tBoom01;
	IMAGEINFO m_tBoom02;
	IMAGEINFO m_tBoom03;
	IMAGEINFO m_tBoom04;
	IMAGEINFO m_tBoom05;
	IMAGEINFO m_tBoom06;
	IMAGEINFO m_tBoom07;
	IMAGEINFO m_tBoom08;
	IMAGEINFO m_tBoom09;
	IMAGEINFO m_tBoom10;
	IMAGEINFO m_tBoom11;
	IMAGEINFO m_tBoom12;
	IMAGEINFO m_tBoom13;
	IMAGEINFO m_tBoom14;
	IMAGEINFO m_tBoom15;
	IMAGEINFO m_tBoom16;
	IMAGEINFO m_tBoom17;
	IMAGEINFO m_tBoom18;
	IMAGEINFO m_tBoom19;

};

