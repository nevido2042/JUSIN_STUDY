#include "pch.h"
#include "CWall.h"

CWall::CWall()
{
}



CWall::~CWall()
{
}

void CWall::Initialize()
{
}

int CWall::Update()
{
	return 0;
}

void CWall::Late_Update()
{
}

void CWall::Render(HDC hDC)
{
    Rectangle(hDC,
        m_tRect.left,
        m_tRect.top,
        m_tRect.right,
        m_tRect.bottom);
}

void CWall::Release()
{
}
