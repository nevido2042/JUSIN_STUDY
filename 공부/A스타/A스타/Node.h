#pragma once
#include "Define.h"

class CNode
{
	POS m_Pos;

	float m_F; //G+H
	float m_G;
	float m_H;

	CNode* m_pParent;

public:
	CNode();
	CNode(POS _Pos);
	CNode(POS _Pos, CNode* _pParent, POS _EndPos);
public:
	POS Get_Pos()
	{
		return m_Pos;
	}
	CNode* Get_Parent()
	{
		return m_pParent;
	}
	int Get_F() const
	{
		return m_F;
	}
private:
	static float Distance(POS _First, POS _Second);
	static float Manhattan_Distance(POS _First, POS _Second);
};

