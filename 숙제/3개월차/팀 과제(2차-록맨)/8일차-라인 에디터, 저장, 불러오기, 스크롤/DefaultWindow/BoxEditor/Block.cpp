#include "pch.h"
#include "Block.h"

CBlock::CBlock()
{
}

CBlock::CBlock(float _fX, float _fY, float _fSize)
{
    Set_Pos(_fX, _fX);
    Set_Size(_fSize, _fSize);
}

CBlock::~CBlock()
{
}

void CBlock::Initialize()
{
    m_tInfo.fCX = 50.f;
    m_tInfo.fCY = 50.f;
    m_fSpeed = 3.f;
}

int CBlock::Update()
{
    if (m_bDead)
        return OBJ_DEAD;

    __super::Update_Rect();

    return OBJ_NOEVENT;
}

void CBlock::Late_Update()
{
}

void CBlock::Render(HDC hDC)
{
    Rectangle(hDC,
        m_tRect.left,
        m_tRect.top,
        m_tRect.right,
        m_tRect.bottom);
}

void CBlock::Release()
{
}
