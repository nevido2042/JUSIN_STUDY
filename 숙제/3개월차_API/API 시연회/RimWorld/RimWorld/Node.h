#pragma once
#include "Define.h"

class CNode
{
	POS m_Pos;

	float m_F; //G+H
	float m_G;
	float m_H;

	DIRECTION m_Direction;

	CNode* m_pParent;

public:
	CNode();
	CNode(POS _Pos);
	CNode(POS _Pos, CNode* _pParent, POS _tEndPos);
	CNode(POS _Pos, CNode* _pParent, POS _tEndPos, DIRECTION _Dir);
public:
	POS Get_Pos() const
	{
		return m_Pos;
	}
	CNode* Get_Parent()
	{
		return m_pParent;
	}
	float Get_G() const
	{
		return m_G;
	}
	float Get_F() const
	{
		return m_F;
	}
	const DIRECTION Get_Direction() const
	{
		return m_Direction;
	}
public:
	static float Distance(POS _First, POS _Second);
	static float Manhattan_Distance(POS _First, POS _Second);
};

