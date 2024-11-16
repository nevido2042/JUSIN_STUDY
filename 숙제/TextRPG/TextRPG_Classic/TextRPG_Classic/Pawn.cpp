#include "pch.h"
#include "Pawn.h"
#include "Define.h"

CPawn::CPawn()
	:m_iHP(0), m_iSTR(0), m_iMaxHP(0), m_iMoney(0), m_pWeapon(nullptr), m_pArmor(nullptr)
{
}

CPawn::~CPawn()
{
	Release();
}

void CPawn::Initialize()
{
	m_iHP = m_iMaxHP;
	if (m_pWeapon)
	{
		m_pWeapon->Initialize();
	}
	if (m_pArmor)
	{
		m_pArmor->Initialize();
	}
}

void CPawn::Render()
{
	cout << "�̸�: " << m_szName << endl;
	cout << "ü��: " << m_iHP << "/" << m_iMaxHP << " ";
	for (int i = 0; i < m_iHP; ++i)
	{
		cout << "��";
	}
	for (int i = 0; i < m_iMaxHP - m_iHP; ++i)
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

void CPawn::Release()
{
	Safe_Delete(m_pWeapon);
	Safe_Delete(m_pArmor);
}

//void CPawn::Release()
//{
//}

void CPawn::Render_Equipment()
{
	cout << "[�������� ���]" << endl;
	cout << "<����>" << endl;
	if (m_pWeapon)
	{
		m_pWeapon->Render();
		//cout << m_pWeapon->Get_Name();
	}
	cout << endl;

	cout << "<����>" << endl;
	if (m_pArmor)
	{
		m_pArmor->Render();
		//cout << m_pArmor->Get_Name();
	}
	cout << endl;
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
