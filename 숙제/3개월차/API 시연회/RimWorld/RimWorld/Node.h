#pragma once
#include "Define.h"

class CNode
{
	POS m_Pos;

	float m_F; //G+H
	float m_G;
	float m_H;

	DIRECTION m_Direction;

	const CNode* m_pParent;

public:
	CNode();
	CNode(POS _Pos);
	CNode(POS _Pos, const CNode* _pParent, POS _tEndPos);
	CNode(POS _Pos, const CNode* _pParent, POS _tEndPos, DIRECTION _Dir);
public:
	POS Get_Pos() const
	{
		return m_Pos;
	}
	const CNode* Get_Parent() const
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
	DIRECTION Get_Direction()
	{
		return m_Direction;
	}
public:
	static float Distance(POS _First, POS _Second);
	static float Manhattan_Distance(POS _First, POS _Second);
};

