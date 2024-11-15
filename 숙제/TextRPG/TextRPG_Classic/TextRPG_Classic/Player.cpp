#include "pch.h"
#include "Player.h"

CPlayer::CPlayer()
{

}

CPlayer::~CPlayer()
{
}

void CPlayer::Initialize()
{
	m_iMoney = 100;
	CPawn::Initialize();
}

void CPlayer::Update()
{
}

void CPlayer::Relase()
{
}
