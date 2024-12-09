#pragma once
#include "CBullet.h"
class CSuper_Arm : public CBullet
{
public:
	CSuper_Arm();
	virtual ~CSuper_Arm();

public:
	virtual void Initialize()	 override;
	virtual int Update()		 override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release()		 override;
	void	Set_JumpPower(float _JumpPower) { m_fJumpPower = _JumpPower; }

private:
	float	m_fJumpPower;
	float	m_fTime;
	int		m_iHp;

};

