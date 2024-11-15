#include "pch.h"
#include "Pawn.h"

CPawn::CPawn()
	:m_iHP(0), m_iSTR(0), m_iMaxHP(0), m_iMoney(0)
{
}

CPawn::~CPawn()
{
}

void CPawn::Initialize()
{
	m_iHP = m_iMaxHP;
}

void CPawn::Render()
{
	cout << "�̸�: " << m_szName << endl;
	cout << "ü��: " << m_iHP << " ";
	for (int i = 0; i < m_iHP; ++i)
	{
		cout << "��";
	}
	cout << endl;

	cout << "���ݷ�: " << m_iSTR << " ";
	for (int i = 0; i < m_iSTR; ++i)
	{
		cout << "��";
	}
	cout << endl;

	cout << "������: " << m_iMoney << endl;
}

void CPawn::Set_HP(int _iHP)
{
	m_iHP = _iHP;
}

void CPawn::Set_STR(int _iSTR)
{
	m_iSTR = _iSTR;
}

void CPawn::Attack(CObj* _pTarget)
{
	if (CPawn* pTarget = dynamic_cast<CPawn*>(_pTarget))
	{
		pTarget->Set_HP(pTarget->Get_HP() - Get_STR());
	}
}
