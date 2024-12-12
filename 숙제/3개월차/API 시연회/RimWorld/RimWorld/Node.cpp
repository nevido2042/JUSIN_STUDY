#include "pch.h"
#include "Node.h"

CNode::CNode()
	:m_Pos(0,0),
	m_F(0), m_G(0), m_H(0), 
	m_pParent(nullptr),
	m_Direction(OO)
{
}

CNode::CNode(POS _Pos)
	:m_Pos(_Pos),
	m_F(0), m_G(0), m_H(0),
	m_pParent(nullptr),
	m_Direction(OO)
{
	
}

CNode::CNode(POS _Pos, const CNode* _pParent, POS _EndPos)
	:m_Pos(_Pos), m_pParent(_pParent),
	m_Direction(OO)
{
	m_G = Distance(_Pos, _pParent->Get_Pos());//이 노드와 부모와의 거리
	m_H = Manhattan_Distance(_Pos, _EndPos); //이 노드와 목적지 까지의 거리(맨해튼 별론데?)

	m_F = m_G + m_H;
}

CNode::CNode(POS _Pos, const CNode* _pParent, POS _EndPos, DIRECTION _Dir)
	:m_Pos(_Pos), m_pParent(_pParent),
	m_Direction(_Dir)
{
	m_G = Distance(_Pos, _pParent->Get_Pos()) + _pParent->Get_G();//이 노드와 부모와의 거리 + 부모의 G값
	m_H = Manhattan_Distance(_Pos, _EndPos); //이 노드와 목적지 까지의 거리(맨해튼 별론데?)

	m_F = m_G + m_H;
}

float CNode::Distance(POS _First, POS _Second)
{
	return 	(float)sqrt(pow(abs(_First.fX - _Second.fX), 2) + pow(abs(_First.fY - _Second.fY), 2));
}

float CNode::Manhattan_Distance(POS _First, POS _Second)
{
	return (float)abs(_First.fX - _Second.fX) + abs(_First.fY - _Second.fY);
}
