#include "pch.h"
#include "CBullet_Normal.h"

CBullet_Normal::CBullet_Normal()
{
}

CBullet_Normal::~CBullet_Normal()
{
}

void CBullet_Normal::Initialize()
{
}

int CBullet_Normal::Update()
{
	if (m_bDead)
		return OBJ_DEAD;
}

void CBullet_Normal::Late_Update()
{
}

void CBullet_Normal::Render(HDC hDC)
{
}

void CBullet_Normal::Release()
{
}
