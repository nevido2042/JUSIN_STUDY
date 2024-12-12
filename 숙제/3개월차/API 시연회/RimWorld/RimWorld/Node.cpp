#include "pch.h"
#include "Node.h"

CNode::CNode()
	:m_Pos(0,0),
	m_F(0), m_G(0), m_H(0), 
	m_pParent(nullptr),
	m_Direction(OO)
{
}

CNode::CNode(POS _tPos)
	:m_Pos(_tPos),
	m_F(0), m_G(0), m_H(0),
	m_pParent(nullptr),
	m_Direction(OO)
{
	
}

CNode::CNode(POS _tPos, const CNode* _pParent, POS _tEndPos)
	:m_Pos(_tPos), m_pParent(_pParent),
	m_Direction(OO)
{
	m_G = Distance(_tPos, _pParent->Get_Pos());//�� ���� �θ���� �Ÿ�
	m_H = Manhattan_Distance(_tPos, _tEndPos); //�� ���� ������ ������ �Ÿ�(����ư ���е�?)

	m_F = m_G + m_H;
}

CNode::CNode(POS _tPos, const CNode* _pParent, POS _tEndPos, DIRECTION _Dir)
	:m_Pos(_tPos), m_pParent(_pParent),
	m_Direction(_Dir)
{
	m_G = Distance(_tPos, _pParent->Get_Pos()) + _pParent->Get_G();//�� ���� �θ���� �Ÿ� + �θ��� G��
	m_H = Manhattan_Distance(_tPos, _tEndPos); //�� ���� ������ ������ �Ÿ�(����ư ���е�?)

	m_F = m_G + m_H;
}

float CNode::Distance(POS _tFirst, POS _tSecond)
{
	return 	(float)sqrt(pow(abs(_tFirst.fX - _tSecond.fX), 2) + pow(abs(_tFirst.fY - _tSecond.fY), 2));
}

float CNode::Manhattan_Distance(POS _tFirst, POS _tSecond)
{
	return (float)abs(_tFirst.fX - _tSecond.fX) + abs(_tFirst.fY - _tSecond.fY);
}
