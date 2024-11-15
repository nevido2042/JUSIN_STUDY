#include "pch.h"
#include "Player.h"
#include "Define.h"

CPlayer::CPlayer()
	:m_pInventory(nullptr)
{

}

CPlayer::~CPlayer()
{
	Relase();
}

void CPlayer::Initialize()
{
	m_iMoney = 100;
	CPawn::Initialize();

	m_pInventory = new CInventory;
}

void CPlayer::Update()
{
}

void CPlayer::Relase()
{
	Safe_Delete(m_pInventory);
}
