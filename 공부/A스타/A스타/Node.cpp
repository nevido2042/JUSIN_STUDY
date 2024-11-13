#include "pch.h"
#include "Node.h"

CNode::CNode()
	:m_Pos(0,0),
	m_F(0), m_G(0), m_H(0), 
	m_pParent(nullptr)
{
}

CNode::CNode(POS _Pos)
	:m_Pos(_Pos)
{
	CNode();
}
