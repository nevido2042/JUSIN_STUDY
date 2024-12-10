#pragma once
#include "CObj.h"
class CWall : public CObj

{
public:
	CWall();
	CWall(float _x, float _y, float _angle)
	{
		m_tInfo.fX = _x;
		m_tInfo.fY = _y;
		m_fAngle = _angle;
	}
	~CWall();

public:


private:


	// CObj��(��) ���� ��ӵ�
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

};

