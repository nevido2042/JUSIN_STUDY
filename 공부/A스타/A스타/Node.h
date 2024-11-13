#pragma once
#include "Define.h"

class CNode
{
	POS m_Pos;

	int m_F;
	int m_G;
	int m_H;

	CNode* m_pParent;

public:
	CNode();
	CNode(POS _Pos);
public:
	POS Get_Pos()
	{
		return m_Pos;
	}

};

