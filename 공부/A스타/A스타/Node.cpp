#include "pch.h"
#include "Node.h"
#include <math.h>

CNode::CNode()
	:m_Pos(0,0),
	m_F(0), m_G(0), m_H(0), 
	m_pParent(nullptr)
{
}

CNode::CNode(POS _Pos)
	:m_Pos(_Pos),
	m_F(0), m_G(0), m_H(0),
	m_pParent(nullptr)
{
	
}

CNode::CNode(POS _Pos, CNode* _pParent, POS _EndPos)
	:m_Pos(_Pos), m_pParent(_pParent)
{
	m_G = Distance(_Pos, _pParent->Get_Pos());//�� ���� �θ���� �Ÿ�
	m_H = Distance(_Pos, _EndPos); //�� ���� ������ ������ �Ÿ�(����ư ���е�?)

	m_F = m_G + m_H;
}

float CNode::Distance(POS _First, POS _Second)
{
	return 	sqrt(pow(abs(_First.iX - _Second.iX), 2) + pow(abs(_First.iY - _Second.iY), 2));
}

float CNode::Manhattan_Distance(POS _First, POS _Second)
{
	return abs(_First.iX - _Second.iX) + abs(_First.iY - _Second.iY);
}
